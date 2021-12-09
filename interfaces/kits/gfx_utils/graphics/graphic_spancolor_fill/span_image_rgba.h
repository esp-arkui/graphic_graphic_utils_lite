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
* @brief Defines 扫描线图像过滤器
* @since 1.0
* @version 1.0
*/

#ifndef GRAPHIC_SPAN_image_rgba_RGBA_INCLUDED
#define GRAPHIC_SPAN_image_rgba_RGBA_INCLUDED

#include "gfx_utils/graphics/graphic_color/agg_color_rgba.h"
#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "gfx_utils/graphics/graphic_geometry/agg_array.h"
#include "span_interpolator.h"
namespace OHOS {
    enum ImageRgbaScale
    {
        IMAGE_RGBA_SHIFT = 14,
        IMAGE_RGBA_SCALE = 1 << IMAGE_RGBA_SHIFT,
        IMAGE_RGBA_MASK = IMAGE_RGBA_SCALE - 1
    };

    enum ImageSubpixelScale
    {
        IMAGE_SUBPIXEL_SHIFT = 8,
        IMAGE_SUBPIXEL_SCALE = 1 << IMAGE_SUBPIXEL_SHIFT,
        IMAGE_SUBPIXEL_MASK = IMAGE_SUBPIXEL_SCALE - 1
    };

    template <class Source, class Interpolator>
    class SpanImage {
    public:
        typedef Source source_type;
        typedef Interpolator interpolator_type;

        SpanImage()
        {
        }
        /**
         * @brief 构造函数传参
         */
        SpanImage(source_type& src,
                  interpolator_type& interpolator) :
            src_(&src),
            interpolator_(&interpolator)
        {
        }

        /**
         * @brief 返回 image_accessor
         */
        source_type& GetSource()
        {
            return *src_;
        }
        /**
         * @brief 返回插值器
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
     *生成相应image
     */
    template <class Source, class Interpolator>
    class spanImageRgba : public SpanImage<Source, Interpolator> {
    public:
        using source_type = Source;
        using color_type = typename source_type::color_type;
        using order_type = typename source_type::order_type;
        using interpolator_type = Interpolator;
        using spanImage = SpanImage<source_type, interpolator_type>;
        using value_type = typename color_type::value_type;
        using calc_type = typename color_type::calc_type;
        using long_type = typename color_type::long_type;

        spanImageRgba()
        {
        }
        spanImageRgba(source_type& src, interpolator_type& interpolator) :
            spanImage(src, interpolator)
        {
        }

        /**
         * @brief Generate 生成相应image
         * @param span 需要填色的扫描线首地址
         * @param x 坐标-x
         * @param y 坐标-y
         * @param len 扫描线长度
         */
        void Generate(color_type* span, int x, int y, unsigned len)
        {
            spanImage::GetInterpolator().begin(x + 0.5, y + 0.5, len);

            long_type luminance[4];
            const value_type* colorsPtr;

            do {
                int x_hr;
                int y_hr;

                spanImage::GetInterpolator().coordinates(&x_hr, &y_hr);

                x_hr -= IMAGE_SUBPIXEL_SCALE / 2;
                y_hr -= IMAGE_SUBPIXEL_SCALE / 2;

                int spanX = x_hr >> IMAGE_SUBPIXEL_SHIFT;
                int spanY = y_hr >> IMAGE_SUBPIXEL_SHIFT;

                unsigned weight;

                luminance[0] = IMAGE_SUBPIXEL_SCALE * IMAGE_SUBPIXEL_SCALE / 2;
                luminance[1] = IMAGE_SUBPIXEL_SCALE * IMAGE_SUBPIXEL_SCALE / 2;
                luminance[2] = IMAGE_SUBPIXEL_SCALE * IMAGE_SUBPIXEL_SCALE / 2;
                luminance[3] = IMAGE_SUBPIXEL_SCALE * IMAGE_SUBPIXEL_SCALE / 2;

                x_hr &= IMAGE_SUBPIXEL_MASK;
                y_hr &= IMAGE_SUBPIXEL_MASK;

                colorsPtr = (const value_type*)spanImage::GetSource().span(spanX, spanY, 2);
                weight = (IMAGE_SUBPIXEL_SCALE - x_hr) *
                         (IMAGE_SUBPIXEL_SCALE - y_hr);
                luminance[0] += weight * *colorsPtr++;
                luminance[1] += weight * *colorsPtr++;
                luminance[2] += weight * *colorsPtr++;
                luminance[3] += weight * *colorsPtr;

                //获取下一个x对应颜色
                colorsPtr = (const value_type*)spanImage::GetSource().next_x();
                weight = x_hr * (IMAGE_SUBPIXEL_SCALE - y_hr);
                luminance[0] += weight * *colorsPtr++;
                luminance[1] += weight * *colorsPtr++;
                luminance[2] += weight * *colorsPtr++;
                luminance[3] += weight * *colorsPtr;

                //获取下一个y对应颜色
                colorsPtr = (const value_type*)spanImage::GetSource().next_y();
                weight = (IMAGE_SUBPIXEL_SCALE - x_hr) * y_hr;
                luminance[0] += weight * *colorsPtr++;
                luminance[1] += weight * *colorsPtr++;
                luminance[2] += weight * *colorsPtr++;
                luminance[3] += weight * *colorsPtr;

                //获取下一个x对应颜色
                colorsPtr = (const value_type*)spanImage::GetSource().next_x();
                weight = x_hr * y_hr;
                luminance[0] += weight * *colorsPtr++;
                luminance[1] += weight * *colorsPtr++;
                luminance[2] += weight * *colorsPtr++;
                luminance[3] += weight * *colorsPtr;

                span->r = value_type(color_type::downshift(luminance[order_type::R], IMAGE_SUBPIXEL_SHIFT * 2));
                span->g = value_type(color_type::downshift(luminance[order_type::G], IMAGE_SUBPIXEL_SHIFT * 2));
                span->b = value_type(color_type::downshift(luminance[order_type::B], IMAGE_SUBPIXEL_SHIFT * 2));
                span->a = value_type(color_type::downshift(luminance[order_type::A], IMAGE_SUBPIXEL_SHIFT * 2));

                ++span;
                ++spanImage::GetInterpolator();

            } while (--len);
        }
    };
} // namespace OHOS

#endif
