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
#include "render/graphic_render_base.h"
#include "render/graphic_render_pixfmt_base.h"
#include "render/graphic_render_pixfmt_transposer.h"

namespace OHOS {
    template <class ColorType>
    class FastBoxBlur {
#if GRAPHIC_GEOMETYR_ENABLE_BLUR_EFFECT_VERTEX_SOURCE
    private:
        GeometryPlainDataVector<ColorType> imageBufferVector;
        template <class Img>
        void static GetRGBAIntegralImage(Img& img, int *Integral)
        {
            const int Channel = 4;
            memset(Integral, 0, (img.Width() + 1) * Channel * sizeof(int));
            for (int Y = 0; Y < img.Height(); Y++)
            {
                int *LinePL = Integral + Y * (img.Height() + 1) * Channel + Channel;
                int *LinePD = Integral + (Y + 1) * (img.Width() + 1) * Channel + Channel;
                LinePD[-4] = 0;
                LinePD[-3] = 0;
                LinePD[-2] = 0;
                LinePD[-1] = 0;

                for (int X = 0, SumB = 0, SumG = 0, SumR = 0 ,SumA =0; X < img.Width(); X++)
                {
                    SumB += img.Pixel(X, Y).blueValue;
                    SumG += img.Pixel(X, Y).greenValue;
                    SumR += img.Pixel(X, Y).redValue;
                    SumA += img.Pixel(X, Y).alphaValue;
                    LinePD[0] = LinePL[0] + SumB;
                    LinePD[1] = LinePL[1] + SumG;
                    LinePD[2] = LinePL[2] + SumR;
                    LinePD[3] = LinePL[3] + SumA;
                    LinePL += Channel;
                    LinePD += Channel;
                }
            }
        }
    public:
        template <class Img>
        void BoxBlur(Img& img, uint16_t Radius)
        {
            int Channel = 4;
            int *Integral = (int *)malloc((img.Width() + 1) * (img.Height() + 1) * Channel * sizeof(int));
            GetRGBAIntegralImage(img, Integral);
            for (int Y = 0; Y < img.Height(); Y++)
            {
                int Y1 = MATH_MAX(Y - Radius, 0);
                int Y2 = MATH_MIN(Y + Radius + 1, img.Height());
                int *LineP1 = Integral + Y1 * (img.Width() + 1) * Channel;
                int *LineP2 = Integral + Y2 * (img.Width() + 1) * Channel;
                imageBufferVector.Allocate(img.Width(), 128);
                for (int X = 0; X < img.Width(); X++)
                {
                    int X1 = MATH_MAX(X - Radius, 0);
                    int X2 = MATH_MIN(X + Radius + 1, img.Width());
                    int Index1 = X1 * Channel;
                    int Index2 = X2 * Channel;
                    int SumB = LineP2[Index2 + 0] - LineP1[Index2 + 0] - LineP2[Index1 + 0] + LineP1[Index1 + 0];
                    int SumG = LineP2[Index2 + 1] - LineP1[Index2 + 1] - LineP2[Index1 + 1] + LineP1[Index1 + 1];
                    int SumR = LineP2[Index2 + 2] - LineP1[Index2 + 2] - LineP2[Index1 + 2] + LineP1[Index1 + 2];
                    int SumA = LineP2[Index2 + 3] - LineP1[Index2 + 3] - LineP2[Index1 + 3] + LineP1[Index1 + 3];
                    int PixelCount = (X2 - X1) * (Y2 - Y1);
                    ColorType color;
                    color.blueValue = (SumB + (PixelCount >> 1)) / PixelCount;
                    color.greenValue = (SumG + (PixelCount >> 1)) / PixelCount;
                    color.redValue = (SumR + (PixelCount >> 1)) / PixelCount;
                    color.alphaValue = img.Pixel(X, Y).alphaValue;
                    imageBufferVector[X] = color;
                }
                img.CopyColorHspan(0, Y, img.Width(), &imageBufferVector[0]);
            }
            free(Integral);
        }
#endif
    };
} // namespace OHOS
#endif
