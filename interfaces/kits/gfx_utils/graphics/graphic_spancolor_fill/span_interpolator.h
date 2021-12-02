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
* @file span_interpolator.h
* @brief Defines 扫描线插入器
* @since 1.0
* @version 1.0
*/


#ifndef GRAPHIC_SPAN_INTERPOLATOR_LINEAR_INCLUDED
#define GRAPHIC_SPAN_INTERPOLATOR_LINEAR_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "gfx_utils/graphics/graphic_geometry/agg_dda_line.h"
#include "gfx_utils/graphics/graphic_transform/agg_trans_affine.h"
#include "gfx_utils/graphics/graphic_color/agg_color_rgba.h"

namespace OHOS
{

    /**
     *渐变的颜色插入器
     */
    template<class ColorT> struct color_interpolator
    {
    public:
        typedef ColorT color_type;

        color_interpolator(const color_type& c1,
                           const color_type& c2,
                           unsigned len) :
            m_c1(c1),
            m_c2(c2),
            m_len(len),
            m_count(0)
        {}

        void operator ++ ()
        {
            ++m_count;
        }

        /**
         * @brief 返回m_c1渐变到m_c2过程中处于m_count的颜色
         * @return
         */
        color_type color() const
        {
            return m_c1.gradient(m_c2, double(m_count) / m_len);
        }

    private:
        color_type m_c1;
        color_type m_c2;
        unsigned   m_len;
        unsigned   m_count;
    };


    /**
     *线性的扫描线插入器
     */
    template<class Transformer = trans_affine, unsigned SubpixelShift = 8> 
    class span_interpolator_linear
    {
    public:
        typedef Transformer trans_type;

        enum subpixel_scale_e
        {
            subpixel_shift = SubpixelShift,
            subpixel_scale  = 1 << subpixel_shift
        };

        //--------------------------------------------------------------------
        span_interpolator_linear() {}
        span_interpolator_linear(trans_type& trans) : m_trans(&trans) {}
        span_interpolator_linear(trans_type& trans,
                                 double x, double y, unsigned len) :
            m_trans(&trans)
        {
            begin(x, y, len);
        }

        //----------------------------------------------------------------
        const trans_type& transformer() const { return *m_trans; }
        void transformer(trans_type& trans) { m_trans = &trans; }

        //----------------------------------------------------------------
        void begin(double x, double y, unsigned len)
        {
            double tx;
            double ty;

            tx = x;
            ty = y;
            m_trans->transform(&tx, &ty);
            int x1 = iround(tx * subpixel_scale);
            int y1 = iround(ty * subpixel_scale);

            tx = x + len;
            ty = y;
            m_trans->transform(&tx, &ty);
            int x2 = iround(tx * subpixel_scale);
            int y2 = iround(ty * subpixel_scale);

            m_li_x = dda2_line_interpolator(x1, x2, len);
            m_li_y = dda2_line_interpolator(y1, y2, len);
        }

        //----------------------------------------------------------------
        void resynchronize(double xe, double ye, unsigned len)
        {
            m_trans->transform(&xe, &ye);
            m_li_x = dda2_line_interpolator(m_li_x.y(), iround(xe * subpixel_scale), len);
            m_li_y = dda2_line_interpolator(m_li_y.y(), iround(ye * subpixel_scale), len);
        }
    
        //----------------------------------------------------------------
        void operator++()
        {
            ++m_li_x;
            ++m_li_y;
        }

        //----------------------------------------------------------------
        void coordinates(int* x, int* y) const
        {
            *x = m_li_x.y();
            *y = m_li_y.y();
        }

    private:
        trans_type* m_trans;
        dda2_line_interpolator m_li_x;
        dda2_line_interpolator m_li_y;
    };
}

#endif


