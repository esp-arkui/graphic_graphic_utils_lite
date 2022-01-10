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
                LinePD[-INDEX_FOUR] = 0;
                LinePD[-INDEX_THREE] = 0;
                LinePD[-INDEX_TWO] = 0;
                LinePD[-INDEX_ONE] = 0;
                for (int X = 0, SumB = 0, SumG = 0, SumR = 0, SumA = 0; X < Width; X++)
                {
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
        }
    public:
        void static BoxBlur(uint8_t *Src, uint8_t *Dest, uint16_t Width, uint16_t Height, uint16_t Stride, uint16_t Radius)
        {
            int32_t Channel = Stride / Width;
            int32_t *Integral = (int32_t *)malloc((Width + 1) * (Height + 1) * Channel * sizeof(int32_t));
            if (Channel == FOUR_TIMES)
            {
                GetRGBAIntegralImage(Src, Integral, Width, Height, Stride);
                #pragma omp parallel for
                for (int32_t Y = 0; Y < Height; Y++)
                {
                    int32_t Y1 = MATH_MAX(Y - Radius, 0);
                    int32_t Y2 = MATH_MIN(Y + Radius + 1, Height);
                    int32_t *LineP1 = Integral + Y1 * ((Width + 1) << INDEX_TWO);
                    int32_t *LineP2 = Integral + Y2 * ((Width + 1) << INDEX_TWO);
                    unsigned char *LinePD = Dest + Y * Stride;
                    for (int X = 0; X < Width; X++)
                    {
                        int X1 = MATH_MAX(X - Radius, 0);
                        int X2 = MATH_MIN(X + Radius + 1, Width);
                        int Index1 = X1 << INDEX_TWO;
                        int Index2 = X2 << INDEX_TWO;
                        int SumB = LineP2[Index2 + INDEX_ZERO] - LineP1[Index2 + INDEX_ZERO] - LineP2[Index1 + INDEX_ZERO] + LineP1[Index1 + INDEX_ZERO];
                        int SumG = LineP2[Index2 + INDEX_ONE] - LineP1[Index2 + INDEX_ONE] - LineP2[Index1 + INDEX_ONE] + LineP1[Index1 + INDEX_ONE];
                        int SumR = LineP2[Index2 + INDEX_TWO] - LineP1[Index2 + INDEX_TWO] - LineP2[Index1 + INDEX_TWO] + LineP1[Index1 + INDEX_TWO];
                        //int SumA = LineP2[Index2 + 3] - LineP1[Index2 + 3] - LineP2[Index1 + 3] + LineP1[Index1 + 3];
                        int PixelCount = (X2 - X1) * (Y2 - Y1);
                        LinePD[INDEX_ZERO] = (SumB + (PixelCount >> INDEX_ONE)) / PixelCount;
                        LinePD[INDEX_ONE] = (SumG + (PixelCount >> INDEX_ONE)) / PixelCount;
                        LinePD[INDEX_TWO] = (SumR + (PixelCount >> INDEX_ONE)) / PixelCount;
                        //LinePD[3] = (SumA + (PixelCount >> 1)) / PixelCount;
                        uint8_t* alpha = Src + Y * Stride + (X << INDEX_TWO);
                        LinePD[INDEX_THREE] = alpha[INDEX_THREE];
                        LinePD += INDEX_FOUR;
                    }
                }
            }
            free(Integral);
        }
#endif
    };
} // namespace OHOS
#endif
