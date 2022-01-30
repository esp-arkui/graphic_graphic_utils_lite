/*
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef AGG_BLUR_INCLUDED
#define AGG_BLUR_INCLUDED

#include <cmath>
#include <cstring>

#include "graphic_config.h"
#include "render/graphic_render_base.h"
#include "render/graphic_render_pixfmt_base.h"
#include "render/graphic_render_pixfmt_transposer.h"
#include "securec.h"

namespace OHOS {
class GraphicFilterblur {
#if GRAPHIC_ENABLE_BLUR_EFFECT_FLAG

public:
    GraphicFilterblur()
    {
        integral_ = nullptr;
        imageWidth_ = 0;
        imageHeight_ = 0;
    }
    ~GraphicFilterblur()
    {
        if (integral_ != nullptr) {
            free(integral_);
        }
    }

    template <class Img>
    void BoxBlur(Img& img, uint16_t radius, int32_t channel, int32_t stride)
    {
        if (radius < 1) {
            return;
        }
        int32_t width = img.Width();
        int32_t height = img.Height();
        bool isGetRGBAIntegral = false;
        if (integral_ == nullptr || ((imageWidth_ * imageHeight_) != (width * height))) {
            if (integral_ != nullptr) {
                free(integral_);
            }
            integral_ = (int32_t*)malloc((width + 1) * (height + 1) * channel * sizeof(int32_t));
            isGetRGBAIntegral = true;
        }
        if (channel == FOUR_TIMES) {
            if (isGetRGBAIntegral) {
                GetRGBAIntegralImage((uint8_t*)img.PixValuePtr(0, 0), width, height, stride);
            }
#pragma omp parallel for
            for (int32_t y = 0; y < height; y++) {
                int32_t y1 = MATH_MAX(y - radius, 0);
                int32_t y2 = MATH_MIN(y + radius + 1, height);
                uint8_t* lineImageHeader = (uint8_t*)img.PixValuePtr(0, 0) + y * stride;
                uint8_t* linePD = lineImageHeader;
                int32_t* lineP1 = integral_ + y1 * ((width + 1) << INDEX_TWO);
                int32_t* lineP2 = integral_ + y2 * ((width + 1) << INDEX_TWO);

                for (int x = 0; x < width; x++) {
                    int x1 = MATH_MAX(x - radius, 0);
                    int x2 = MATH_MIN(x + radius + 1, width);
                    int index1 = x1 << INDEX_TWO;
                    int index2 = x2 << INDEX_TWO;
                    int sumB = lineP2[index2 + INDEX_ZERO] - lineP1[index2 + INDEX_ZERO] -
                               lineP2[index1 + INDEX_ZERO] + lineP1[index1 + INDEX_ZERO];
                    int sumG = lineP2[index2 + INDEX_ONE] - lineP1[index2 + INDEX_ONE] -
                               lineP2[index1 + INDEX_ONE] + lineP1[index1 + INDEX_ONE];
                    int sumR = lineP2[index2 + INDEX_TWO] - lineP1[index2 + INDEX_TWO] -
                               lineP2[index1 + INDEX_TWO] + lineP1[index1 + INDEX_TWO];

                    const int32_t pixelCount = (x2 - x1) * (y2 - y1);
                    linePD[INDEX_ZERO] = (sumB + (pixelCount >> INDEX_ONE)) / pixelCount;
                    linePD[INDEX_ONE] = (sumG + (pixelCount >> INDEX_ONE)) / pixelCount;
                    linePD[INDEX_TWO] = (sumR + (pixelCount >> INDEX_ONE)) / pixelCount;
                    uint8_t* alpha = lineImageHeader + (x << INDEX_TWO);
                    linePD[INDEX_THREE] = alpha[INDEX_THREE];
                    linePD += INDEX_FOUR;
                }
            }
        }
    }
private:
    void GetRGBAIntegralImage(uint8_t* src, uint16_t width, uint16_t height, uint16_t stride)
    {
        int channel = FOUR_TIMES;
        if (integral_ != nullptr && ((imageWidth_ * imageHeight_) != (width * height))) {
            int integralSize = (width + 1) * channel * sizeof(int32_t);
            memset_s(integral_, integralSize, 0, integralSize);
        }
        for (int y = 0; y < height; y++) {
            uint8_t* linePS = src + y * stride;
            //	last position
            int* linePL = integral_ + y * (width + 1) * channel + channel;
            //	curretn position，waring the first column of every line row is zero
            int* linePD = integral_ + (y + 1) * (width + 1) * channel + channel;
            //	the first column is 0
            linePD[-INDEX_FOUR] = 0;
            linePD[-INDEX_THREE] = 0;
            linePD[-INDEX_TWO] = 0;
            linePD[-INDEX_ONE] = 0;
            for (int x = 0, sumB = 0, sumG = 0, sumR = 0, sumA = 0; x < width; x++) {
                sumB += linePS[INDEX_ZERO];
                sumG += linePS[INDEX_ONE];
                sumR += linePS[INDEX_TWO];
                sumA += linePS[INDEX_THREE];
                linePD[INDEX_ZERO] = linePL[INDEX_ZERO] + sumB;
                linePD[INDEX_ONE] = linePL[INDEX_ONE] + sumG;
                linePD[INDEX_TWO] = linePL[INDEX_TWO] + sumR;
                linePD[INDEX_THREE] = linePL[INDEX_THREE] + sumA;
                linePS += channel;
                linePL += channel;
                linePD += channel;
            }
        }
    }

    int32_t* integral_;
    int32_t imageWidth_;
    int32_t imageHeight_;
#endif
};
} // namespace OHOS
#endif
