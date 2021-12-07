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


#include <cstring>
#include <cmath>
#include "gfx_utils/graphics/graphic_geometry/agg_array.h"
#include "render/agg_pixfmt_base.h"
#include "render/agg_pixfmt_transposer.h"
#include "render/agg_renderer_base.h"

namespace OHOS
{

    template<class T> struct StackBlurTables
    {
        static int16u const stackBlur8Mul[255];
        static int8u  const stackBlur8Shr[255];
    };

    //------------------------------------------------------------------------
    template<class T> 
    int16u const StackBlurTables<T>::stackBlur8Mul[255] = 
    {
        512,512,456,512,328,456,335,512,405,328,271,456,388,335,292,512,
        454,405,364,328,298,271,496,456,420,388,360,335,312,292,273,512,
        482,454,428,405,383,364,345,328,312,298,284,271,259,496,475,456,
        437,420,404,388,374,360,347,335,323,312,302,292,282,273,265,512,
        497,482,468,454,441,428,417,405,394,383,373,364,354,345,337,328,
        320,312,305,298,291,284,278,271,265,259,507,496,485,475,465,456,
        446,437,428,420,412,404,396,388,381,374,367,360,354,347,341,335,
        329,323,318,312,307,302,297,292,287,282,278,273,269,265,261,512,
        505,497,489,482,475,468,461,454,447,441,435,428,422,417,411,405,
        399,394,389,383,378,373,368,364,359,354,350,345,341,337,332,328,
        324,320,316,312,309,305,301,298,294,291,287,284,281,278,274,271,
        268,265,262,259,257,507,501,496,491,485,480,475,470,465,460,456,
        451,446,442,437,433,428,424,420,416,412,408,404,400,396,392,388,
        385,381,377,374,370,367,363,360,357,354,350,347,344,341,338,335,
        332,329,326,323,320,318,315,312,310,307,304,302,299,297,294,292,
        289,287,285,282,280,278,275,273,271,269,267,265,263,261,259
    };

    //------------------------------------------------------------------------
    template<class T> 
    int8u const StackBlurTables<T>::stackBlur8Shr[255] = 
    {
          9, 11, 12, 13, 13, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 17, 
         17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 19, 
         19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20,
         20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 21,
         21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
         21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 
         22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
         22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 
         23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
         23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
         23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 
         23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
         24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
         24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
         24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
         24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24
    };



    //==============================================================StackBlur
    template<class ColorType, class CalculatorType> class StackBlur
    {
    public:
        //--------------------------------------------------------------------
        template<class Img> void BlurX(Img& img, unsigned radius)
        {
            if(radius < 1) {
                return;
            }

            unsigned x, y, xp, i;
            unsigned stack_ptr;
            unsigned stackStart;

            ColorType      pix;
            ColorType*     stackPixel;
            CalculatorType sum;
            CalculatorType sumIn;
            CalculatorType sumOut;

            unsigned w   = img.width();
            unsigned h   = img.height();
            unsigned wm  = w - 1;
            unsigned div = radius * 2 + 1;

            unsigned divSum = (radius + 1) * (radius + 1);
            unsigned mulSum = 0;
            unsigned shrSum = 0;
            unsigned maxVal = ColorType::base_mask;

            if(maxVal <= 255 && radius < 255)
            {
                mulSum = StackBlurTables<int>::stackBlur8Mul[radius];
                shrSum = StackBlurTables<int>::stackBlur8Shr[radius];
            }

            buffer_.allocate(w, 128);
            stack_.allocate(div, 32);

            for(y = 0; y < h; y++){
                sum.Clear();
                sumIn.Clear();
                sumOut.Clear();

                pix = img.pixel(0, y);
                for(i = 0; i <= radius; i++){
                    stack_[i] = pix;
                    sum.Add(pix, i + 1);
                    sumOut.Add(pix);
                }
                for(i = 1; i <= radius; i++){
                    if(i>wm){
                        pix = img.pixel(wm, y);
                    }else{
                        pix = img.pixel(i, y);
                    }
                    stack_[i + radius] = pix;
                    sum.Add(pix, radius + 1 - i);
                    sumIn.Add(pix);
                }

                stack_ptr = radius;
                for(x = 0; x < w; x++){
                    if(mulSum) {
                        sum.CalculatePixel(buffer_[x], mulSum, shrSum);
                    }else{        
                        sum.CalculatePixel(buffer_[x], divSum);
                    }

                    sum.Sub(sumOut);
           
                    stackStart = stack_ptr + div - radius;
                    if(stackStart >= div) {
                        stackStart -= div;
                    }
                    stackPixel = &stack_[stackStart];

                    sumOut.Sub(*stackPixel);

                    xp = x + radius + 1;
                    if(xp > wm) {
                        xp = wm;
                    }
                    pix = img.pixel(xp, y);
            
                    *stackPixel = pix;
            
                    sumIn.Add(pix);
                    sum.Add(sumIn);
            
                    ++stack_ptr;
                    if(stack_ptr >= div) {
                        stack_ptr = 0;
                    }
                    stackPixel = &stack_[stack_ptr];

                    sumOut.Add(*stackPixel);
                    sumIn.Sub(*stackPixel);
                }
                img.copy_color_hspan(0, y, w, &buffer_[0]);
            }
        }

        //--------------------------------------------------------------------
        template<class Img> void BlurY(Img& img, unsigned radius)
        {
            pixfmt_transposer<Img> img2(img);
            BlurX(img2, radius);
        }

        //--------------------------------------------------------------------
        template<class Img> void Blur(Img& img, unsigned radius)
        {
            BlurX(img, radius);
            pixfmt_transposer<Img> img2(img);
            BlurX(img2, radius);
        }

    private:
        pod_vector<ColorType> buffer_;
        pod_vector<ColorType> stack_;
    };

    //====================================================StackBlurCalcRGBA
    template<class ValueType=unsigned> struct StackBlurCalcRGBA
    {
        ValueType r,g,b,a;

        GRAPTHIC_INLINE void Clear() 
        { 
            r = g = b = a = 0; 
        }

        template<class ArgT> GRAPTHIC_INLINE void Add(const ArgT& v)
        {
            r += v.r;
            g += v.g;
            b += v.b;
            a += v.a;
        }

        template<class ArgT> GRAPTHIC_INLINE void Add(const ArgT& v, unsigned k)
        {
            r += v.r * k;
            g += v.g * k;
            b += v.b * k;
            a += v.a * k;
        }

        template<class ArgT> GRAPTHIC_INLINE void Sub(const ArgT& v)
        {
            r -= v.r;
            g -= v.g;
            b -= v.b;
            a -= v.a;
        }

        template<class ArgT> GRAPTHIC_INLINE void CalculatePixel(ArgT& v, unsigned div)
        {
            v.r = ValueType(r / div);
            v.g = ValueType(g / div);
            v.b = ValueType(b / div);
            v.a = ValueType(a / div);
        }

        template<class ArgT> 
        GRAPTHIC_INLINE void CalculatePixel(ArgT& v, unsigned mul, unsigned shr)
        {
            v.r = ValueType((r * mul) >> shr);
            v.g = ValueType((g * mul) >> shr);
            v.b = ValueType((b * mul) >> shr);
            v.a = ValueType((a * mul) >> shr);
        }
    };
}




#endif
