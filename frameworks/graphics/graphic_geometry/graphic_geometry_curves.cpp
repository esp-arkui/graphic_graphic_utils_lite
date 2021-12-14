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

#include "gfx_utils/graphics/graphic_common/graphic_common_math.h"

namespace OHOS {
    const double CURVECOLLINEARITYEPSILON = 1e-30;
    const double CURVEANGLETOLERANCEEPSILON = 0.01;
    const int CURVERECURSIONLIMIT = 32;
    const double CURVES_NUM_STEP_LEN = 0.25;
    double QuadraticBezierCurveIncrement::ApproximationScale() const
    {
        return approximationScale;
    }

    void QuadraticBezierCurveIncrement::ApproximationScale(double scale)
    {
        approximationScale = scale;
    }

    void QuadraticBezierCurveIncrement::Init(double x1, double y1,
                                             double x2, double y2,
                                             double x3, double y3)
    {
        startXCoordinate = x1;
        startYCoordinate = y1;
        endXCoordinate = x3;
        endYCoordinate = y3;

        double deltaX1 = x2 - x1;
        double deltaY1 = y2 - y1;
        double deltaX2 = x3 - x2;
        double deltaY2 = y3 - y2;

        double len = std::sqrt(deltaX1 * deltaX1 + deltaY1 * deltaY1) +
                     std::sqrt(deltaX2 * deltaX2 + deltaY2 * deltaY2);

        numberSteps = Uround(len * CURVES_NUM_STEP_LEN * approximationScale);
        const int NUM_STEPS_MAX = 4;
        if (numberSteps < NUM_STEPS_MAX) {
            numberSteps = NUM_STEPS_MAX;
        }
        double subdivideStep = 1.0 / numberSteps;
        double subdivideStep2 = subdivideStep * subdivideStep;

        double tmpX = (x1 - x2 * DOUBLENUM + x3) * subdivideStep2;
        double tmpY = (y1 - y2 * DOUBLENUM + y3) * subdivideStep2;

        savedFinalXCoordinate = x1;
        finalXCoordinate = x1;
        savedFinalYCoordinate = y1;
        finalYCoordinate = y1;

        savedDeltaFinalXCoordinate = tmpX + (x2 - x1) * (DOUBLENUM * subdivideStep);
        deltaFinalXCoordinate = tmpY + (x2 - x1) * (DOUBLENUM * subdivideStep);
        savedDeltaFinalYCoordinate = tmpY + (y2 - y1) * (DOUBLENUM * subdivideStep);
        deltaFinalYCoordinate = tmpY + (y2 - y1) * (DOUBLENUM * subdivideStep);

        doubleDeltaFinalXCoordinate = tmpX * DOUBLENUM;
        doubleDeltaFinalYCoordinate = tmpY * DOUBLENUM;

        currentStep = numberSteps;
    }

    void QuadraticBezierCurveIncrement::Rewind(unsigned)
    {
        if (numberSteps == 0) {
            currentStep = -1;
            return;
        }

        currentStep = numberSteps;
        finalXCoordinate = savedFinalXCoordinate;
        finalYCoordinate = savedFinalYCoordinate;
        deltaFinalXCoordinate = savedDeltaFinalXCoordinate;
        deltaFinalYCoordinate = savedDeltaFinalYCoordinate;
    }

    unsigned QuadraticBezierCurveIncrement::Vertex(double* x, double* y)
    {
        if (currentStep < 0) {
            return PATH_CMD_STOP;
        }
        if (currentStep == numberSteps) {
            *x = startXCoordinate;
            *y = startYCoordinate;
            --currentStep;
            return PATH_CMD_MOVE_TO;
        }
        if (currentStep == 0) {
            *x = endXCoordinate;
            *y = endYCoordinate;
            --currentStep;
            return PATH_CMD_LINE_TO;
        }

        finalXCoordinate += deltaFinalXCoordinate;
        finalYCoordinate += deltaFinalYCoordinate;
        deltaFinalXCoordinate += doubleDeltaFinalXCoordinate;
        deltaFinalYCoordinate += doubleDeltaFinalYCoordinate;
        *x = finalXCoordinate;
        *y = finalYCoordinate;
        --currentStep;
        return PATH_CMD_LINE_TO;
    }

    void QuadraticBezierCurveDividOperate::Init(double x1, double y1,
                         double x2, double y2,
                         double x3, double y3)
    {
        points_.RemoveAll();
        distanceToleranceSquare_ = HALFNUM / approximationScale_;
        distanceToleranceSquare_ *= distanceToleranceSquare_;
        Bezier(x1, y1, x2, y2, x3, y3);
        count_ = 0;
    }

    void QuadraticBezierCurveDividOperate::RecursiveBezier(double x1, double y1,
                                    double x2, double y2,
                                    double x3, double y3,
                                    unsigned level)
    {
        if (level > CURVERECURSIONLIMIT) {
            return;
        }

        // 计算线段的所有中点
        double x12 = (x1 + x2) / DOUBLENUM;
        double y12 = (y1 + y2) / DOUBLENUM;
        double x23 = (x2 + x3) / DOUBLENUM;
        double y23 = (y2 + y3) / DOUBLENUM;
        double x123 = (x12 + x23) / DOUBLENUM;
        double y123 = (y12 + y23) / DOUBLENUM;

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
                    da = TWO_TIMES * PI - da;
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
                    // 简单共线情况，1---2---3
                    // 我们可以只留下两个端点
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

    void QuadraticBezierCurveDividOperate::Bezier(double x1, double y1,
                           double x2, double y2,
                           double x3, double y3)
    {
        points_.Add(PointD(x1, y1));
        RecursiveBezier(x1, y1, x2, y2, x3, y3, 0);
        points_.Add(PointD(x3, y3));
    }

    void CubicBezierCurveIncrement::ApproximationScale(double scale)
    {
        approximationScale = scale;
    }

    double CubicBezierCurveIncrement::ApproximationScale() const
    {
        return approximationScale;
    }

    void CubicBezierCurveIncrement::Init(double x1, double y1,
                         double x2, double y2,
                         double x3, double y3,
                         double x4, double y4)
    {
        startXCoordinate = x1;
        startYCoordinate = y1;
        endXCoordinate = x4;
        endYCoordinate = y4;

        double deltaX1 = x2 - x1;
        double deltaY1 = y2 - y1;
        double deltaX2 = x3 - x2;
        double deltaY2 = y3 - y2;
        double deltaX3 = x4 - x3;
        double deltaY3 = y4 - y3;

        double len = (std::sqrt(deltaX1 * deltaX1 + deltaY1 * deltaY1) +
                      std::sqrt(deltaX2 * deltaX2 + deltaY2 * deltaY2) +
                      std::sqrt(deltaX3 * deltaX3 + deltaY3 * deltaY3)) *
                     CURVES_NUM_STEP_LEN * approximationScale;

        numberSteps = Uround(len);
        const int cuvereNumStep = 4;
        if (numberSteps < cuvereNumStep) {
            numberSteps = cuvereNumStep;
        }

        double subdivideStep = 1.0 / numberSteps;
        double subdivideStep2 = subdivideStep * subdivideStep;
        double subdivideStep3 = subdivideStep * subdivideStep * subdivideStep;
        const double PrelMin = 3.0;
        const double PrelMax = 6.0;

        double pre1 = PrelMin * subdivideStep;
        double pre2 = PrelMin * subdivideStep2;
        double pre4 = PrelMax * subdivideStep2;
        double pre5 = PrelMax * subdivideStep3;

        double tmp1X = x1 - x2 * DOUBLENUM + x3;
        double tmp1Y = y1 - y2 * DOUBLENUM + y3;

        double tmp2X = (x2 - x3) * PrelMin - x1 + x4;
        double tmp2Y = (y2 - y3) * PrelMin - y1 + y4;

        savedFinalXCoordinate = x1;
        finalXCoordinate = x1;
        savedFinalYCoordinate = y1;
        finalYCoordinate = y1;

        savedDeltaFinalXCoordinate = (x2 - x1) * pre1 + tmp1X * pre2 + tmp2X * subdivideStep3;
        deltaFinalXCoordinate = (x2 - x1) * pre1 + tmp1X * pre2 + tmp2X * subdivideStep3;
        savedDeltaFinalYCoordinate = (y2 - y1) * pre1 + tmp1Y * pre2 + tmp2Y * subdivideStep3;
        deltaFinalYCoordinate = (y2 - y1) * pre1 + tmp1Y * pre2 + tmp2Y * subdivideStep3;

        savedDoubleDeltaFinalXCoordinate = tmp1X * pre4 + tmp2X * pre5;
        doubleDeltaFinalXCoordinate = tmp1X * pre4 + tmp2X * pre5;
        savedDoubleDeltaFinalYCoordinate = tmp1Y * pre4 + tmp2Y * pre5;
        doubleDeltaFinalYCoordinate = tmp1Y * pre4 + tmp2Y * pre5;

        doubleDoubleDeltaFinalXCoordinate = tmp2X * pre5;
        doubleDoubleDeltaFinalYCoordinate = tmp2Y * pre5;

        currentStep = numberSteps;
    }

    void CubicBezierCurveIncrement::Rewind(unsigned)
    {
        if (numberSteps == 0) {
            currentStep = -1;
            return;
        }

        currentStep = numberSteps;
        finalXCoordinate = savedFinalXCoordinate;
        finalYCoordinate = savedFinalYCoordinate;
        deltaFinalXCoordinate = savedDeltaFinalXCoordinate;
        deltaFinalYCoordinate = savedDeltaFinalYCoordinate;
        doubleDeltaFinalXCoordinate = savedDoubleDeltaFinalXCoordinate;
        doubleDeltaFinalYCoordinate = savedDoubleDeltaFinalYCoordinate;
    }

    unsigned CubicBezierCurveIncrement::Vertex(double* x, double* y)
    {
        if (currentStep < 0) {
            return PATH_CMD_STOP;
        }
        if (currentStep == numberSteps) {
            *x = startXCoordinate;
            *y = startYCoordinate;
            --currentStep;
            return PATH_CMD_MOVE_TO;
        }

        if (currentStep == 0) {
            *x = endXCoordinate;
            *y = endYCoordinate;
            --currentStep;
            return PATH_CMD_LINE_TO;
        }

        finalXCoordinate += deltaFinalXCoordinate;
        finalYCoordinate += deltaFinalYCoordinate;
        deltaFinalXCoordinate += doubleDeltaFinalXCoordinate;
        deltaFinalYCoordinate += doubleDeltaFinalYCoordinate;
        doubleDeltaFinalXCoordinate += doubleDoubleDeltaFinalXCoordinate;
        doubleDeltaFinalYCoordinate += doubleDoubleDeltaFinalYCoordinate;

        *x = finalXCoordinate;
        *y = finalYCoordinate;
        --currentStep;
        return PATH_CMD_LINE_TO;
    }

    void CubicBezierCurveDividOperate::Init(double x1, double y1,
                         double x2, double y2,
                         double x3, double y3,
                         double x4, double y4)
    {
        points_.RemoveAll();
        distanceToleranceSquare_ = HALFNUM / approximationScale_;
        distanceToleranceSquare_ *= distanceToleranceSquare_;
        Bezier(x1, y1, x2, y2, x3, y3, x4, y4);
        count_ = 0;
    }

    void CubicBezierCurveDividOperate::RecursiveBezier(double x1, double y1,
                                    double x2, double y2,
                                    double x3, double y3,
                                    double x4, double y4,
                                    unsigned level)
    {
        if (level > CURVERECURSIONLIMIT) {
            return;
        }

        // 计算线段的所有中点
        double x12 = (x1 + x2) / DOUBLENUM;
        double y12 = (y1 + y2) / DOUBLENUM;
        double x23 = (x2 + x3) / DOUBLENUM;
        double y23 = (y2 + y3) / DOUBLENUM;
        double x34 = (x3 + x4) / DOUBLENUM;
        double y34 = (y3 + y4) / DOUBLENUM;
        double x123 = (x12 + x23) / DOUBLENUM;
        double y123 = (y12 + y23) / DOUBLENUM;
        double x234 = (x23 + x34) / DOUBLENUM;
        double y234 = (y23 + y34) / DOUBLENUM;
        double x1234 = (x123 + x234) / DOUBLENUM;
        double y1234 = (y123 + y234) / DOUBLENUM;

        // 试着用一条直线近似整个三次曲线
        double deltaX = x4 - x1;
        double deltaY = y4 - y1;

        double delta2 = std::fabs(((x2 - x4) * deltaY - (y2 - y4) * deltaX));
        double delta3 = std::fabs(((x3 - x4) * deltaY - (y3 - y4) * deltaX));

        double da1;
        double da2;
        double gradient;

        switch ((int(delta2 > CURVECOLLINEARITYEPSILON) << 1) + int(delta3 > CURVECOLLINEARITYEPSILON)) {
            case COLLINEAR:
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
                        // 简单共线情况，1--2--3--4
                        // 我们可以只留下两个端点
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

            case COLLINEAR1:
                // p1、p2、p4是共线的
                if (delta3 * delta3 <= distanceToleranceSquare_ * (deltaX * deltaX + deltaY * deltaY)) {
                    if (angleTolerance_ < CURVEANGLETOLERANCEEPSILON) {
                        points_.Add(PointD(x23, y23));
                        return;
                    }

                    // 角度条件
                    da1 = std::fabs(std::atan2(y4 - y3, x4 - x3) - std::atan2(y3 - y2, x3 - x2));
                    if (da1 >= PI) {
                        da1 = DOUBLENUM * PI - da1;
                    }

                    if (da1 < angleTolerance_) {
                        points_.Add(PointD(x2, y2));
                        points_.Add(PointD(x3, y3));
                        return;
                    }

                    if (cuspLimit_ != 0.0 && da1 > cuspLimit_) {
                        points_.Add(PointD(x3, y3));
                        return;
                    }
                }
                break;

            case COLLINEAR2:
                // p1、p3、p4是共线的
                if (delta2 * delta2 <= distanceToleranceSquare_ * (deltaX * deltaX + deltaY * deltaY)) {
                    if (angleTolerance_ < CURVEANGLETOLERANCEEPSILON) {
                        points_.Add(PointD(x23, y23));
                        return;
                    }

                    // 角度条件
                    da1 = std::fabs(std::atan2(y3 - y2, x3 - x2) - std::atan2(y2 - y1, x2 - x1));
                    if (da1 >= PI) {
                        da1 = DOUBLENUM * PI - da1;
                    }

                    if (da1 < angleTolerance_) {
                        points_.Add(PointD(x2, y2));
                        points_.Add(PointD(x3, y3));
                        return;
                    }

                    if (cuspLimit_ != 0.0 && da1 > cuspLimit_) {
                        points_.Add(PointD(x2, y2));
                        return;
                    }
                }
                break;

            case COLLINEAR3:
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
                        da1 = DOUBLENUM * PI - da1;
                    }
                    if (da2 >= PI) {
                        da2 = DOUBLENUM * PI - da2;
                    }

                    if (da1 + da2 < angleTolerance_) {
                        points_.Add(PointD(x23, y23));
                        return;
                    }

                    if (cuspLimit_ != 0.0 && da1 > cuspLimit_) {
                        points_.Add(PointD(x2, y2));
                        return;
                    }

                    if (cuspLimit_ != 0.0 && da2 > cuspLimit_) {
                        points_.Add(PointD(x3, y3));
                        return;
                    }
                }
                break;
        }

        RecursiveBezier(x1, y1, x12, y12, x123, y123, x1234, y1234, level + 1);
        RecursiveBezier(x1234, y1234, x234, y234, x34, y34, x4, y4, level + 1);
    }

    void CubicBezierCurveDividOperate::Bezier(double x1, double y1,
                           double x2, double y2,
                           double x3, double y3,
                           double x4, double y4)
    {
        points_.Add(PointD(x1, y1));
        RecursiveBezier(x1, y1, x2, y2, x3, y3, x4, y4, 0);
        points_.Add(PointD(x4, y4));
    }
} // namespace OHOS
