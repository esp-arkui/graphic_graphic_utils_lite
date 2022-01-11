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
 * @file graphic_geometry_ellipse.h
 * @brief Elliptic class
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_ELLIPSE_INCLUDED
#define GRAPHIC_GEOMETRY_ELLIPSE_INCLUDED

#include <cmath>

#include "gfx_utils/graphic_math.h"
#include "gfx_utils/graphics/common/graphic_common_basics.h"

namespace OHOS {
#if GRAPHIC_GEOMETYR_ENABLE_ELLIPSE_CIRCLE_VERTEX_SOURCE
    /**
     * @brief Elliptic class
     * Elliptic class,the radius of the major and minor axes of
     * the ellipse must be given before the ellipse can be drawn.
     * @see Ellipse
     * @since 1.0
     * @version 1.0
     */
    class Ellipse {
    public:
        /**
         * @brief Ellipse constructor
         * @param 无
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        Ellipse()
            : circleCenterX(0.0), circleCenterY(0.0),
              circleRadiusX(1.0), circleRadiusY(1.0), scaleRadio(1.0),
            vertexNumber(ELLIPSE_VERTEX_NUM), circleInnerStep(0), isClockwise(false)
        {}

        /**
         * @brief Ellipse constructor
         * @param x is the X coordinate of the center of the circle
         * @param y Is the Y coordinate of the center of the circle
         * @param rx is the X radius of the circle,ry is the Y radius of the circle.
         * @param numSteps Is the number of vertices.
         * @param clockwise Does it mean clockwise or counterclockwise rendering.
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        Ellipse(float x, float y, float rx, float ry,
                unsigned numSteps = 0, bool clockwise = false)
            : circleCenterX(x), circleCenterY(y),
              circleRadiusX(rx), circleRadiusY(ry),
              scaleRadio(1.0), vertexNumber(numSteps), circleInnerStep(0), isClockwise(clockwise)
        {
            if (vertexNumber == 0) {
                CalcNumSteps();
            }
        }

        /**
         * @brief Initialization function
         * @param X is the X coordinate of the center of the circle.
         * @param Y is the Y coordinate of the center of the circle
         * @param rx is the X radius of the circle.
         * @param ry is the Y radius of the circle.
         * @param numSteps is the number of vertices.
         * @param clockwise Does it mean clockwise or counterclockwise rendering.
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        void Init(float x, float y, float rx, float ry,
                  unsigned numSteps = 0, bool clockwise = false);

        /**
         * @brief To adjust the approximation level correctly, the method approximationscale() should be called.
         * @param scale is the ratio between viewport coordinates and logical coordinates.
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        void ApproximationScale(float scale);

        /**
         * @brief Called during the sampling phase.
         * @param X is the pointer to the value of point coordinate X.
         * @param Y is the pointer to the value of point coordinate y.
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(float* x, float* y);

    private:
        /**
         * @brief Calculate the number of vertices.
         * @param 无
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        void CalcNumSteps();

        float circleCenterX;     // X coordinate of circle center.
        float circleCenterY;     // Y coordinate of circle center.
        float circleRadiusX;     // X radius of circle.
        float circleRadiusY;     // Y radius of circle.
        float scaleRadio;        // Scale.
        unsigned vertexNumber;    // Number of vertices.
        unsigned circleInnerStep; // A circle inscribed polygon is constructed.
        bool isClockwise;         // Clockwise or counterclockwise rendering.
    };

    inline void Ellipse::Init(float x, float y, float rx, float ry,
                              unsigned numSteps, bool clockwise)
    {
        circleCenterX = x;
        circleCenterY = y;
        circleRadiusX = rx;
        circleRadiusY = ry;
        vertexNumber = numSteps;
        circleInnerStep = 0;
        isClockwise = clockwise;
        if (vertexNumber == 0) {
            CalcNumSteps();
        }
    }

    inline void Ellipse::ApproximationScale(float scale)
    {
        scaleRadio = scale;
        CalcNumSteps();
    }

    inline void Ellipse::CalcNumSteps()
    {
        const float raDa = 0.125f;
        float ra = (std::fabs(circleRadiusX) + std::fabs(circleRadiusY)) / FLOATNUM;
        float da = std::acos(ra / (ra + raDa / scaleRadio)) * FLOATNUM;
        vertexNumber = MATH_ROUND32(TWO_TIMES * PI / da);
    }

    inline unsigned Ellipse::Vertex(float* x, float* y)
    {
        if (circleInnerStep == vertexNumber) {
            ++circleInnerStep;
            return PATH_CMD_END_POLY | PATH_FLAGS_CLOSE | PATH_FLAGS_CLOCKWISE;
        }
        if (circleInnerStep > vertexNumber) {
            return PATH_CMD_STOP;
        }
        float angle = float(circleInnerStep) / float(vertexNumber) * FLOATNUM * PI;
        if (isClockwise) {
            angle = FLOATNUM * PI - angle;
        }
        *x = circleCenterX + std::cos(angle) * circleRadiusX;
        *y = circleCenterY + std::sin(angle) * circleRadiusY;
        circleInnerStep++;
        return ((circleInnerStep == 1) ? PATH_CMD_MOVE_TO : PATH_CMD_LINE_TO);
    }
#endif
} // namespace OHOS
#endif
