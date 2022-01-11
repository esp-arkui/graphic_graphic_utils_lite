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
    class FastBoxBlur {
#if GRAPHIC_GEOMETYR_ENABLE_BLUR_EFFECT_VERTEX_SOURCE
    private:
        void static GetRGBAIntegralImage(uint8_t *Src, int *Integral, uint16_t Width, uint16_t Height, uint16_t Stride)
        {
            int Channel = Stride / Width;
            if(Channel < FOUR_TIMES) {
                return;
            }
            memset(Integral, 0, (Width + 1) * Channel * sizeof(int));

            for (int Y = 0; Y < Height; Y++)
            {
                unsigned char *LinePS = Src + Y * Stride;
                //	last position
                int *LinePL = Integral + Y * (Width + 1) * Channel + Channel;
                //	curretn position，waring the first column of every line row is zero
                int *LinePD = Integral + (Y + 1) * (Width + 1) * Channel + Channel;
                //	the first column is 0
                LinePD[-4] = 0;
                LinePD[-3] = 0;
                LinePD[-2] = 0;
                LinePD[-1] = 0;
                for (int X = 0, SumB = 0, SumG = 0, SumR = 0, SumA = 0; X < Width; X++)
                {
                    SumB += LinePS[0];
                    SumG += LinePS[1];
                    SumR += LinePS[2];
                    SumA += LinePS[3];
                    LinePD[0] = LinePL[0] + SumB;
                    LinePD[1] = LinePL[1] + SumG;
                    LinePD[2] = LinePL[2] + SumR;
                    LinePD[3] = LinePL[3] + SumA;
                    LinePS += Channel;
                    LinePL += Channel;
                    LinePD += Channel;
                }
            }
        }

        void static BoxBlur(uint8_t *Src, unsigned char *Dest, uint16_t Width, uint16_t Height, uint16_t Stride, uint16_t Radius)
        {
            int32_t Channel = Stride / Width;
            int32_t *Integral = (int32_t *)malloc((Width + 1) * (Height + 1) * Channel * sizeof(int32_t));
            if (Channel == 4)
            {
                GetRGBAIntegralImage(Src, Integral, Width, Height, Stride);
                #pragma omp parallel for
                for (int32_t Y = 0; Y < Height; Y++)
                {
                    int32_t Y1 = MATH_MAX(Y - Radius, 0);
                    int32_t Y2 = MATH_MIN(Y + Radius + 1, Height);
                    int32_t *LineP1 = Integral + Y1 * (Width + 1) * 4;
                    int32_t *LineP2 = Integral + Y2 * (Width + 1) * 4;
                    unsigned char *LinePD = Dest + Y * Stride;
                    for (int X = 0; X < Width; X++)
                    {
                        int X1 = MATH_MAX(X - Radius, 0);
                        int X2 = MATH_MIN(X + Radius + 1, Width);
                        int Index1 = X1 * 4;
                        int Index2 = X2 * 4;
                        int SumB = LineP2[Index2 + 0] - LineP1[Index2 + 0] - LineP2[Index1 + 0] + LineP1[Index1 + 0];
                        int SumG = LineP2[Index2 + 1] - LineP1[Index2 + 1] - LineP2[Index1 + 1] + LineP1[Index1 + 1];
                        int SumR = LineP2[Index2 + 2] - LineP1[Index2 + 2] - LineP2[Index1 + 2] + LineP1[Index1 + 2];
                        //int SumA = LineP2[Index2 + 3] - LineP1[Index2 + 3] - LineP2[Index1 + 3] + LineP1[Index1 + 3];
                        int PixelCount = (X2 - X1) * (Y2 - Y1);
                        LinePD[0] = (SumB + (PixelCount >> 1)) / PixelCount;
                        LinePD[1] = (SumG + (PixelCount >> 1)) / PixelCount;
                        LinePD[2] = (SumR + (PixelCount >> 1)) / PixelCount;
                        //LinePD[3] = (SumA + (PixelCount >> 1)) / PixelCount;
                        uint8_t* alpha = Src + Y * Stride + X * 4;
                        LinePD[3] = alpha[3];
                        LinePD += 4;
                    }
                }
            }
            free(Integral);
        }
    public:
        /**
         * @brief Blur
         * @since 1.0
         * @version 1.0
         */
        void BlurImageFilter(uint8_t* pImagePtr, unsigned radius)
        {
            //PixfmtTransposer<Img> img2(img);
            //BoxBlur(pImagePtr,pImagePtr,)
        }
#endif
    };
} // namespace OHOS
#endif
