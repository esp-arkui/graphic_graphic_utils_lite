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
 * @file span_image_rgba.h
 * @brief Defines Scan line image filter
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_SPANCOLOR_FILL_IMAGE_RGBA_INCLUDED
#define GRAPHIC_SPANCOLOR_FILL_IMAGE_RGBA_INCLUDED

#include "gfx_utils/color.h"
#include "gfx_utils/graphics/common/graphic_common_basics.h"
#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_array.h"
#include "graphic_spancolor_fill_interpolator.h"
namespace OHOS {
    const double COORD_OFFSET = 0.5;
    const unsigned PIXEL_TIMES = 2;
    const unsigned LUMINANCE_SIZE = 4;

    enum ImageRgbaScale {
        IMAGE_RGBA_SHIFT = 14,
        IMAGE_RGBA_SCALE = 1 << IMAGE_RGBA_SHIFT,
        IMAGE_RGBA_MASK = IMAGE_RGBA_SCALE - 1
    };

    enum ImageSubpixelScale {
        IMAGE_SUBPIXEL_SHIFT = 8,
        IMAGE_SUBPIXEL_SCALE = 1 << IMAGE_SUBPIXEL_SHIFT,
        IMAGE_SUBPIXEL_MASK = IMAGE_SUBPIXEL_SCALE - 1
    };
    /**
     * @template class SpanImage
     * @brief Defines Image interpolator wrapper class
     * @template Class source represents the image source and
     * interpolator represents the image observation interpolator
     * @since 1.0
     * @version 1.0
     */
    template <class Source, class Interpolator>
    class SpanFillImage {
    public:
        using source_type = Source;
        using interpolator_type = Interpolator;

        SpanFillImage()
        {
        }
        /**
         * @brief Constructor parameters
         */
        SpanFillImage(source_type& src,
                      interpolator_type& interpolator)
            : src_(&src),
            interpolator_(&interpolator)
        {
        }

        /**
         * @brief return image_accessor
         */
        source_type& GetSource()
        {
            return *src_;
        }
        /**
         * @brief Return interpolator
         */
        interpolator_type& GetInterpolator()
        {
            return *interpolator_;
        }
        void Prepare()
        {
        }

    private:
        source_type* src_;
        interpolator_type* interpolator_;
    };

    /**
     * @template class SpanImage
     * @brief Defines Scan line image filter
     * @template Class source represents the image source and interpolator represents
     * the image observation interpolator
     * @since 1.0
     * @version 1.0
     */
    template <class Source, class Interpolator>
    class SpanImageRgba : public SpanFillImage<Source, Interpolator> {
    public:
        using source_type = Source;
        using color_type = typename source_type::color_type;
        using order_type = typename source_type::order_type;
        using interpolator_type = Interpolator;
        using spanImage = SpanFillImage<source_type, interpolator_type>;

        SpanImageRgba()
        {
        }
        SpanImageRgba(source_type& src, interpolator_type& interpolator)
            : spanImage(src, interpolator)
        {
        }
        /**
         * @brief Generate Generate corresponding image
         * @param span First address of scan line to be filled
         * @param x coordinate-x
         * @param y coordinate-y
         * @param len span length
         */
        void Generate(color_type* span, int x, int y, unsigned len)
        {
            spanImage::GetInterpolator().Begin(x + COORD_OFFSET, y + COORD_OFFSET, len);
            int32_t luminance[LUMINANCE_SIZE];
            const uint8_t* colorsPtr;
            do {
                int x_hr, y_hr;
                /**
                 * Get the new coordinate points added by the interpolator
                 */
                spanImage::GetInterpolator().Coordinates(&x_hr, &y_hr);
                x_hr -= IMAGE_SUBPIXEL_SCALE / PIXEL_TIMES;
                y_hr -= IMAGE_SUBPIXEL_SCALE / PIXEL_TIMES;
                int spanX = x_hr >> IMAGE_SUBPIXEL_SHIFT;
                int spanY = y_hr >> IMAGE_SUBPIXEL_SHIFT;
                unsigned weight;
                /**
                 * Setting the pixel value of the image color value r g b a
                 */
                luminance[INDEX_ZERO] = IMAGE_SUBPIXEL_SCALE * IMAGE_SUBPIXEL_SCALE / PIXEL_TIMES;
                luminance[INDEX_ONE] = IMAGE_SUBPIXEL_SCALE * IMAGE_SUBPIXEL_SCALE / PIXEL_TIMES;
                luminance[INDEX_TWO] = IMAGE_SUBPIXEL_SCALE * IMAGE_SUBPIXEL_SCALE / PIXEL_TIMES;
                luminance[INDEX_THREE] = IMAGE_SUBPIXEL_SCALE * IMAGE_SUBPIXEL_SCALE / PIXEL_TIMES;
                /**
                 * Obtain image pixel coordinates
                 */
                x_hr &= IMAGE_SUBPIXEL_MASK;
                y_hr &= IMAGE_SUBPIXEL_MASK;
                /**
                 * Gets the color pointer that needs interpolation
                 */
                colorsPtr = (const uint8_t*)spanImage::GetSource().Span(spanX, spanY, PIXEL_TIMES);
                weight = (IMAGE_SUBPIXEL_SCALE - x_hr) * (IMAGE_SUBPIXEL_SCALE - y_hr);
                luminance[INDEX_ZERO] += weight * *colorsPtr++;
                luminance[INDEX_ONE] += weight * *colorsPtr++;
                luminance[INDEX_TWO] += weight * *colorsPtr++;
                luminance[INDEX_THREE] += weight * *colorsPtr;
                // Get the color corresponding to the next x
                colorsPtr = (const uint8_t*)spanImage::GetSource().NextX();
                weight = x_hr * (IMAGE_SUBPIXEL_SCALE - y_hr);
                luminance[INDEX_ZERO] += weight * *colorsPtr++;
                luminance[INDEX_ONE] += weight * *colorsPtr++;
                luminance[INDEX_TWO] += weight * *colorsPtr++;
                luminance[INDEX_THREE] += weight * *colorsPtr;
                // Get the color corresponding to the next y
                colorsPtr = (const uint8_t*)spanImage::GetSource().Nexty();
                weight = (IMAGE_SUBPIXEL_SCALE - x_hr) * y_hr;
                luminance[INDEX_ZERO] += weight * *colorsPtr++;
                luminance[INDEX_ONE] += weight * *colorsPtr++;
                luminance[INDEX_TWO] += weight * *colorsPtr++;
                luminance[INDEX_THREE] += weight * *colorsPtr;
                // Get the color corresponding to the next x
                colorsPtr = (const uint8_t*)spanImage::GetSource().NextX();
                weight = x_hr * y_hr;
                luminance[INDEX_ZERO] += weight * *colorsPtr++;
                luminance[INDEX_ONE] += weight * *colorsPtr++;
                luminance[INDEX_TWO] += weight * *colorsPtr++;
                luminance[INDEX_THREE] += weight * *colorsPtr;
                span->redValue = uint8_t(
                    color_type::Downshift(luminance[order_type::RED], IMAGE_SUBPIXEL_SHIFT * PIXEL_TIMES));
                span->greenValue = uint8_t(
                    color_type::Downshift(luminance[order_type::GREEN], IMAGE_SUBPIXEL_SHIFT * PIXEL_TIMES));
                span->blueValue = uint8_t(
                    color_type::Downshift(luminance[order_type::BLUE], IMAGE_SUBPIXEL_SHIFT * PIXEL_TIMES));
                span->alphaValue = uint8_t(
                    color_type::Downshift(luminance[order_type::ALPHA], IMAGE_SUBPIXEL_SHIFT * PIXEL_TIMES));
                ++span;
                ++spanImage::GetInterpolator();
            } while (--len);
        }
    };
} // namespace OHOS

#endif
