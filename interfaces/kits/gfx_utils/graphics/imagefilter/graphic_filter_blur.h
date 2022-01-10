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
class FastBoxBlur{
public:

    void static GetGrayIntegralImage(unsigned char *Src, int *Integral, int Width, int Height, int Stride)
    {
        //	你所看到的每一句代码都是作者辛勤劳作和多年经验的积累，希望你能尊重作者的成果
        //	你的每一个  评论  和  打赏  都是作者撰写更多博文和分享经验的鼓励。
        //	本代码对应博文见：http://www.cnblogs.com/Imageshop/p/6219990.html

        memset(Integral, 0, (Width + 1) * sizeof(int));					//	第一行都为0
        for (int Y = 0; Y < Height; Y++)
        {
            unsigned char *LinePS = Src + Y * Stride;
            int *LinePL = Integral + Y * (Width + 1) + 1;				//	上一行位置
            int *LinePD = Integral + (Y + 1) * (Width + 1) + 1;			//	当前位置，注意每行的第一列的值都为0
            LinePD[-1] = 0;												//	第一列的值为0
            for (int X = 0, Sum = 0; X < Width; X++)
            {
                Sum += LinePS[X];										//	行方向累加
                LinePD[X] = LinePL[X] + Sum;							//	更新积分图
            }
        }
    }

    void static GetRGBIntegralImage(unsigned char *Src, int *Integral, int Width, int Height, int Stride)
    {
        //	你所看到的每一句代码都是作者辛勤劳作和多年经验的积累，希望你能尊重作者的成果
        //	你的每一个  评论  和  打赏  都是作者撰写更多博文和分享经验的鼓励。
        //	本代码对应博文见：http://www.cnblogs.com/Imageshop/p/6219990.html

//        const int Channel = 3;
//        memset(Integral, 0, (Width + 1) * Channel * sizeof(int));
//        for (int Y = 0; Y < Height; Y++)
//        {
//            unsigned char *LinePS = Src + Y * Stride;
//            int *LinePL = Integral + Y * (Width + 1) * Channel + Channel;
//            int *LinePD = Integral + (Y + 1) * (Width + 1) * Channel + Channel;
//            LinePD[-3] = 0; LinePD[-2] = 0; LinePD[-1] = 0;
//            for (int X = 0, SumB = 0, SumG = 0, SumR = 0; X < Width; X++)
//            {
//                SumB += LinePS[0];
//                SumG += LinePS[1];
//                SumR += LinePS[2];
//                LinePD[0] = LinePL[0] + SumB;
//                LinePD[1] = LinePL[1] + SumG;
//                LinePD[2] = LinePL[2] + SumR;
//                LinePS += Channel;
//                LinePL += Channel;
//                LinePD += Channel;
//            }
//        }
        int Channel = Stride / Width;
        memset(Integral, 0, (Width + 1) * Channel * sizeof(int));
        for (int Y = 0; Y < Height; Y++)
        {
            unsigned char *LinePS = Src + Y * Stride;
            int *LinePL = Integral + Y * (Width + 1) * Channel + Channel;
            int *LinePD = Integral + (Y + 1) * (Width + 1) * Channel + Channel;
            LinePD[-3] = 0; LinePD[-2] = 0; LinePD[-1] = 0;
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



    void static BoxBlur(unsigned char *Src, unsigned char *Dest, int16_t Width, int16_t Height, int16_t Stride, int16_t Radius)
    {
        //	你所看到的每一句代码都是作者辛勤劳作和多年经验的积累，希望你能尊重作者的成果
        //	你的每一个  评论  和  打赏  都是作者撰写更多博文和分享经验的鼓励。
        //	本代码对应博文见：http://www.cnblogs.com/Imageshop/p/6219990.html

        int32_t Channel = Stride / Width;
        int32_t *Integral = (int32_t *)malloc((Width + 1) * (Height + 1) * Channel * sizeof(int32_t));
        if (Channel == 1)
        {
            GetGrayIntegralImage(Src, Integral, Width, Height, Stride);
            #pragma omp parallel for
            for (int Y = 0; Y < Height; Y++)
            {
                int Y1 = MATH_MAX(Y - Radius, 0);
                int Y2 = MATH_MIN(Y + Radius + 1, Height);
                //	int Y1 = Y - Radius;
                //	int Y2 = Y + Radius + 1;
                //	if (Y1 < 0) Y1 = 0;
                //	if (Y2 > Height) Y2 = Height;
                int32_t *LineP1 = Integral + Y1 * (Width + 1);
                int32_t *LineP2 = Integral + Y2 * (Width + 1);
                unsigned char *LinePD = Dest + Y * Stride;
                for (int X = 0; X < Width; X++)
                {
                    int X1 = MATH_MAX(X - Radius, 0);
                    int X2 = MATH_MIN(X + Radius + 1, Width);
                    //	int X1 = X - Radius;
                    //	if (X1 < 0) X1 = 0;
                    //	int X2 = X + Radius + 1;
                    //	if (X2 > Width) X2 = Width;
                    int32_t Sum = LineP2[X2] - LineP1[X2] - LineP2[X1] + LineP1[X1];
                    int32_t PixelCount = (X2 - X1) * (Y2 - Y1);					//	有效的像素数
                    LinePD[X] = (Sum + (PixelCount >> 1)) / PixelCount;		//	四舍五入
                }
            }
        }
        else if (Channel == 3)
        {
            GetRGBIntegralImage(Src, Integral, Width, Height, Stride);
            #pragma omp parallel for
            for (int32_t Y = 0; Y < Height; Y++)
            {
                int32_t Y1 = MATH_MAX(Y - Radius, 0);
                int32_t Y2 = MATH_MIN(Y + Radius + 1, Height);
                int32_t *LineP1 = Integral + Y1 * (Width + 1) * 3;
                int32_t *LineP2 = Integral + Y2 * (Width + 1) * 3;
                unsigned char *LinePD = Dest + Y * Stride;
                for (int32_t X = 0; X < Width; X++)
                {
                    int32_t X1 = MATH_MAX(X - Radius, 0);
                    int32_t X2 = MATH_MIN(X + Radius + 1, Width);
                    int32_t Index1 = X1 * 3;
                    int32_t Index2 = X2 * 3;
                    int32_t SumB = LineP2[Index2 + 0] - LineP1[Index2 + 0] - LineP2[Index1 + 0] + LineP1[Index1 + 0];
                    int32_t SumG = LineP2[Index2 + 1] - LineP1[Index2 + 1] - LineP2[Index1 + 1] + LineP1[Index1 + 1];
                    int32_t SumR = LineP2[Index2 + 2] - LineP1[Index2 + 2] - LineP2[Index1 + 2] + LineP1[Index1 + 2];
                    int32_t PixelCount = (X2 - X1) * (Y2 - Y1);
                    LinePD[0] = (SumB + (PixelCount >> 1)) / PixelCount;
                    LinePD[1] = (SumG + (PixelCount >> 1)) / PixelCount;
                    LinePD[2] = (SumR + (PixelCount >> 1)) / PixelCount;
                    LinePD += 3;
                }
            }
        }
        else if (Channel == 4)
        {
            GetRGBIntegralImage(Src, Integral, Width, Height, Stride);
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
                    int SumA = LineP2[Index2 + 3] - LineP1[Index2 + 3] - LineP2[Index1 + 3] + LineP1[Index1 + 3];
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
};



#if GRAPHIC_GEOMETYR_ENABLE_BLUR_EFFECT_VERTEX_SOURCE
    template <class T>
    struct StackBlurTables {
        static uint16_t const stackBlur8Multiplication[OHOS::MAX_COLOR_NUM];
        static uint8_t const stackBlur8Shear[OHOS::MAX_COLOR_NUM];
    };

    template <class T>
    uint16_t const StackBlurTables<T>::stackBlur8Multiplication[OHOS::MAX_COLOR_NUM] = {
        512, 512, 456, 512, 328, 456, 335, 512, 405, 328, 271, 456, 388, 335, 292, 512,
        454, 405, 364, 328, 298, 271, 496, 456, 420, 388, 360, 335, 312, 292, 273, 512,
        482, 454, 428, 405, 383, 364, 345, 328, 312, 298, 284, 271, 259, 496, 475, 456,
        437, 420, 404, 388, 374, 360, 347, 335, 323, 312, 302, 292, 282, 273, 265, 512,
        497, 482, 468, 454, 441, 428, 417, 405, 394, 383, 373, 364, 354, 345, 337, 328,
        320, 312, 305, 298, 291, 284, 278, 271, 265, 259, 507, 496, 485, 475, 465, 456,
        446, 437, 428, 420, 412, 404, 396, 388, 381, 374, 367, 360, 354, 347, 341, 335,
        329, 323, 318, 312, 307, 302, 297, 292, 287, 282, 278, 273, 269, 265, 261, 512,
        505, 497, 489, 482, 475, 468, 461, 454, 447, 441, 435, 428, 422, 417, 411, 405,
        399, 394, 389, 383, 378, 373, 368, 364, 359, 354, 350, 345, 341, 337, 332, 328,
        324, 320, 316, 312, 309, 305, 301, 298, 294, 291, 287, 284, 281, 278, 274, 271,
        268, 265, 262, 259, 257, 507, 501, 496, 491, 485, 480, 475, 470, 465, 460, 456,
        451, 446, 442, 437, 433, 428, 424, 420, 416, 412, 408, 404, 400, 396, 392, 388,
        385, 381, 377, 374, 370, 367, 363, 360, 357, 354, 350, 347, 344, 341, 338, 335,
        332, 329, 326, 323, 320, 318, 315, 312, 310, 307, 304, 302, 299, 297, 294, 292,
        289, 287, 285, 282, 280, 278, 275, 273, 271, 269, 267, 265, 263, 261, 259};

    template <class T>
    uint8_t const StackBlurTables<T>::stackBlur8Shear[OHOS::MAX_COLOR_NUM] = {
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
        24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24};
#endif
    template <class ColorType, class CalculatorType>
    class StackBlur {
#if GRAPHIC_GEOMETYR_ENABLE_BLUR_EFFECT_VERTEX_SOURCE
    public:
        /**
         * @brief x轴方向进行模糊
         * @since 1.0
         * @version 1.0
         */
        template <class Img>
        void BlurX(Img& img, unsigned radius)
        {
            if (radius < 1) {
                return;
            }
            unsigned imagX, imageY, xRadiusPixel, pixelIndex, stackPtr, stackStart;
            ColorType pixelColor;
            ColorType* stackPixel;
            CalculatorType sumPixel, sumPixelIn, sumPixelOut;
            unsigned imgWidth = img.Width();
            unsigned imgHeight = img.Height();
            unsigned widthManager = imgWidth - 1;
            unsigned div = radius * 2 + 1;
            unsigned divSum = (radius + 1) * (radius + 1);
            unsigned multipleSum = 0;
            unsigned shearSum = 0;
            unsigned maxVal = ColorType::BASEMASK;
            if (maxVal <= OHOS::MAX_COLOR_NUM && radius < OHOS::MAX_COLOR_NUM) {
                multipleSum = StackBlurTables<int>::stackBlur8Multiplication[radius];
                shearSum = StackBlurTables<int>::stackBlur8Shear[radius];
            }
            imageBufferVector.Allocate(imgWidth, OHOS::HALF_COLOR_NUM);
            stackBufferVector.Allocate(div, OHOS::EIGHTH_COLOR_NUM);
            for (imageY = 0; imageY < imgHeight; imageY++) {
                sumPixel.Clear();
                sumPixelIn.Clear();
                sumPixelOut.Clear();
                pixelColor = img.Pixel(0, imageY);
                for (pixelIndex = 0; pixelIndex <= radius; pixelIndex++) {
                    stackBufferVector[pixelIndex] = pixelColor;
                    sumPixel.Add(pixelColor, pixelIndex + 1);
                    sumPixelOut.Add(pixelColor);
                }
                for (pixelIndex = 1; pixelIndex <= radius; pixelIndex++) {
                    if (pixelIndex > widthManager) {
                        pixelColor = img.Pixel(widthManager, imageY);
                    } else {
                        pixelColor = img.Pixel(pixelIndex, imageY);
                    }
                    stackBufferVector[pixelIndex + radius] = pixelColor;
                    sumPixel.Add(pixelColor, radius + 1 - pixelIndex);
                    sumPixelIn.Add(pixelColor);
                }
                stackPtr = radius;
                for (imagX = 0; imagX < imgWidth; imagX++) {
                    if (multipleSum) {
                        sumPixel.CalculatePixel(imageBufferVector[imagX], multipleSum, shearSum);
                    } else {
                        sumPixel.CalculatePixel(imageBufferVector[imagX], divSum);
                    }
                    sumPixel.Sub(sumPixelOut);
                    stackStart = stackPtr + div - radius;
                    if (stackStart >= div) {
                        stackStart -= div;
                    }
                    stackPixel = &stackBufferVector[stackStart];
                    sumPixelOut.Sub(*stackPixel);
                    xRadiusPixel = imagX + radius + 1;
                    if (xRadiusPixel > widthManager) {
                        xRadiusPixel = widthManager;
                    }
                    pixelColor = img.Pixel(xRadiusPixel, imageY);
                    *stackPixel = pixelColor;
                    sumPixelIn.Add(pixelColor);
                    sumPixel.Add(sumPixelIn);
                    ++stackPtr;
                    if (stackPtr >= div) {
                        stackPtr = 0;
                    }
                    stackPixel = &stackBufferVector[stackPtr];
                    sumPixelOut.Add(*stackPixel);
                    sumPixelIn.Sub(*stackPixel);
                }
                img.CopyColorHspan(0, imageY, imgWidth, &imageBufferVector[0]);
            }
        }
        /**
         * @brief Y轴方向进行模糊
         * @since 1.0
         * @version 1.0
         */
        template <class Img>
        void BlurY(Img& img, unsigned radius)
        {
            PixfmtTransposer<Img> img2(img);
            BlurX(img2, radius);
        }
        /**
         * @brief 模糊
         * @since 1.0
         * @version 1.0
         */
        template <class Img>
        void Blur(Img& img, unsigned radius)
        {
            BlurX(img, radius);
            PixfmtTransposer<Img> img2(img);
            BlurX(img2, radius);
        }

    private:
        GeometryPlainDataVector<ColorType> imageBufferVector;
        GeometryPlainDataVector<ColorType> stackBufferVector;
    };

    template <class ValueType = unsigned>
    struct StackBlurCalcRGBA {
        ValueType redValue, greenValue, blueValue, alphaValue;
        /**
         * @brief 清空颜色值
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void Clear()
        {
            redValue = greenValue = blueValue = alphaValue = 0;
        }
        /**
         * @brief 颜色值相加
         * @param vColor 像素的颜色值
         * @since 1.0
         * @version 1.0
         */
        template <class ArgT>
        GRAPHIC_GEOMETRY_INLINE void Add(const ArgT& vColor)
        {
            redValue += vColor.redValue;
            greenValue += vColor.greenValue;
            blueValue += vColor.blueValue;
            alphaValue += vColor.alphaValue;
        }
        /**
         * @brief 像素颜色值加权添加
         * @param vColor 像素的颜色值
         * @param kWeight 权值
         * @since 1.0
         * @version 1.0
         */
        template <class ArgT>
        GRAPHIC_GEOMETRY_INLINE void Add(const ArgT& vColor, unsigned kWeight)
        {
            redValue += vColor.redValue * kWeight;
            greenValue += vColor.greenValue * kWeight;
            blueValue += vColor.blueValue * kWeight;
            alphaValue += vColor.alphaValue * kWeight;
        }
        /**
         * @brief 减去像素的颜色值
         * @param vColor 像素的颜色值
         * @since 1.0
         * @version 1.0
         */
        template <class ArgT>
        GRAPHIC_GEOMETRY_INLINE void Sub(const ArgT& vColor)
        {
            redValue -= vColor.redValue;
            greenValue -= vColor.greenValue;
            blueValue -= vColor.blueValue;
            alphaValue -= vColor.alphaValue;
        }
        /**
         * @brief 颜色值求平均数
         * @param vColor 像素颜色值
         * @param div 除数
         * @since 1.0
         * @version 1.0
         */
        template <class ArgT>
        GRAPHIC_GEOMETRY_INLINE void CalculatePixel(ArgT& vColor, unsigned div)
        {
            vColor.redValue = ValueType(redValue / div);
            vColor.greenValue = ValueType(greenValue / div);
            vColor.blueValue = ValueType(blueValue / div);
            vColor.alphaValue = ValueType(alphaValue / div);
        }
        /**
         * @brief 像素颜色值加权平均
         * @param vColor 像素颜色值
         * @param mul 权值
         * @param shr 右移位数
         * @since 1.0
         * @version 1.0
         */
        template <class ArgT>
        GRAPHIC_GEOMETRY_INLINE void CalculatePixel(ArgT& vColor, unsigned mul, unsigned shr)
        {
            vColor.redValue = ValueType((redValue * mul) >> shr);
            vColor.greenValue = ValueType((greenValue * mul) >> shr);
            vColor.blueValue = ValueType((blueValue * mul) >> shr);
            vColor.alphaValue = ValueType((alphaValue * mul) >> shr);
        }
#endif
    };
} // namespace OHOS

#endif
