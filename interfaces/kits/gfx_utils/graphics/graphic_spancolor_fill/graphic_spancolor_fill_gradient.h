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

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"
#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_array.h"

namespace OHOS {
    /**
     * 渐变的扫描线填色模板
     * @template ColorT 颜色来源类型，Interpolator span 插值器
     * @GradientFgradient_function 对应模式计算当前点所处位置的函数
     * @ColorF 颜色数组
     */
    template <class ColorT, class Interpolator, class GradientF, class ColorF>
    class SpanGradient {
    public:
        typedef Interpolator interpolator_type;
        typedef ColorT color_type;
        SpanGradient()
        {
        }
        /**
         * @brief SpanGradient 扫描线渐变的构造函数
         * @param inter 插值器
         * @param gradient_function 对应模式计算当前点所处位置的函数
         * @param color_function 渐变梯度数组
         * @param distance1  根据模式确定参数内容：放射渐变时为 开始圆半径
         * @param distance2 根据模式确定参数内容：放射渐变时为 结束圆半径，线性渐变时为起止点的距离
         */
        SpanGradient(interpolator_type& inter,
                     GradientF& gradient_function,
                     ColorF& color_function,
                     double distance1, double distance2)
                     : interpolator_(&inter),
            gradientFunction_(&gradient_function),
            colorFunction_(&color_function),
            distance1_(Iround(distance1 * gradient_subpixel_scale)),
            distance2_(Iround(distance2 * gradient_subpixel_scale))
        {
        }

        void Prepare()
        {
        }

        /**
         * @brief Generate 从colorFunction_取出rgba赋予span中的rgba
         * @param span 需要填色的扫描线首地址
         * @param x 坐标-x
         * @param y 坐标-y
         * @param len 扫描线长度
         */
        void Generate(color_type* span, int x, int y, unsigned len)
        {
            int downscaleShift = interpolator_type::SUBPIXEL_SHIFT - gradient_subpixel_shift;
            interpolator_->Begin(x + HALFNUM, y + HALFNUM, len);
            for (; len; --len, ++(*interpolator_)) {
                interpolator_->Coordinates(&x, &y);
                int index = gradientFunction_->Calculate(x >> downscaleShift,
                                                         y >> downscaleShift,
                                                         distance1_,
                                                         distance2_,
                                                         colorFunction_->size());
                *span++ = (*colorFunction_)[index];
            }
        }

    private:
        interpolator_type* interpolator_;
        GradientF* gradientFunction_;
        ColorF* colorFunction_;
        int distance1_;
        int distance2_;
    };

    /**
     * gradient_function
     * @brief 计算放射渐变时当前(x,y)的color_function 数组下标
     * @since 1.0
     * @version 1.0
     */
    class GradientRadialCalculate {
    public:
        GradientRadialCalculate()
        : endRadius_(INDEX_HUNDRED * gradient_subpixel_scale),
            dx_(0),
            dy_(0)
        {
            UpdateValues();
        }

        /**
         * @brief GradientRadialCalculate 构造函数入参
         * @param endRadius 结束圆半径
         * @param dx x轴方向上，结束圆圆心到开始圆圆心得距离
         * @param dy y轴方向上，结束圆圆心到开始圆圆心得距离
         */
        GradientRadialCalculate(double endRadius, double dx, double dy)
        : endRadius_(Iround(endRadius * gradient_subpixel_scale)),
            dx_(Iround(dx * gradient_subpixel_scale)),
            dy_(Iround(dy * gradient_subpixel_scale))
        {
            UpdateValues();
        }

        /**
         * @brief calculate 计算放射渐变时当前(x,y)的color_function 数组下标
         * @param x 坐标x
         * @param y 坐标y
         * @param startRadius 开始圆半径
         * @param endRadius 结束圆半径
         * @param size colorFunction_的size
         * @return
         */
        int Calculate(int x, int y, int startRadius, int endRadius, int size) const
        {
            double dx = x - dx_;
            double dy = y - dy_;
            double m_distanceRadius = dx * dy_ - dy * dx_;
            double m_RadiusDistance = endRadiusSquare_ * (dx * dx + dy * dy) - m_distanceRadius * m_distanceRadius;
            int deltaRadius = endRadius - startRadius; // 半径的差
            if (deltaRadius < 1) {
                deltaRadius = 1;
            }
            int ret = Iround((dx * dx_ + dy * dy_ + std::sqrt(std::fabs(m_RadiusDistance))) * m_mul) - startRadius;
            int index = (ret * size) / deltaRadius;
            if (index < 0) {
                index = 0;
            }
            if (index >= size) {
                index = size - 1;
            }
            return index;
        }

    private:
        /**
         * @brief 更新m_mul
         */
        void UpdateValues()
        {
            endRadiusSquare_ = double(endRadius_) * double(endRadius_);
            double dxSquare_ = double(dx_) * double(dx_);
            double dySquare_ = double(dy_) * double(dy_);
            double dRadius = (endRadiusSquare_ - (dxSquare_ + dySquare_));
            if (dRadius == 0) {
                if (dx_) {
                    if (dx_ < 0) {
                        ++dx_;
                    } else {
                        --dx_;
                    }
                }
                if (dy_) {
                    if (dy_ < 0) {
                        ++dy_;
                    } else {
                        --dy_;
                    }
                }
                dxSquare_ = double(dx_) * double(dx_);
                dySquare_ = double(dy_) * double(dy_);
                dRadius = (endRadiusSquare_ - (dxSquare_ + dySquare_));
            }
            m_mul = endRadius_ / dRadius;
        }

        int endRadius_;
        /** x轴方向上，结束圆圆心到开始圆圆心得距离 */
        int dx_;
        /** y轴方向上，结束圆圆心到开始圆圆心得距离 */
        int dy_;
        double endRadiusSquare_;
        double m_mul;
    };

    /**
     * @brief 计算线性渐变时当前(x,y)的color_function 数组下标
     * @since 1.0
     * @version 1.0
     */
    class GradientLinearCalculate {
    public:
        /**
         * @brief calculate 计算线性渐变时当前(x,y)的color_function 数组下标
         * @param x 坐标x
         * @param distance 线性渐变起止点的距离
         * @param size color_function的size
         * @return
         */
        static int Calculate(int x, int, int, int distance, int size)
        {
            if (distance < 1) {
                distance = 1;
            }
            int index = (x * size) / distance;
            if (index < 0) {
                index = 0;
            }
            if (index >= size) {
                index = size - 1;
            }
            return index;
        }
    };
} // namespace OHOS

#endif
