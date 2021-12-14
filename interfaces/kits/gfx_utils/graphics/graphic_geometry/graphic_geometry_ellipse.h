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

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"

namespace OHOS {
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
        Ellipse() :
            x_(0.0), y_(0.0), rx_(1.0), ry_(1.0), scale_(1.0),
            num_(4), step_(0), clockwise_(false)
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
        Ellipse(double x, double y, double rx, double ry,
                unsigned numSteps = 0, bool clockwise = false) :
            x_(x),
            y_(y), rx_(rx), ry_(ry), scale_(1.0),
            num_(numSteps), step_(0), clockwise_(clockwise)
        {
            if (num_ == 0) {
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
        void Init(double x, double y, double rx, double ry,
                  unsigned numSteps = 0, bool clockwise = false);

        /**
         * @brief 为正确调整近似级别，应该调用方法ApproximationScale()
         * @param scale为视口坐标和逻辑坐标之间的比率
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        void ApproximationScale(double scale);

        /**
         * @brief 在采样阶段调用
         * @param x为点坐标x的值的指针，y为点坐标y的值的指针
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(double* x, double* y);

    private:
        /**
         * @brief 计算顶点数量
         * @param 无
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        void CalcNumSteps();

        double x_;       // 圆心的X坐标
        double y_;       // 圆心的Y坐标
        double rx_;      // 圆形的X半径
        double ry_;      // 圆形的Y半径
        double scale_;   // 缩放比例
        unsigned num_;   // 顶点数量
        unsigned step_;  // 构建的是一个圆内接多边形
        bool clockwise_; // 顺时针，还是逆时针渲染
    };

    inline void Ellipse::Init(double x, double y, double rx, double ry,
                              unsigned numSteps, bool clockwise)
    {
        x_ = x;
        y_ = y;
        rx_ = rx;
        ry_ = ry;
        num_ = numSteps;
        step_ = 0;
        clockwise_ = clockwise;
        if (num_ == 0) {
            CalcNumSteps();
        }
    }

    inline void Ellipse::ApproximationScale(double scale)
    {
        scale_ = scale;
        CalcNumSteps();
    }

    inline void Ellipse::CalcNumSteps()
    {
        double ra = (std::fabs(rx_) + std::fabs(ry_)) / 2;
        double da = std::acos(ra / (ra + 0.125 / scale_)) * 2;
        num_ = Uround(2 * PI / da);
    }

    inline unsigned Ellipse::Vertex(double* x, double* y)
    {
        if (step_ == num_) {
            ++step_;
            return PATH_CMD_END_POLY | PATH_FLAGS_CLOSE | PATH_FLAGS_CCW;
        }
        if (step_ > num_) {
            return PATH_CMD_STOP;
        }
        double angle = double(step_) / double(num_) * 2.0 * PI;
        if (clockwise_) {
            angle = 2.0 * PI - angle;
        }
        *x = x_ + std::cos(angle) * rx_;
        *y = y_ + std::sin(angle) * ry_;
        step_++;
        return ((step_ == 1) ? PATH_CMD_MOVE_TO : PATH_CMD_LINE_TO);
    }

} // namespace OHOS

#endif
