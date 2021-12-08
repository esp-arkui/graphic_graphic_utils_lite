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
    enum image_rgba_scale_e
    {
        image_rgba_shift = 14,                    //----image_rgba_shift
        image_rgba_scale = 1 << image_rgba_shift, //----image_rgba_scale
        image_rgba_mask = image_rgba_scale - 1    //----image_rgba_mask
    };

    enum image_subpixel_scale_e
    {
        image_subpixel_shift = 8,                         //----image_subpixel_shift
        image_subpixel_scale = 1 << image_subpixel_shift, //----image_subpixel_scale
        image_subpixel_mask = image_subpixel_scale - 1    //----image_subpixel_mask
    };

    class image_rgba_lut
    {
    public:
        template <class FilterF>
        void calculate(const FilterF& filter,
                       bool normalization = true)
        {
            filter;
            double r = filter.radius();
            realloc_lut(r);
            unsigned i;
            unsigned pivot = diameter() << (image_subpixel_shift - 1);
            for (i = 0; i < pivot; i++)
            {
                double x = double(i) / double(image_subpixel_scale);
                double y = filter.calc_weight(x);
                m_weight_array[pivot + i] =
                    m_weight_array[pivot - i] = (int16)iround(y * image_rgba_scale);
            }
            unsigned end = (diameter() << image_subpixel_shift) - 1;
            m_weight_array[0] = m_weight_array[end];
            if (normalization)
            {
                normalize();
            }
        }

        image_rgba_lut() :
            m_radius(0), m_diameter(0), m_start(0)
        {
        }

        template <class FilterF>
        image_rgba_lut(const FilterF& filter,
                       bool normalization = true)
        {
            calculate(filter, normalization);
        }

        double radius() const
        {
            return m_radius;
        }
        unsigned diameter() const
        {
            return m_diameter;
        }
        int start() const
        {
            return m_start;
        }
        const int16* weight_array() const
        {
            return &m_weight_array[0];
        }
        void normalize();

    private:
        void realloc_lut(double radius);
        image_rgba_lut(const image_rgba_lut&);
        const image_rgba_lut& operator=(const image_rgba_lut&);

        double m_radius;
        unsigned m_diameter;
        int m_start;
        pod_array<int16> m_weight_array;
    };

    template <class FilterF>
    class image_rgba : public image_rgba_lut
    {
    public:
        image_rgba()
        {
            calculate(m_filter_function);
        }

    private:
        FilterF m_filter_function;
    };

    template <class Source, class Interpolator>
    class span_image
    {
    public:
        typedef Source source_type;
        typedef Interpolator interpolator_type;

        span_image()
        {
        }
        span_image(source_type& src,
                   interpolator_type& interpolator,
                   image_rgba_lut* filter) :
            m_src(&src),
            m_interpolator(&interpolator),
            m_filter(filter),
            m_dx_dbl(0.5),
            m_dy_dbl(0.5),
            m_dx_int(image_subpixel_scale / 2),
            m_dy_int(image_subpixel_scale / 2)
        {
        }
        void attach(source_type& v)
        {
            m_src = &v;
        }

        source_type& source()
        {
            return *m_src;
        }
        const source_type& source() const
        {
            return *m_src;
        }
        const image_rgba_lut& filter() const
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
            m_interpolator = &v;
        }
        /**
         * @brief 添加过滤模式
         */
        void filter(image_rgba_lut& v)
        {
            m_filter = &v;
        }
        void filter_offset(double dx, double dy)
        {
            m_dx_dbl = dx;
            m_dy_dbl = dy;
            m_dx_int = iround(dx * image_subpixel_scale);
            m_dy_int = iround(dy * image_subpixel_scale);
        }
        void filter_offset(double d)
        {
            filter_offset(d, d);
        }

        interpolator_type& interpolator()
        {
            return *m_interpolator;
        }
        void prepare()
        {
        }

    private:
        source_type* m_src;
        interpolator_type* m_interpolator;
        image_rgba_lut* m_filter;
        double m_dx_dbl;
        double m_dy_dbl;
        unsigned m_dx_int;
        unsigned m_dy_int;
    };

    template <class Source, class Interpolator>
    class span_image_rgba : public span_image<Source, Interpolator>
    {
    public:
        typedef Source source_type;
        typedef typename source_type::color_type color_type;
        typedef typename source_type::order_type order_type;
        typedef Interpolator interpolator_type;
        typedef span_image<source_type, interpolator_type> base_type;
        typedef typename color_type::value_type value_type;
        typedef typename color_type::calc_type calc_type;
        typedef typename color_type::long_type long_type;

        span_image_rgba()
        {
        }
        span_image_rgba(source_type& src, interpolator_type& inter) :
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

                int x_lr = x_hr >> image_subpixel_shift;
                int y_lr = y_hr >> image_subpixel_shift;

                unsigned weight;

                fg[0] =
                    fg[1] =
                        fg[2] =
                            fg[3] = image_subpixel_scale * image_subpixel_scale / 2;

                x_hr &= image_subpixel_mask;
                y_hr &= image_subpixel_mask;

                fg_ptr = (const value_type*)base_type::source().span(x_lr, y_lr, 2);
                weight = (image_subpixel_scale - x_hr) *
                         (image_subpixel_scale - y_hr);
                fg[0] += weight * *fg_ptr++;
                fg[1] += weight * *fg_ptr++;
                fg[2] += weight * *fg_ptr++;
                fg[3] += weight * *fg_ptr;

                fg_ptr = (const value_type*)base_type::source().next_x();
                weight = x_hr * (image_subpixel_scale - y_hr);
                fg[0] += weight * *fg_ptr++;
                fg[1] += weight * *fg_ptr++;
                fg[2] += weight * *fg_ptr++;
                fg[3] += weight * *fg_ptr;

                fg_ptr = (const value_type*)base_type::source().next_y();
                weight = (image_subpixel_scale - x_hr) * y_hr;
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

                span->r = value_type(color_type::downshift(fg[order_type::R], image_subpixel_shift * 2));
                span->g = value_type(color_type::downshift(fg[order_type::G], image_subpixel_shift * 2));
                span->b = value_type(color_type::downshift(fg[order_type::B], image_subpixel_shift * 2));
                span->a = value_type(color_type::downshift(fg[order_type::A], image_subpixel_shift * 2));

                ++span;
                ++base_type::interpolator();

            } while (--len);
        }
    };
} // namespace OHOS

#endif
