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

/**
* @file gradient_lut.h
* @brief Defines 渐变模式
* @since 1.0
* @version 1.0
*/

#ifndef GRAPHIC_GRADIENT_LUT_INCLUDED
#define GRAPHIC_GRADIENT_LUT_INCLUDED

#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_dda_line.h"
#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_plaindata_array.h"
#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_plaindata_blockvector.h"

namespace OHOS {
    const unsigned COLOR_PROFILE_SIZE = 4;
    /**
    * @根据remove_all,add_color,build_lut构建颜色的渐变过程，起止和中间的渐变颜色
    * @模板参数是ColorInterpolator 颜色插值器，ColorLutSize 颜色单元大小
    * @since 1.0
    * @version 1.0
    */
    template <class ColorInterpolator, unsigned ColorLutSize = MAX_COLOR_SIZE>
    class GradientColorCalibration {
#if GRAPHIC_GEOMETYR_ENABLE_GRADIENT_FILLSTROKECOLOR
    public:
        typedef ColorInterpolator interpolator_type;
        typedef typename interpolator_type::color_type color_type;
        enum {
            colorLutSize_ = ColorLutSize
        };
        GradientColorCalibration() : colorType(colorLutSize_)
        {
        }

        /**
        * @brief 删除所有颜色
        * @since 1.0
        * @version 1.0
        */
        void RemoveAll()
        {
            colorProfile.RemoveAll();
        }

        /**
         * @brief 在渐变过程中添加需要渐变的颜色和位置
         * @param offset (0-1)
         * @param color 添加的颜色
         * @since 1.0
         * @version 1.0
         */
        void AddColor(double offset, const color_type& color)
        {
            colorProfile.Add(ColorPoint(offset, color));
        }

        /**
        * @brief 根据渐变颜色构建color_type数组
        * @数组长度0-255
        * @数组内容根据渐变颜色分布在数组上
        * @since 1.0
        * @version 1.0
        */
        void BuildLut()
        {
            /*
             * 对于渐变颜色数组记性快速排序
             */
            QuickSort(colorProfile, OffsetLess);
            colorProfile.CutAt(RemoveDuplicates(colorProfile, OffsetEqual));
            if (colorProfile.Size() > 1) {
                unsigned index;
                unsigned start = MATH_UROUND(colorProfile[0].offset * colorLutSize_);
                unsigned end;
                color_type color = colorProfile[0].color;

                /*
                 * 对于colorProfile[0]赋予初始颜色计算.
                 */
                for (index = 0; index < start; index++) {
                    colorType[index] = color;
                }
                /*
                 * 从1到colorProfile.size() 间进行插值颜色计算.
                 */
                for (index = 1; index < colorProfile.Size(); index++) {
                    end = MATH_UROUND(colorProfile[index].offset * colorLutSize_);
                    interpolator_type ci(colorProfile[index - 1].color,
                                         colorProfile[index].color,
                                         end - start + 1);
                    while (start < end) {
                        colorType[start] = ci.GetColor();
                        ++ci;
                        ++start;
                    }
                }
                color = colorProfile.Last().color;
                /*
                 * 对于colorProfile last 赋予end颜色..
                 */
                for (; end < colorType.Size(); end++) {
                    color = colorProfile.Last().color;
                    colorType[end] = color;
                }
            }
        }

        /**
         * @brief size 返回color_lut_type的size
         */
        static unsigned size()
        {
            return colorLutSize_;
        }

        /**
         * @brief 重写[]运算符
         */
        const color_type& operator[](unsigned i) const
        {
            return colorType[i];
        }

    private:
        struct ColorPoint {
            double offset;
            color_type color;

            ColorPoint()
            {}
            /**
             * @brief 入参
             * @param offset_ (0-1)
             * @param color_ 添加的颜色
             */
            ColorPoint(double offset_, const color_type& color_)
                : offset(offset_), color(color_)
            {
                if (offset < 0.0)
                    offset = 0.0;
                if (offset > 1.0)
                    offset = 1.0;
            }
        };

        /**
         * @brief OffsetLess 返回比较a的offset比b的offset小的比较结果
         * @param colorPoint1 struct ColorPoint结构体
         * @param colorPoint2 struct ColorPoint结构体
         * @return true:colorPoint1在colorPoint2的前面 false colorPoint1在colorPoint2的后面
         */
        static bool OffsetLess(const ColorPoint& colorPoint1, const ColorPoint& colorPoint2)
        {
            return colorPoint1.offset < colorPoint2.offset;
        }

        /**
         * @brief OffsetEqual 返回比较a的offset比b的offset相等的比较结果
         * @param colorPoint1 struct ColorPoint结构体
         * @param colorPoint2 struct ColorPoint结构体
         * @return 返回 true：colorPoint1,colorPoint2的offset相等, false：a,b的offset不相等
         */
        static bool OffsetEqual(const ColorPoint& colorPoint1, const ColorPoint& colorPoint2)
        {
            return colorPoint1.offset == colorPoint2.offset;
        }

        using colorProfileType = OHOS::GeometryPlainDataBlockVector<ColorPoint, COLOR_PROFILE_SIZE>;
        colorProfileType colorProfile;

        using colorLutType = OHOS::GeometryPlainDataArray<color_type>;
        colorLutType colorType;
#endif
    };
} // namespace OHOS

#endif
