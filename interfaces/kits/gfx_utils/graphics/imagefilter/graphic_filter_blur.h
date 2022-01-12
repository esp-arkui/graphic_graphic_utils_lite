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

#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_array.h"
#include "graphic_config.h"
#include "render/graphic_render_base.h"
#include "render/graphic_render_pixfmt_base.h"
#include "render/graphic_render_pixfmt_transposer.h"
#include "securec.h"
#ifdef ARM_NEON_OPT
#    include <arm_neon.h>
#endif
#if ENABLE_ARM_MATH
#    include "arm_math.h"
#endif
namespace OHOS {
    class FastBoxBlur {
#if GRAPHIC_GEOMETYR_ENABLE_BLUR_EFFECT_VERTEX_SOURCE
    private:
        void GetRGBAIntegralImage(uint8_t* Src, uint16_t Width, uint16_t Height, uint16_t Stride)
        {
            int Channel = FOUR_TIMES;
            if (Integral != nullptr && ((imageWidth * imageHeight) != (Width * Height))) {
                int integralSize = (Width + 1) * Channel * sizeof(int32_t);
                memset_s(Integral, integralSize, 0, integralSize);
            }
#ifdef ARM_NEON_OPT
            for (int Y = 0; Y < Height; Y++) {
                int32_t* LinePS_ = Src + Y * Stride;
                int32x4x4_t LinePS = vld4q_s32(LinePS_);
                int32x4x4_t LinePL = vld4q_s32(Integral + Y * (Width + 1) * Channel + Channel);
                int32x4x4_t LinePD = vld4q_s32(Integral + (Y + 1) * (Width + 1) * Channel + Channel);

                LinePD.val[-INDEX_FOUR] = 0;
                LinePD.val[-INDEX_THREE] = 0;
                LinePD.val[-INDEX_TWO] = 0;
                LinePD.val[-INDEX_ONE] = 0;
                int32x4_t SumB, SumG, SumR, SumA;

                int16_t tmpWidth = Width;
                while (tmpWidth >= 4) {
                    SumB = vaddq_s32(LinePS.val[INDEX_ZERO], SumB);
                    SumG = vaddq_s32(LinePS.val[INDEX_ONE], SumG);
                    SumR = vaddq_s32(LinePS.val[INDEX_TWO], SumR);
                    SumA = vaddq_s32(LinePS.val[INDEX_THREE], SumA);

                    LinePD.val[INDEX_ZERO] = vaddq_s32(LinePL.val[INDEX_ZERO], SumB);
                    LinePD.val[INDEX_ONE] = vaddq_s32(LinePL.val[INDEX_ONE], SumG);
                    LinePD.val[INDEX_TWO] = vaddq_s32(LinePL.val[INDEX_TWO], SumR);
                    LinePD.val[INDEX_THREE] = vaddq_s32(LinePL.val[INDEX_THREE], SumA);

                    vst4q_s32(Integral + Y * (Width + 1) * Channel + Channel, LinePL);
                    vst4q_s32(Integral + (Y + 1) * (Width + 1) * Channel + Channel, LinePD);

                    LinePS = vaddq_s32(LinePS, vdupq_n_s32(Channel));
                    LinePL = vaddq_s32(LinePL, vdupq_n_s32(Channel));
                    LinePD = vaddq_s32(LinePD, vdupq_n_s32(Channel));
                    tmpWidth -= 4;
                }
                if (tmpWidth > 0 && tmpWidth < 4) {
                    int* NormalLinePS = Src + Y * Stride;
                    int* NormalLinePL = Integral + Y * (Width + 1) * Channel + Channel;
                    int* NormalLinePD = Integral + (Y + 1) * (Width + 1) * Channel + Channel;
                    int32_t NormalSumB = 0, NormalSumG = 0, NormalSumR = 0, NormalSumA = 0;
                    vst1q_s32(&NormalSumB, SumB);
                    vst1q_s32(&NormalSumG, SumG);
                    vst1q_s32(&NormalSumR, SumR);
                    vst1q_s32(&NormalSumA, SumA);
                    for (int16_t i = 0; i < tmpWidth; ++i) {
                        NormalSumB += NormalLinePS[INDEX_ZERO];
                        NormalSumG += NormalLinePS[INDEX_ONE];
                        NormalSumR += NormalLinePS[INDEX_TWO];
                        NormalSumA += NormalLinePS[INDEX_THREE];
                        NormalLinePD[INDEX_ZERO] = NormalLinePL[INDEX_ZERO] + SumB;
                        NormalLinePD[INDEX_ONE] = NormalLinePL[INDEX_ONE] + SumG;
                        NormalLinePD[INDEX_TWO] = NormalLinePL[INDEX_TWO] + SumR;
                        NormalLinePD[INDEX_THREE] = NormalLinePL[INDEX_THREE] + SumA;
                        NormalLinePS += Channel;
                        NormalLinePL += Channel;
                        NormalLinePD += Channel;
                    }
                }
            }
#else
            for (int Y = 0; Y < Height; Y++) {
                uint8_t* LinePS = Src + Y * Stride;
                //	last position
                int* LinePL = Integral + Y * (Width + 1) * Channel + Channel;
                //	curretn position，waring the first column of every line row is zero
                int* LinePD = Integral + (Y + 1) * (Width + 1) * Channel + Channel;
                //	the first column is 0
                LinePD[-INDEX_FOUR] = 0;
                LinePD[-INDEX_THREE] = 0;
                LinePD[-INDEX_TWO] = 0;
                LinePD[-INDEX_ONE] = 0;
                for (int X = 0, SumB = 0, SumG = 0, SumR = 0, SumA = 0; X < Width; X++) {
                    SumB += LinePS[INDEX_ZERO];
                    SumG += LinePS[INDEX_ONE];
                    SumR += LinePS[INDEX_TWO];
                    SumA += LinePS[INDEX_THREE];
                    LinePD[INDEX_ZERO] = LinePL[INDEX_ZERO] + SumB;
                    LinePD[INDEX_ONE] = LinePL[INDEX_ONE] + SumG;
                    LinePD[INDEX_TWO] = LinePL[INDEX_TWO] + SumR;
                    LinePD[INDEX_THREE] = LinePL[INDEX_THREE] + SumA;
                    LinePS += Channel;
                    LinePL += Channel;
                    LinePD += Channel;
                }
            }
#endif
        }

    public:
        FastBoxBlur()
        {
            Integral = nullptr;
            imageWidth = imageHeight = 0;
        }
        ~FastBoxBlur()
        {
            if (Integral != nullptr) {
                free(Integral);
            }
        }

        template <class Img>
        void BoxBlur(Img& img, uint16_t Radius, int32_t Channel, int32_t Stride)
        {
            if (Radius < 1) {
                return;
            }
            int32_t Width = img.Width();
            int32_t Height = img.Height();
            bool isGetRGBAIntegral = false;
            if (Integral == nullptr || ((imageWidth * imageHeight) != (Width * Height))) {
                if (Integral != nullptr) {
                    free(Integral);
                }
                Integral = (int32_t*)malloc((Width + 1) * (Height + 1) * Channel * sizeof(int32_t));
                isGetRGBAIntegral = true;
            }
            if (Channel == FOUR_TIMES) {
                if (isGetRGBAIntegral) {
                    GetRGBAIntegralImage((uint8_t*)img.PixValuePtr(0, 0), Width, Height, Stride);
                }
                #pragma omp parallel for
                for (int32_t Y = 0; Y < Height; Y++) {
                    int32_t Y1 = MATH_MAX(Y - Radius, 0);
                    int32_t Y2 = MATH_MIN(Y + Radius + 1, Height);
                    uint8_t* LineImageHeader = (uint8_t*)img.PixValuePtr(0, 0) + Y * Stride;
                    uint8_t* LinePD = LineImageHeader;
                    int32_t* LineP1 = Integral + Y1 * ((Width + 1) << INDEX_TWO);
                    int32_t* LineP2 = Integral + Y2 * ((Width + 1) << INDEX_TWO);

                    for (int X = 0; X < Width; X++) {
                        int X1 = MATH_MAX(X - Radius, 0);
                        int X2 = MATH_MIN(X + Radius + 1, Width);
                        int Index1 = X1 << INDEX_TWO;
                        int Index2 = X2 << INDEX_TWO;
                        int SumB = LineP2[Index2 + INDEX_ZERO] - LineP1[Index2 + INDEX_ZERO] - LineP2[Index1 + INDEX_ZERO] + LineP1[Index1 + INDEX_ZERO];
                        int SumG = LineP2[Index2 + INDEX_ONE] - LineP1[Index2 + INDEX_ONE] - LineP2[Index1 + INDEX_ONE] + LineP1[Index1 + INDEX_ONE];
                        int SumR = LineP2[Index2 + INDEX_TWO] - LineP1[Index2 + INDEX_TWO] - LineP2[Index1 + INDEX_TWO] + LineP1[Index1 + INDEX_TWO];
#if ENABLE_ARM_MATH
                        const int32_t PixelCount = __SMUAD(X2 - X1, Y2 - Y1);
                        LinePD[INDEX_ZERO] = __UDIV((SumB + (PixelCount >> INDEX_ONE)), PixelCount);
                        LinePD[INDEX_ONE] = __UDIV(SumG + (PixelCount >> INDEX_ONE)) , PixelCount);
                        LinePD[INDEX_TWO] = __UDIV(SumR + (PixelCount >> INDEX_ONE)) , PixelCount);
#else
                        const int32_t PixelCount = (X2 - X1) * (Y2 - Y1);
                        LinePD[INDEX_ZERO] = (SumB + (PixelCount >> INDEX_ONE)) / PixelCount;
                        LinePD[INDEX_ONE] = (SumG + (PixelCount >> INDEX_ONE)) / PixelCount;
                        LinePD[INDEX_TWO] = (SumR + (PixelCount >> INDEX_ONE)) / PixelCount;
#endif
                        uint8_t* alpha = LineImageHeader + (X << INDEX_TWO);
                        LinePD[INDEX_THREE] = alpha[INDEX_THREE];
                        LinePD += INDEX_FOUR;
                    }
                }
            }
        }

    private:
        int32_t* Integral;
        int32_t imageWidth;
        int32_t imageHeight;
        //GeometryPlainDataVector<uint8_t> imageBufferVector;
#endif
    };
} // namespace OHOS
#endif
