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
namespace OHOS
{
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

    class ImageRgbaLut
    {
    public:
        template <class FilterF>
        void Calculate(const FilterF& filter,
                       bool normalization = true)
        {
            filter;
            double r = filter.radius();
            realloc_lut(r);
            unsigned i;
            unsigned pivot = diameter() << (IMAGE_SUBPIXEL_SHIFT - 1);
            for (i = 0; i < pivot; i++)
            {
                double x = double(i) / double(IMAGE_SUBPIXEL_SCALE);
                double y = filter.calc_weight(x);
                weightArray_[pivot + i] =
                    weightArray_[pivot - i] = (int16)iround(y * IMAGE_RGBA_SCALE);
            }
            unsigned end = (diameter() << IMAGE_SUBPIXEL_SHIFT) - 1;
            weightArray_[0] = weightArray_[end];
            if (normalization)
            {
                normalize();
            }
        }

        ImageRgbaLut() :
            radius_(0), diameter_(0), start_(0)
        {
        }

        template <class FilterF>
        ImageRgbaLut(const FilterF& filter,
                     bool normalization = true)
        {
            calculate(filter, normalization);
        }

        double radius() const
        {
            return radius_;
        }
        unsigned diameter() const
        {
            return diameter_;
        }
        int start() const
        {
            return start_;
        }
        const int16* weight_array() const
        {
            return &weightArray_[0];
        }
        void normalize();

    private:
        void realloc_lut(double radius);
        ImageRgbaLut(const ImageRgbaLut&);
        const ImageRgbaLut& operator=(const ImageRgbaLut&);

        double radius_;
        unsigned diameter_;
        int start_;
        pod_array<int16> weightArray_;
    };

    template <class FilterF>
    class ImageRgba : public ImageRgbaLut
    {
    public:
        ImageRgba()
        {
            Calculate(filterFunction);
        }

    private:
        FilterF filterFunction;
    };

    template <class Source, class Interpolator>
    class SpanImage
    {
    public:
        typedef Source source_type;
        typedef Interpolator interpolator_type;

        SpanImage()
        {
        }
        SpanImage(source_type& src,
                  interpolator_type& interpolator,
                  ImageRgbaLut* filter) :
            src_(&src),
            interpolator_(&interpolator),
            m_filter(filter),
            m_dx_dbl(0.5),
            m_dy_dbl(0.5),
            m_dx_int(IMAGE_SUBPIXEL_SCALE / 2),
            m_dy_int(IMAGE_SUBPIXEL_SCALE / 2)
        {
        }
        void attach(source_type& v)
        {
            src_ = &v;
        }

        source_type& source()
        {
            return *src_;
        }
        const source_type& source() const
        {
            return *src_;
        }
        const ImageRgbaLut& filter() const
        {
            return *m_filter;
        }
        int filter_dx_int() const
        {
            return m_dx_int;
        }
        int filter_dy_int() const
        {
            return m_dy_int;
        }
        double filter_dx_dbl() const
        {
            return m_dx_dbl;
        }
        double filter_dy_dbl() const
        {
            return m_dy_dbl;
        }

        void interpolator(interpolator_type& v)
        {
            interpolator_ = &v;
        }
        /**
         * @brief 添加过滤模式
         */
        void filter(ImageRgbaLut& v)
        {
            m_filter = &v;
        }
        void filter_offset(double dx, double dy)
        {
            m_dx_dbl = dx;
            m_dy_dbl = dy;
            m_dx_int = iround(dx * IMAGE_SUBPIXEL_SCALE);
            m_dy_int = iround(dy * IMAGE_SUBPIXEL_SCALE);
        }
        void filter_offset(double d)
        {
            filter_offset(d, d);
        }

        interpolator_type& interpolator()
        {
            return *interpolator_;
        }
        void prepare()
        {
        }

    private:
        source_type* src_;
        interpolator_type* interpolator_;
        ImageRgbaLut* m_filter;
        double m_dx_dbl;
        double m_dy_dbl;
        unsigned m_dx_int;
        unsigned m_dy_int;
    };

    template <class Source, class Interpolator>
    class spanImageRgba : public SpanImage<Source, Interpolator>
    {
    public:
        typedef Source source_type;
        typedef typename source_type::color_type color_type;
        typedef typename source_type::order_type order_type;
        typedef Interpolator interpolator_type;
        typedef SpanImage<source_type, interpolator_type> base_type;
        typedef typename color_type::value_type value_type;
        typedef typename color_type::calc_type calc_type;
        typedef typename color_type::long_type long_type;

        spanImageRgba()
        {
        }
        spanImageRgba(source_type& src, interpolator_type& inter) :
            base_type(src, inter, 0)
        {
        }

        void generate(color_type* span, int x, int y, unsigned len)
        {
            base_type::interpolator().begin(x + base_type::filter_dx_dbl(),
                                            y + base_type::filter_dy_dbl(), len);

            long_type fg[4];
            const value_type* fg_ptr;

            do
            {
                int x_hr;
                int y_hr;

                base_type::interpolator().coordinates(&x_hr, &y_hr);

                x_hr -= base_type::filter_dx_int();
                y_hr -= base_type::filter_dy_int();

                int x_lr = x_hr >> IMAGE_SUBPIXEL_SHIFT;
                int y_lr = y_hr >> IMAGE_SUBPIXEL_SHIFT;

                unsigned weight;

                fg[0] =
                    fg[1] =
                        fg[2] =
                            fg[3] = IMAGE_SUBPIXEL_SCALE * IMAGE_SUBPIXEL_SCALE / 2;

                x_hr &= IMAGE_SUBPIXEL_MASK;
                y_hr &= IMAGE_SUBPIXEL_MASK;

                fg_ptr = (const value_type*)base_type::source().span(x_lr, y_lr, 2);
                weight = (IMAGE_SUBPIXEL_SCALE - x_hr) *
                         (IMAGE_SUBPIXEL_SCALE - y_hr);
                fg[0] += weight * *fg_ptr++;
                fg[1] += weight * *fg_ptr++;
                fg[2] += weight * *fg_ptr++;
                fg[3] += weight * *fg_ptr;

                fg_ptr = (const value_type*)base_type::source().next_x();
                weight = x_hr * (IMAGE_SUBPIXEL_SCALE - y_hr);
                fg[0] += weight * *fg_ptr++;
                fg[1] += weight * *fg_ptr++;
                fg[2] += weight * *fg_ptr++;
                fg[3] += weight * *fg_ptr;

                fg_ptr = (const value_type*)base_type::source().next_y();
                weight = (IMAGE_SUBPIXEL_SCALE - x_hr) * y_hr;
                fg[0] += weight * *fg_ptr++;
                fg[1] += weight * *fg_ptr++;
                fg[2] += weight * *fg_ptr++;
                fg[3] += weight * *fg_ptr;

                fg_ptr = (const value_type*)base_type::source().next_x();
                weight = x_hr * y_hr;
                fg[0] += weight * *fg_ptr++;
                fg[1] += weight * *fg_ptr++;
                fg[2] += weight * *fg_ptr++;
                fg[3] += weight * *fg_ptr;

                span->r = value_type(color_type::downshift(fg[order_type::R], IMAGE_SUBPIXEL_SHIFT * 2));
                span->g = value_type(color_type::downshift(fg[order_type::G], IMAGE_SUBPIXEL_SHIFT * 2));
                span->b = value_type(color_type::downshift(fg[order_type::B], IMAGE_SUBPIXEL_SHIFT * 2));
                span->a = value_type(color_type::downshift(fg[order_type::A], IMAGE_SUBPIXEL_SHIFT * 2));

                ++span;
                ++base_type::interpolator();

            } while (--len);
        }
    };
} // namespace OHOS

#endif
