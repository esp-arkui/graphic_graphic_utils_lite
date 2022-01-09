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
 * @brief 椭圆类
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
     * @brief 椭圆类
     * 椭圆类，必须给定椭圆的长短半轴的半径，才能够绘制椭圆
     * @see Ellipse
     * @since 1.0
     * @version 1.0
     */
    class Ellipse {
    public:
        /**
         * @brief Ellipse构造函数
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
         * @brief Ellipse构造函数
         * @param x为圆心的X坐标,y为圆心的Y坐标
         * @param rx为圆形的X半径,ry为圆形的X半径
         * @param numSteps为顶点数量
         * @param clockwise指的是顺时针，还是逆时针渲染
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        Ellipse(float x, float y, float rx, float ry,
                unsigned numSteps = 0, bool clockwise = false) :
            circleCenterX(x),
            circleCenterY(y), circleRadiusX(rx), circleRadiusY(ry), scaleRadio(1.0),
            vertexNumber(numSteps), circleInnerStep(0), isClockwise(clockwise)
        {
            if (vertexNumber == 0) {
                CalcNumSteps();
            }
        }

        /**
         * @brief 初始化函数
         * @param x为圆心的X坐标,y为圆心的Y坐标
         * @param rx为圆形的X半径,ry为圆形的X半径
         * @param numSteps为顶点数量
         * @param clockwise指的是顺时针，还是逆时针渲染
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        void Init(float x, float y, float rx, float ry,
                  unsigned numSteps = 0, bool clockwise = false);

        /**
         * @brief 为正确调整近似级别，应该调用方法ApproximationScale()
         * @param scale为视口坐标和逻辑坐标之间的比率
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        void ApproximationScale(float scale);

        /**
         * @brief 在采样阶段调用
         * @param x为点坐标x的值的指针，y为点坐标y的值的指针
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(float* x, float* y);

    private:
        /**
         * @brief 计算顶点数量
         * @param 无
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        void CalcNumSteps();

        float circleCenterX;     // 圆心的X坐标
        float circleCenterY;     // 圆心的Y坐标
        float circleRadiusX;     // 圆形的X半径
        float circleRadiusY;     // 圆形的Y半径
        float scaleRadio;        // 缩放比例
        unsigned vertexNumber;    // 顶点数量
        unsigned circleInnerStep; // 构建的是一个圆内接多边形
        bool isClockwise;         // 顺时针，还是逆时针渲染
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
