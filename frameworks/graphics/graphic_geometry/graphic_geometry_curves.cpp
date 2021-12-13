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

#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_curves.h"

#include <cmath>

#include "gfx_utils/graphics/graphic_common/graphic_common_math.h"

namespace OHOS {
    const double CURVECOLLINEARITYEPSILON = 1e-30;
    const double CURVEANGLETOLERANCEEPSILON = 0.01;
    const int CURVERECURSIONLIMIT = 32;

    double Curve3Inc::ApproximationScale() const
    {
        return scale_;
    }

    void Curve3Inc::ApproximationScale(double scale)
    {
        scale_ = scale;
    }

    void Curve3Inc::Init(double x1, double y1,
                         double x2, double y2,
                         double x3, double y3)
    {
        startX_ = x1;
        startY_ = y1;
        endX_ = x3;
        endY_ = y3;

        double deltaX1 = x2 - x1;
        double deltaY1 = y2 - y1;
        double deltaX2 = x3 - x2;
        double deltaY2 = y3 - y2;

        double len = std::sqrt(deltaX1 * deltaX1 + deltaY1 * deltaY1) +
                     std::sqrt(deltaX2 * deltaX2 + deltaY2 * deltaY2);

        numSteps_ = Uround(len * 0.25 * scale_);

        if (numSteps_ < 4) {
            numSteps_ = 4;
        }

        double subdivideStep = 1.0 / numSteps_;
        double subdivideStep2 = subdivideStep * subdivideStep;

        double tmpX = (x1 - x2 * 2.0 + x3) * subdivideStep2;
        double tmpY = (y1 - y2 * 2.0 + y3) * subdivideStep2;

        savedFx_ = x1;
        fx_ = x1;
        savedFy_ = y1;
        fy_ = y1;

        savedDfx_ = tmpX + (x2 - x1) * (2.0 * subdivideStep);
        dfx_ = tmpY + (x2 - x1) * (2.0 * subdivideStep);
        savedDfy_ = tmpY + (y2 - y1) * (2.0 * subdivideStep);
        dfy_ = tmpY + (y2 - y1) * (2.0 * subdivideStep);

        ddfx_ = tmpX * 2.0;
        ddfy_ = tmpY * 2.0;

        step_ = numSteps_;
    }

    void Curve3Inc::Rewind(unsigned)
    {
        if (numSteps_ == 0) {
            step_ = -1;
            return;
        }

        step_ = numSteps_;
        fx_ = savedFx_;
        fy_ = savedFy_;
        dfx_ = savedDfx_;
        dfy_ = savedDfy_;
    }

    unsigned Curve3Inc::Vertex(double* x, double* y)
    {
        if (step_ < 0) {
            return PATH_CMD_STOP;
        }
        if (step_ == numSteps_) {
            *x = startX_;
            *y = startY_;
            --step_;
            return PATH_CMD_MOVE_TO;
        }
        if (step_ == 0) {
            *x = endX_;
            *y = endY_;
            --step_;
            return PATH_CMD_LINE_TO;
        }

        fx_ += dfx_;
        fy_ += dfy_;
        dfx_ += ddfx_;
        dfy_ += ddfy_;
        *x = fx_;
        *y = fy_;
        --step_;
        return PATH_CMD_LINE_TO;
    }

    void Curve3Div::Init(double x1, double y1,
                         double x2, double y2,
                         double x3, double y3)
    {
        points_.RemoveAll();
        distanceToleranceSquare_ = 0.5 / approximationScale_;
        distanceToleranceSquare_ *= distanceToleranceSquare_;
        Bezier(x1, y1, x2, y2, x3, y3);
        count_ = 0;
    }

    void Curve3Div::RecursiveBezier(double x1, double y1,
                                    double x2, double y2,
                                    double x3, double y3,
                                    unsigned level)
    {
        if (level > CURVERECURSIONLIMIT) {
            return;
        }

        // 计算线段的所有中点
        double x12 = (x1 + x2) / 2;
        double y12 = (y1 + y2) / 2;
        double x23 = (x2 + x3) / 2;
        double y23 = (y2 + y3) / 2;
        double x123 = (x12 + x23) / 2;
        double y123 = (y12 + y23) / 2;

        double deltaX = x3 - x1;
        double deltaY = y3 - y1;
        double distance = std::fabs(((x2 - x3) * deltaY - (y2 - y3) * deltaX));
        double da;

        if (distance > CURVECOLLINEARITYEPSILON) {
            if (distance * distance <= distanceToleranceSquare_ * (deltaX * deltaX + deltaY * deltaY)) {
                // 如果曲率未超过距离公差值
                if (angleTolerance_ < CURVEANGLETOLERANCEEPSILON) {
                    points_.Add(PointD(x123, y123));
                    return;
                }

                da = std::fabs(std::atan2(y3 - y2, x3 - x2) - std::atan2(y2 - y1, x2 - x1));
                if (da >= PI) {
                    da = 2 * PI - da;
                }

                if (da < angleTolerance_) {
                    points_.Add(PointD(x123, y123));
                    return;
                }
            }
        } else {
            da = deltaX * deltaX + deltaY * deltaY;
            if (da == 0) {
                distance = CalcSqDistance(x1, y1, x2, y2);
            } else {
                distance = ((x2 - x1) * deltaX + (y2 - y1) * deltaY) / da;
                if (distance > 0 && distance < 1) {
                    //简单共线情况，1---2---3
                    //我们可以只留下两个端点
                    return;
                }
                if (distance <= 0) {
                    distance = CalcSqDistance(x2, y2, x1, y1);
                } else if (distance >= 1) {
                    distance = CalcSqDistance(x2, y2, x3, y3);
                } else {
                    distance = CalcSqDistance(x2, y2, x1 + distance * deltaX, y1 + distance * deltaY);
                }
            }
            if (distance < distanceToleranceSquare_) {
                points_.Add(PointD(x2, y2));
                return;
            }
        }

        // 继续细分
        RecursiveBezier(x1, y1, x12, y12, x123, y123, level + 1);
        RecursiveBezier(x123, y123, x23, y23, x3, y3, level + 1);
    }

    void Curve3Div::Bezier(double x1, double y1,
                           double x2, double y2,
                           double x3, double y3)
    {
        points_.Add(PointD(x1, y1));
        RecursiveBezier(x1, y1, x2, y2, x3, y3, 0);
        points_.Add(PointD(x3, y3));
    }

    void Curve4Inc::ApproximationScale(double scale)
    {
        scale_ = scale;
    }

    double Curve4Inc::ApproximationScale() const
    {
        return scale_;
    }

#if defined(_MSC_VER) && _MSC_VER <= 1200
    static double MSC60FixICE(double value)
    {
        return value;
    }
#endif

    void Curve4Inc::Init(double x1, double y1,
                         double x2, double y2,
                         double x3, double y3,
                         double x4, double y4)
    {
        startX_ = x1;
        startY_ = y1;
        endX_ = x4;
        endY_ = y4;

        double deltaX1 = x2 - x1;
        double deltaY1 = y2 - y1;
        double deltaX2 = x3 - x2;
        double deltaY2 = y3 - y2;
        double deltaX3 = x4 - x3;
        double deltaY3 = y4 - y3;

        double len = (std::sqrt(deltaX1 * deltaX1 + deltaY1 * deltaY1) +
                      std::sqrt(deltaX2 * deltaX2 + deltaY2 * deltaY2) +
                      std::sqrt(deltaX3 * deltaX3 + deltaY3 * deltaY3)) *
                     0.25 * scale_;

#if defined(_MSC_VER) && _MSC_VER <= 1200
        numSteps_ = Uround(MSC60FixICE(len));
#else
        numSteps_ = Uround(len);
#endif

        if (numSteps_ < 4) {
            numSteps_ = 4;
        }

        double subdivideStep = 1.0 / numSteps_;
        double subdivideStep2 = subdivideStep * subdivideStep;
        double subdivideStep3 = subdivideStep * subdivideStep * subdivideStep;

        double pre1 = 3.0 * subdivideStep;
        double pre2 = 3.0 * subdivideStep2;
        double pre4 = 6.0 * subdivideStep2;
        double pre5 = 6.0 * subdivideStep3;

        double tmp1X = x1 - x2 * 2.0 + x3;
        double tmp1Y = y1 - y2 * 2.0 + y3;

        double tmp2X = (x2 - x3) * 3.0 - x1 + x4;
        double tmp2Y = (y2 - y3) * 3.0 - y1 + y4;

        savedFx_ = x1;
        fx_ = x1;
        savedFy_ = y1;
        fy_ = y1;

        savedDfx_ = (x2 - x1) * pre1 + tmp1X * pre2 + tmp2X * subdivideStep3;
        dfx_ = (x2 - x1) * pre1 + tmp1X * pre2 + tmp2X * subdivideStep3;
        savedDfy_ = (y2 - y1) * pre1 + tmp1Y * pre2 + tmp2Y * subdivideStep3;
        dfy_ = (y2 - y1) * pre1 + tmp1Y * pre2 + tmp2Y * subdivideStep3;

        savedDdfx_ = tmp1X * pre4 + tmp2X * pre5;
        ddfx_ = tmp1X * pre4 + tmp2X * pre5;
        savedDdfy_ = tmp1Y * pre4 + tmp2Y * pre5;
        ddfy_ = tmp1Y * pre4 + tmp2Y * pre5;

        dddfx_ = tmp2X * pre5;
        dddfy_ = tmp2Y * pre5;

        step_ = numSteps_;
    }

    void Curve4Inc::Rewind(unsigned)
    {
        if (numSteps_ == 0) {
            step_ = -1;
            return;
        }

        step_ = numSteps_;
        fx_ = savedFx_;
        fy_ = savedFy_;
        dfx_ = savedDfx_;
        dfy_ = savedDfy_;
        ddfx_ = savedDdfx_;
        ddfy_ = savedDdfy_;
    }

    unsigned Curve4Inc::Vertex(double* x, double* y)
    {
        if (step_ < 0) {
            return PATH_CMD_STOP;
        }
        if (step_ == numSteps_) {
            *x = startX_;
            *y = startY_;
            --step_;
            return PATH_CMD_MOVE_TO;
        }

        if (step_ == 0) {
            *x = endX_;
            *y = endY_;
            --step_;
            return PATH_CMD_LINE_TO;
        }

        fx_ += dfx_;
        fy_ += dfy_;
        dfx_ += ddfx_;
        dfy_ += ddfy_;
        ddfx_ += dddfx_;
        ddfy_ += dddfy_;

        *x = fx_;
        *y = fy_;
        --step_;
        return PATH_CMD_LINE_TO;
    }

    void Curve4Div::Init(double x1, double y1,
                         double x2, double y2,
                         double x3, double y3,
                         double x4, double y4)
    {
        points_.RemoveAll();
        distanceToleranceSquare_ = 0.5 / approximationScale_;
        distanceToleranceSquare_ *= distanceToleranceSquare_;
        Bezier(x1, y1, x2, y2, x3, y3, x4, y4);
        count_ = 0;
    }

    void Curve4Div::RecursiveBezier(double x1, double y1,
                                    double x2, double y2,
                                    double x3, double y3,
                                    double x4, double y4,
                                    unsigned level)
    {
        if (level > CURVERECURSIONLIMIT) {
            return;
        }

        // 计算线段的所有中点
        double x12 = (x1 + x2) / 2;
        double y12 = (y1 + y2) / 2;
        double x23 = (x2 + x3) / 2;
        double y23 = (y2 + y3) / 2;
        double x34 = (x3 + x4) / 2;
        double y34 = (y3 + y4) / 2;
        double x123 = (x12 + x23) / 2;
        double y123 = (y12 + y23) / 2;
        double x234 = (x23 + x34) / 2;
        double y234 = (y23 + y34) / 2;
        double x1234 = (x123 + x234) / 2;
        double y1234 = (y123 + y234) / 2;

        // 试着用一条直线近似整个三次曲线
        double deltaX = x4 - x1;
        double deltaY = y4 - y1;

        double delta2 = std::fabs(((x2 - x4) * deltaY - (y2 - y4) * deltaX));
        double delta3 = std::fabs(((x3 - x4) * deltaY - (y3 - y4) * deltaX));

        double da1;
        double da2;
        double gradient;

        switch ((int(delta2 > CURVECOLLINEARITYEPSILON) << 1) + int(delta3 > CURVECOLLINEARITYEPSILON)) {
            case 0:
                // 全部共线或p1 == p4
                gradient = deltaX * deltaX + deltaY * deltaY;
                if (gradient == 0) {
                    delta2 = CalcSqDistance(x1, y1, x2, y2);
                    delta3 = CalcSqDistance(x4, y4, x3, y3);
                } else {
                    gradient = 1 / gradient;
                    da1 = x2 - x1;
                    da2 = y2 - y1;
                    delta2 = gradient * (da1 * deltaX + da2 * deltaY);
                    da1 = x3 - x1;
                    da2 = y3 - y1;
                    delta3 = gradient * (da1 * deltaX + da2 * deltaY);
                    if (delta2 > 0 && delta2 < 1 && delta3 > 0 && delta3 < 1) {
                        //简单共线情况，1--2--3--4
                        //我们可以只留下两个端点
                        return;
                    }
                    if (delta2 <= 0) {
                        delta2 = CalcSqDistance(x2, y2, x1, y1);
                    } else if (delta2 >= 1) {
                        delta2 = CalcSqDistance(x2, y2, x4, y4);
                    } else {
                        delta2 = CalcSqDistance(x2, y2, x1 + delta2 * deltaX, y1 + delta2 * deltaY);
                    }

                    if (delta3 <= 0) {
                        delta3 = CalcSqDistance(x3, y3, x1, y1);
                    } else if (delta3 >= 1) {
                        delta3 = CalcSqDistance(x3, y3, x4, y4);
                    } else {
                        delta3 = CalcSqDistance(x3, y3, x1 + delta3 * deltaX, y1 + delta3 * deltaY);
                    }
                }
                if (delta2 > delta3) {
                    if (delta2 < distanceToleranceSquare_) {
                        points_.Add(PointD(x2, y2));
                        return;
                    }
                } else {
                    if (delta3 < distanceToleranceSquare_) {
                        points_.Add(PointD(x3, y3));
                        return;
                    }
                }
                break;

            case 1:
                // p1、p2、p4是共线的
                if (delta3 * delta3 <= distanceToleranceSquare_ * (deltaX * deltaX + deltaY * deltaY)) {
                    if (angleTolerance_ < CURVEANGLETOLERANCEEPSILON) {
                        points_.Add(PointD(x23, y23));
                        return;
                    }

                    // 角度条件
                    da1 = std::fabs(std::atan2(y4 - y3, x4 - x3) - std::atan2(y3 - y2, x3 - x2));
                    if (da1 >= PI) {
                        da1 = 2 * PI - da1;
                    }

                    if (da1 < angleTolerance_) {
                        points_.Add(PointD(x2, y2));
                        points_.Add(PointD(x3, y3));
                        return;
                    }

                    if (cuspLimit_ != 0.0) {
                        if (da1 > cuspLimit_) {
                            points_.Add(PointD(x3, y3));
                            return;
                        }
                    }
                }
                break;

            case 2:
                // p1、p3、p4是共线的
                if (delta2 * delta2 <= distanceToleranceSquare_ * (deltaX * deltaX + deltaY * deltaY)) {
                    if (angleTolerance_ < CURVEANGLETOLERANCEEPSILON) {
                        points_.Add(PointD(x23, y23));
                        return;
                    }

                    // 角度条件
                    da1 = std::fabs(std::atan2(y3 - y2, x3 - x2) - std::atan2(y2 - y1, x2 - x1));
                    if (da1 >= PI) {
                        da1 = 2 * PI - da1;
                    }

                    if (da1 < angleTolerance_) {
                        points_.Add(PointD(x2, y2));
                        points_.Add(PointD(x3, y3));
                        return;
                    }

                    if (cuspLimit_ != 0.0) {
                        if (da1 > cuspLimit_) {
                            points_.Add(PointD(x2, y2));
                            return;
                        }
                    }
                }
                break;

            case 3:
                if ((delta2 + delta3) * (delta2 + delta3) <= distanceToleranceSquare_ * (deltaX * deltaX + deltaY * deltaY)) {
                    // 如果曲率未超过距离公差值
                    if (angleTolerance_ < CURVEANGLETOLERANCEEPSILON) {
                        points_.Add(PointD(x23, y23));
                        return;
                    }

                    gradient = std::atan2(y3 - y2, x3 - x2);
                    da1 = std::fabs(gradient - std::atan2(y2 - y1, x2 - x1));
                    da2 = std::fabs(std::atan2(y4 - y3, x4 - x3) - gradient);
                    if (da1 >= PI) {
                        da1 = 2 * PI - da1;
                    }
                    if (da2 >= PI) {
                        da2 = 2 * PI - da2;
                    }

                    if (da1 + da2 < angleTolerance_) {
                        points_.Add(PointD(x23, y23));
                        return;
                    }

                    if (cuspLimit_ != 0.0) {
                        if (da1 > cuspLimit_) {
                            points_.Add(PointD(x2, y2));
                            return;
                        }

                        if (da2 > cuspLimit_) {
                            points_.Add(PointD(x3, y3));
                            return;
                        }
                    }
                }
                break;
        }

        RecursiveBezier(x1, y1, x12, y12, x123, y123, x1234, y1234, level + 1);
        RecursiveBezier(x1234, y1234, x234, y234, x34, y34, x4, y4, level + 1);
    }

    void Curve4Div::Bezier(double x1, double y1,
                           double x2, double y2,
                           double x3, double y3,
                           double x4, double y4)
    {
        points_.Add(PointD(x1, y1));
        RecursiveBezier(x1, y1, x2, y2, x3, y3, x4, y4, 0);
        points_.Add(PointD(x4, y4));
    }

} // namespace OHOS
