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
 * @brief Defines Scanline gradient mode
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_SPAN_GRADIENT_INCLUDED
#define GRAPHIC_SPAN_GRADIENT_INCLUDED

#include <cmath>
#include <cstdlib>

#include "gfx_utils/graphics/common/graphic_common_basics.h"
#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_array.h"
#include "gfx_utils/graphic_math.h"
namespace OHOS {
    /**
     * Gradient scanline fill template
     * @template ColorT Color source type, Interpolator span interpolator
     * @GradientFgradient_function The function of
     * the corresponding mode to calculate the position of the current point
     * @ColorF Color array
     */
    template <class ColorT, class Interpolator, class GradientF, class ColorF>
    class SpanFillColorGradient {
#if GRAPHIC_GEOMETYR_ENABLE_GRADIENT_FILLSTROKECOLOR
    public:
        typedef Interpolator interpolator_type;
        typedef ColorT color_type;
        SpanFillColorGradient()
        {
        }
        /**
         * @brief SpanGradient Constructor for scanline gradient
         * @param inter Interpolator
         * @param gradient_function
         * The function of the corresponding mode to calculate the position of the current point
         * @param color_function Gradient array
         * @param distance1 The parameter content is determined according to the mode:
         * it is the starting circle radius when radiating the gradient
         * @param distance2 Determine the parameter content according to the mode:
         * the radius of the end circle in the case of radial gradient and
         * the distance of the starting and ending points in the case of linear gradient
         */
        SpanFillColorGradient(interpolator_type& inter,
                              GradientF& gradient_function,
                              ColorF& color_function,
                              double distance1, double distance2)
            : interpolator_(&inter),
            gradientFunction_(&gradient_function),
            colorFunction_(&color_function),
            distance1_(MATH_ROUND32(distance1 * GRADIENT_SUBPIXEL_SCALE)),
            distance2_(MATH_ROUND32(distance2 * GRADIENT_SUBPIXEL_SCALE))
        {
        }

        void Prepare()
        {
        }

        /**
         * @brief Generate From colorfunction_ Remove the rgba from the span
         * @param span First address of scan line to be filled
         * @param x coordinate-x
         * @param y coordinate-y
         * @param len Scan line length
         */
        void Generate(color_type* span, int x, int y, unsigned len)
        {
            int downscaleShift = interpolator_type::SUBPIXEL_SHIFT - GRADIENT_SUBPIXEL_SHIFT;
            interpolator_->Begin(x + HALFNUM, y + HALFNUM, len);
            for (; len; --len, ++(*interpolator_)) {
                interpolator_->Coordinates(&x, &y);
                float proportion = gradientFunction_->Calculate(x >> downscaleShift,
                                                                y >> downscaleShift,
                                                                distance1_,
                                                                distance2_,
                                                                colorFunction_->size());
                *span++ = colorFunction_->getColor(proportion);
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
     * @brief The subscript of the current (x, y) color_function array when calculating the radial gradient
     * @since 1.0
     * @version 1.0
     */
    class GradientRadialCalculate {
    public:
        GradientRadialCalculate()
            : endRadius_(HUNDRED_TIMES * GRADIENT_SUBPIXEL_SCALE),
            dx_(0),
            dy_(0)
        {
            UpdateValues();
        }

        /**
         * @brief GradientRadialCalculate Constructor arguments
         * @param endRadius End circle radius
         * @param dx In the x-axis direction, the distance from the end circle center to the start circle center
         * @param dy In the y-axis direction, the distance from the end circle center to the start circle center
         */
        GradientRadialCalculate(double endRadius, double dx, double dy)
            : endRadius_(MATH_ROUND32(endRadius * GRADIENT_SUBPIXEL_SCALE)),
            dx_(MATH_ROUND32(dx * GRADIENT_SUBPIXEL_SCALE)),
            dy_(MATH_ROUND32(dy * GRADIENT_SUBPIXEL_SCALE))
        {
            UpdateValues();
        }

        /**
         * @brief calculate The subscript of the current (x, y) color_function array when calculating the radial gradient
         * @param x coordinate x
         * @param y coordinate y
         * @param startRadius Start circle radius
         * @param endRadius End circle radius
         * @param size colorFunction_的size
         * @return
         */
        float Calculate(float x, float y, float startRadius, float endRadius, int size) const
        {
            double dx = x - dx_;
            double dy = y - dy_;
            double m_distanceRadius = dx * dy_ - dy * dx_;
            double m_RadiusDistance = endRadiusSquare_ * (dx * dx + dy * dy)
                    - m_distanceRadius * m_distanceRadius;
            int deltaRadius = endRadius - startRadius; // Difference of radius
            if (deltaRadius < 1) {
                deltaRadius = 1;
            }
            float proportion = (MATH_ROUND32((dx * dx_ + dy * dy_ +
                                              std::sqrt(std::fabs(m_RadiusDistance)))
                                             * m_mul) - startRadius)/deltaRadius;
            if (proportion < 0) {
                proportion = 0;
            }
            if (proportion >= 1) {
                proportion = 1;
            }
            return proportion;
        }

    private:
        /**
         * @brief update m_mul
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
        /** In the x-axis direction, the distance from the end circle center to the start circle center */
        int dx_;
        /** In the y-axis direction, the distance from the end circle center to the start circle center */
        int dy_;
        double endRadiusSquare_;
        double m_mul;
    };

    /**
     * @brief The subscript of the current (x, y) color_function array when calculating a linear gradient
     * @since 1.0
     * @version 1.0
     */
    class GradientLinearCalculate {
    public:
        /**
         * @brief The subscript of the current (x, y) color_function array when calculating a linear gradient
         * @param x coordinate x
         * @param distance Distance between start and end of linear gradient
         * @param size color_function的size
         * @return
         */
        static float Calculate(float x, int, int, float distance, int size)
        {
            if (distance < 1) {
                distance = 1;
            }
            float proportion = x / distance;
            if (proportion < 0) {
                proportion = 0;
            }
            if (proportion >= 1) {
                proportion = 1;
            }
            return proportion;
        }
#endif
    };
} // namespace OHOS
#endif
