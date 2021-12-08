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

#include "gfx_utils/graphics/graphic_geometry/agg_array.h"
#include "gfx_utils/graphics/graphic_geometry/agg_dda_line.h"

namespace OHOS
{
    /**
     *根据remove_all,add_color,build_lut构建颜色的渐变过程，起止和中间的渐变颜色
     */
    template <class ColorInterpolator, unsigned ColorLutSize = 256>
    class gradient_lut
    {
    public:
        typedef ColorInterpolator interpolator_type;
        typedef typename interpolator_type::color_type color_type;
        enum
        {
            color_lut_size = ColorLutSize
        };
        gradient_lut() :
            m_color_lut(color_lut_size)
        {
        }

        /**
         *删除所有颜色
         */
        void remove_all()
        {
            m_color_profile.remove_all();
        }

        /**
         * @brief  在渐变过程中添加需要渐变的颜色和位置
         * @param offset (0-1)
         * @param color 添加的颜色
         */
        void add_color(double offset, const color_type& color)
        {
            m_color_profile.add(color_point(offset, color));
        }

        /**
        *根据渐变颜色构建color_lut_type
        */
        void build_lut()
        {
            quick_sort(m_color_profile, offset_less);
            m_color_profile.cut_at(remove_duplicates(m_color_profile, offset_equal));
            if (m_color_profile.size() > 1)
            {
                unsigned i;
                unsigned start = uround(m_color_profile[0].offset * color_lut_size);
                unsigned end;
                color_type c = m_color_profile[0].color;
                for (i = 0; i < start; i++)
                {
                    m_color_lut[i] = c;
                }
                for (i = 1; i < m_color_profile.size(); i++)
                {
                    end = uround(m_color_profile[i].offset * color_lut_size);
                    interpolator_type ci(m_color_profile[i - 1].color,
                                         m_color_profile[i].color,
                                         end - start + 1);
                    while (start < end)
                    {
                        m_color_lut[start] = ci.color();
                        ++ci;
                        ++start;
                    }
                }
                c = m_color_profile.last().color;
                for (; end < m_color_lut.size(); end++)
                {
                    m_color_lut[end] = c;
                }
            }
        }

        /**
         * @brief size 返回color_lut_type的size
         */
        static unsigned size()
        {
            return color_lut_size;
        }
        const color_type& operator[](unsigned i) const
        {
            return m_color_lut[i];
        }

    private:
        struct color_point
        {
            double offset;
            color_type color;

            color_point()
            {
            }
            color_point(double off, const color_type& c) :
                offset(off), color(c)
            {
                if (offset < 0.0)
                    offset = 0.0;
                if (offset > 1.0)
                    offset = 1.0;
            }
        };
        typedef OHOS::pod_bvector<color_point, 4> color_profile_type;
        typedef OHOS::pod_array<color_type> color_lut_type;

        static bool offset_less(const color_point& a, const color_point& b)
        {
            return a.offset < b.offset;
        }
        static bool offset_equal(const color_point& a, const color_point& b)
        {
            return a.offset == b.offset;
        }

        color_profile_type m_color_profile;
        color_lut_type m_color_lut;
    };
} // namespace OHOS

#endif
