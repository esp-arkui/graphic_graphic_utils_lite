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
* @file span_gradient.h
* @brief Defines 扫描线渐变模式
* @since 1.0
* @version 1.0
*/


#ifndef GRAPHIC_SPAN_GRADIENT_INCLUDED
#define GRAPHIC_SPAN_GRADIENT_INCLUDED

#include <cmath>
#include <cstdlib>
#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "gfx_utils/graphics/graphic_geometry/agg_array.h"


namespace OHOS
{
    /**
     *渐变的扫描线填色模板
     */
    template<class ColorT,class Interpolator,class GradientF,class ColorF>
    class span_gradient
    {
    public:
        typedef Interpolator interpolator_type;
        typedef ColorT color_type;
        span_gradient() {}
        span_gradient(interpolator_type& inter,
                      GradientF& gradient_function,
                      ColorF& color_function,
                      double d1, double d2) : 
            m_interpolator(&inter),
            m_gradient_function(&gradient_function),
            m_color_function(&color_function),
            m_d1(iround(d1 * gradient_subpixel_scale)),
            m_d2(iround(d2 * gradient_subpixel_scale))
        {}

        void prepare() {}

        /**
         * @brief generate 从color_function取出rgba赋予span中的rgba
         * @param span 扫描线
         * @param x 坐标-x
         * @param y 坐标-y
         * @param len 扫描线长度
         */
        void generate(color_type* span, int x, int y, unsigned len)
        {   
            int downscaleShift = interpolator_type::subpixel_shift - gradient_subpixel_shift;
            m_interpolator->begin(x+0.5, y+0.5, len);
            for(;len;--len,++(*m_interpolator)){
                m_interpolator->coordinates(&x, &y);
                int index = m_gradient_function->calculate(x >> downscaleShift,
                                                           y >> downscaleShift,
                                                           m_d1,
                                                           m_d2,
                                                           m_color_function->size());
                *span++ = (*m_color_function)[index];
            }
        }

    private:
        interpolator_type* m_interpolator;
        GradientF*         m_gradient_function;
        ColorF*            m_color_function;
        int                m_d1;
        int                m_d2;
    };

    /**
     * @brief 计算放射渐变时当前(x,y)的color_function 数组下标
     */
    class GradientRadialCalculate
    {
    public:
        GradientRadialCalculate() :
            m_r(100 * gradient_subpixel_scale), 
            m_fx(0), 
            m_fy(0)
        {
            update_values();
        }

        //---------------------------------------------------------------------
        GradientRadialCalculate(double r, double fx, double fy) :
            m_r (iround(r  * gradient_subpixel_scale)), 
            m_fx(iround(fx * gradient_subpixel_scale)), 
            m_fy(iround(fy * gradient_subpixel_scale))
        {
            update_values();
        }

        /**
         * @brief calculate 计算放射渐变时当前(x,y)的color_function 数组下标
         * @param x 坐标x
         * @param y 坐标y
         * @param d1 开始圆半径
         * @param d2 结束圆半径
         * @param size color_function的size
         * @return
         */
        int calculate(int x, int y, int d1,int d2,int size) const
        {
            double dx = x - m_fx;
            double dy = y - m_fy;
            double m_d2 = dx * m_fy - dy * m_fx;
            double m_d3 = m_r2 * (dx * dx + dy * dy) - m_d2 * m_d2;
            int dd = d2 - d1;
            if(dd < 1) dd = 1;
            int index = ((iround((dx * m_fx + dy * m_fy + std::sqrt(std::fabs(m_d3))) * m_mul) - d1) * size) / dd;
            if(index < 0) index = 0;
            if(index >= size) index = size - 1;
            return index;
        }

    private:
        //---------------------------------------------------------------------
        void update_values()
        {
            m_r2  = double(m_r)  * double(m_r);
            m_fx2 = double(m_fx) * double(m_fx);
            m_fy2 = double(m_fy) * double(m_fy);
            double d = (m_r2 - (m_fx2 + m_fy2));
            if(d == 0)
            {
                if(m_fx) { if(m_fx < 0) ++m_fx; else --m_fx; }
                if(m_fy) { if(m_fy < 0) ++m_fy; else --m_fy; }
                m_fx2 = double(m_fx) * double(m_fx);
                m_fy2 = double(m_fy) * double(m_fy);
                d = (m_r2 - (m_fx2 + m_fy2));
            }
            m_mul = m_r / d;
        }

        int    m_r;
        int    m_fx;
        int    m_fy;
        double m_r2;
        double m_fx2;
        double m_fy2;
        double m_mul;
    };


    /**
     * @brief 计算线性渐变时当前(x,y)的color_function 数组下标
     */
    class GradientLinearCalculate
    {
    public:

        /**
         * @brief calculate 计算线性渐变时当前(x,y)的color_function 数组下标
         * @param x 坐标x
         * @param d2 线性渐变起止点的距离
         * @param size color_function的size
         * @return
         */
        static int calculate(int x, int , int ,int d2,int size)
        {
            if(d2 < 1) d2 = 1;
            int index = ( x  * size) / d2;
            if(index < 0) index = 0;
            if(index >= size) index = size - 1;
            return index;
        }
    };
}

#endif
