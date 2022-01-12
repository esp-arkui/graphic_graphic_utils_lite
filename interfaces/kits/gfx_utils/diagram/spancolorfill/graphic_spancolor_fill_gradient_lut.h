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
 * @brief Defines Gradient mode
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GRADIENT_LUT_INCLUDED
#define GRAPHIC_GRADIENT_LUT_INCLUDED

#include "gfx_utils/diagram/vertexprimitive/graphic_geometry_dda_line.h"
#include "gfx_utils/diagram/vertexprimitive/graphic_geometry_plaindata_array.h"
#include "gfx_utils/diagram/vertexprimitive/graphic_geometry_plaindata_blockvector.h"
#include "gfx_utils/diagram/vertexprimitive/graphic_geometry_range_adapter.h"

namespace OHOS {
const unsigned COLOR_PROFILE_SIZE = 4;
/**
* @brief According to remove_all,add_color,and build_lut,
* build the color gradient process, start, end and middle gradient colors
* @param The template parameters are colorinterpolator color interpolator and colorlutsize color cell size
* @since 1.0
* @version 1.0
*/
template <class ColorInterpolator>
class GraphicSpancolorFillGradientLut {
#if GRAPHIC_ENABLE_GRADIENT_FILL_FLAG
public:
    using InterpolatorType = ColorInterpolator;
    using ColorType = typename InterpolatorType::ColorType;
    GraphicSpancolorFillGradientLut() {}

    /**
     * @brief Remove all colors
     * @since 1.0
     * @version 1.0
     */
    void RemoveAll()
    {
        colorProfile_.RemoveAll();
    }

    /**
     * @brief Add the color and position you want to gradient during the gradient process
     * @param offset (0-1)
     * @param color Added color
     * @since 1.0
     * @version 1.0
     */
    void AddColor(float offset, const ColorType& color)
    {
        colorProfile_.Add(ColorPoint(offset, color));
    }

    /**
     * @brief Building a color_typ array from gradient colors
     * Array length 0-255
     * The contents of the array are distributed on the array according to the gradient color
     * @since 1.0
     * @version 1.0
     */
    void BuildLut()
    {
        /**
         * Quick sort for gradient color arrays
         */
        QuickSort(colorProfile_, OffsetLess);
    }

    ColorType getColor(float proportion)
    {
        if (proportion >= 1) {
            ColorPoint  end = colorProfile_.Last();
            return end.color;
        }

        if (proportion <= 0) {
            ColorPoint  start = colorProfile_[0];
            return start.color;
        }

        ColorPoint start;
        ColorPoint end;
        for (int i = 0; i < colorProfile_.Size(); i++) {
            if (colorProfile_[i].offset > proportion) {
                end = colorProfile_[i];
                start = colorProfile_[i-1];
                break;
            }
        }
        float growth = (proportion-start.offset)/(end.offset-start.offset);
        ColorType color;
        color.redValue = start.color.redValue + (end.color.redValue-start.color.redValue)*growth;
        color.greenValue = start.color.greenValue + (end.color.greenValue-start.color.greenValue)*growth;
        color.blueValue = start.color.blueValue + (end.color.blueValue-start.color.blueValue)*growth;
        color.alphaValue = start.color.alphaValue + (end.color.alphaValue-start.color.alphaValue)*growth;
        return color;
    }

    /**
     * @brief size Returns the size of the colorprofile
     */
    unsigned size()
    {
        return colorProfile_.Size();
    }

    /**
     * @brief Override [] operator
     */
    const ColorType& operator[](unsigned i) const
    {
        return colorProfile_[i];
    }
private:
    struct ColorPoint {
        float offset;
        ColorType color;

        ColorPoint() {}
        /**
         * @brief Input parameter
         * @param offsetValue (0-1)
         * @param color_ Added color
         */
        ColorPoint(float offsetValue, const ColorType& colorValue)
            : offset(offsetValue), color(colorValue)
        {
            if (offset < 0.0) {
                offset = 0.0;
            }
            if (offset > 1.0) {
                offset = 1.0;
            }
        }
    };

    /**
     * @brief OffsetLess Returns the comparison result that the offset of a is smaller than that of B
     * @param colorPoint1 struct ColorPoint struct
     * @param colorPoint2 struct ColorPoint struct
     * @return true: colorpoint1 precedes colorpoint2 false colorpoint1 follows colorpoint2
     */
    static bool OffsetLess(const ColorPoint& colorPoint1, const ColorPoint& colorPoint2)
    {
        return colorPoint1.offset < colorPoint2.offset;
    }

    /**
     * @brief OffsetEqual Returns a comparison result in
     * which the offset of comparison a is equal to that of comparison b
     * @param colorPoint1 struct ColorPoint struct
     * @param colorPoint2 struct ColorPoint struct
     * @return Return true: the offsets of colorpoint1 and colorpoint2 are equal,
     * and false: the offsets of a and B are not equal
     */
    static bool OffsetEqual(const ColorPoint& colorPoint1, const ColorPoint& colorPoint2)
    {
        return colorPoint1.offset == colorPoint2.offset;
    }
    using ColorProfileType = OHOS::GraphicGeometryPlainDataBlockVector<ColorPoint, COLOR_PROFILE_SIZE>;
    ColorProfileType colorProfile_;
#endif
};
} // namespace OHOS

#endif
