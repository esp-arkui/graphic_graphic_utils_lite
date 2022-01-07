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

#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_curves.h"

#include "gfx_utils/graphics/common/graphic_common_math.h"

namespace OHOS {
    const float CURVECOLLINEARITYEPSILON = 1e-30;
    const float CURVEANGLETOLERANCEEPSILON = 0.01f;
    const int CURVERECURSIONLIMIT = 32;
    const float CURVES_NUM_STEP_LEN = 0.25f;
    float QuadraticBezierCurveIncrement::ApproximationScale() const
    {
        return approximationScale;
    }

    void QuadraticBezierCurveIncrement::ApproximationScale(float scale)
    {
        approximationScale = scale;
    }

    void QuadraticBezierCurveIncrement::Init(float x1, float y1,
                                             float x2, float y2,
                                             float x3, float y3)
    {
        startXCoordinate = x1;
        startYCoordinate = y1;
        endXCoordinate = x3;
        endYCoordinate = y3;

        float deltaX1 = x2 - x1;
        float deltaY1 = y2 - y1;
        float deltaX2 = x3 - x2;
        float deltaY2 = y3 - y2;

        float len = std::sqrt(deltaX1 * deltaX1 + deltaY1 * deltaY1) +
                     std::sqrt(deltaX2 * deltaX2 + deltaY2 * deltaY2);

        numberSteps = Uround(len * CURVES_NUM_STEP_LEN * approximationScale);
        const int NUM_STEPS_MAX = 4;
        if (numberSteps < NUM_STEPS_MAX) {
            numberSteps = NUM_STEPS_MAX;
        }
        if(numberSteps == 0) {
            return;
        }
        float subdivideStep = 1.0f / numberSteps;
        float subdivideStep2 = subdivideStep * subdivideStep;

        float tmpX = (x1 - x2 * FLOATNUM + x3) * subdivideStep2;
        float tmpY = (y1 - y2 * FLOATNUM + y3) * subdivideStep2;

        savedFinalXCoordinate = x1;
        finalXCoordinate = x1;
        savedFinalYCoordinate = y1;
        finalYCoordinate = y1;

        savedDeltaFinalXCoordinate = tmpX + (x2 - x1) * (FLOATNUM * subdivideStep);
        deltaFinalXCoordinate = tmpY + (x2 - x1) * (FLOATNUM * subdivideStep);
        savedDeltaFinalYCoordinate = tmpY + (y2 - y1) * (FLOATNUM * subdivideStep);
        deltaFinalYCoordinate = tmpY + (y2 - y1) * (FLOATNUM * subdivideStep);

        floatDeltaFinalXCoordinate = tmpX * FLOATNUM;
        floatDeltaFinalYCoordinate = tmpY * FLOATNUM;

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

    unsigned QuadraticBezierCurveIncrement::Vertex(float* x, float* y)
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
        deltaFinalXCoordinate += floatDeltaFinalXCoordinate;
        deltaFinalYCoordinate += floatDeltaFinalYCoordinate;
        *x = finalXCoordinate;
        *y = finalYCoordinate;
        --currentStep;
        return PATH_CMD_LINE_TO;
    }

    void QuadraticBezierCurveDividOperate::Init(float x1, float y1,
                                                float x2, float y2,
                                                float x3, float y3)
    {
        points_.RemoveAll();
        distanceToleranceSquare_ = HALFNUM / approximationScale_;
        distanceToleranceSquare_ *= distanceToleranceSquare_;
        Bezier(x1, y1, x2, y2, x3, y3);
        count_ = 0;
    }

    void QuadraticBezierCurveDividOperate::RecursiveBezier(float x1, float y1,
                                                           float x2, float y2,
                                                           float x3, float y3,
                                                           unsigned level)
    {
        if (level > CURVERECURSIONLIMIT) {
            return;
        }

        // 计算线段的所有中点
        float x12 = (x1 + x2) / FLOATNUM;
        float y12 = (y1 + y2) / FLOATNUM;
        float x23 = (x2 + x3) / FLOATNUM;
        float y23 = (y2 + y3) / FLOATNUM;
        float x123 = (x12 + x23) / FLOATNUM;
        float y123 = (y12 + y23) / FLOATNUM;

        float deltaX = x3 - x1;
        float deltaY = y3 - y1;
        float distance = std::fabs(((x2 - x3) * deltaY - (y2 - y3) * deltaX));
        float da;

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

    void QuadraticBezierCurveDividOperate::Bezier(float x1, float y1,
                                                  float x2, float y2,
                                                  float x3, float y3)
    {
        points_.Add(PointD(x1, y1));
        RecursiveBezier(x1, y1, x2, y2, x3, y3, 0);
        points_.Add(PointD(x3, y3));
    }

    void CubicBezierCurveIncrement::ApproximationScale(float scale)
    {
        approximationScale = scale;
    }

    float CubicBezierCurveIncrement::ApproximationScale() const
    {
        return approximationScale;
    }

    void CubicBezierCurveIncrement::Init(float x1, float y1,
                                         float x2, float y2,
                                         float x3, float y3,
                                         float x4, float y4)
    {
        startXCoordinate = x1;
        startYCoordinate = y1;
        endXCoordinate = x4;
        endYCoordinate = y4;

        float deltaX1 = x2 - x1;
        float deltaY1 = y2 - y1;
        float deltaX2 = x3 - x2;
        float deltaY2 = y3 - y2;
        float deltaX3 = x4 - x3;
        float deltaY3 = y4 - y3;

        float len = (std::sqrt(deltaX1 * deltaX1 + deltaY1 * deltaY1) +
                      std::sqrt(deltaX2 * deltaX2 + deltaY2 * deltaY2) +
                      std::sqrt(deltaX3 * deltaX3 + deltaY3 * deltaY3)) *
                     CURVES_NUM_STEP_LEN * approximationScale;

        numberSteps = Uround(len);
        const int cuvereNumStep = 4;
        if (numberSteps < cuvereNumStep) {
            numberSteps = cuvereNumStep;
        }
        if(numberSteps == 0) {
            return;
        }
        float subdivideStep = 1.0 / numberSteps;
        float subdivideStep2 = subdivideStep * subdivideStep;
        float subdivideStep3 = subdivideStep * subdivideStep * subdivideStep;
        const float PrelMin = 3.0;
        const float PrelMax = 6.0;

        float pre1 = PrelMin * subdivideStep;
        float pre2 = PrelMin * subdivideStep2;
        float pre4 = PrelMax * subdivideStep2;
        float pre5 = PrelMax * subdivideStep3;

        float tmp1X = x1 - x2 * FLOATNUM + x3;
        float tmp1Y = y1 - y2 * FLOATNUM + y3;

        float tmp2X = (x2 - x3) * PrelMin - x1 + x4;
        float tmp2Y = (y2 - y3) * PrelMin - y1 + y4;

        savedFinalXCoordinate = x1;
        finalXCoordinate = x1;
        savedFinalYCoordinate = y1;
        finalYCoordinate = y1;

        savedDeltaFinalXCoordinate = (x2 - x1) * pre1 + tmp1X * pre2 + tmp2X * subdivideStep3;
        deltaFinalXCoordinate = (x2 - x1) * pre1 + tmp1X * pre2 + tmp2X * subdivideStep3;
        savedDeltaFinalYCoordinate = (y2 - y1) * pre1 + tmp1Y * pre2 + tmp2Y * subdivideStep3;
        deltaFinalYCoordinate = (y2 - y1) * pre1 + tmp1Y * pre2 + tmp2Y * subdivideStep3;

        savedfloatDeltaFinalXCoordinate = tmp1X * pre4 + tmp2X * pre5;
        floatDeltaFinalXCoordinate = tmp1X * pre4 + tmp2X * pre5;
        savedfloatDeltaFinalYCoordinate = tmp1Y * pre4 + tmp2Y * pre5;
        floatDeltaFinalYCoordinate = tmp1Y * pre4 + tmp2Y * pre5;

        floatfloatDeltaFinalXCoordinate = tmp2X * pre5;
        floatfloatDeltaFinalYCoordinate = tmp2Y * pre5;

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
        floatDeltaFinalXCoordinate = savedfloatDeltaFinalXCoordinate;
        floatDeltaFinalYCoordinate = savedfloatDeltaFinalYCoordinate;
    }

    unsigned CubicBezierCurveIncrement::Vertex(float* x, float* y)
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
        deltaFinalXCoordinate += floatDeltaFinalXCoordinate;
        deltaFinalYCoordinate += floatDeltaFinalYCoordinate;
        floatDeltaFinalXCoordinate += floatfloatDeltaFinalXCoordinate;
        floatDeltaFinalYCoordinate += floatfloatDeltaFinalYCoordinate;

        *x = finalXCoordinate;
        *y = finalYCoordinate;
        --currentStep;
        return PATH_CMD_LINE_TO;
    }

    void CubicBezierCurveDividOperate::Init(float x1, float y1,
                                            float x2, float y2,
                                            float x3, float y3,
                                            float x4, float y4)
    {
        points_.RemoveAll();
        distanceToleranceSquare_ = HALFNUM / approximationScale_;
        distanceToleranceSquare_ *= distanceToleranceSquare_;
        Bezier(x1, y1, x2, y2, x3, y3, x4, y4);
        count_ = 0;
    }

    void CubicBezierCurveDividOperate::RecursiveBezier(float x1, float y1,
                                                       float x2, float y2,
                                                       float x3, float y3,
                                                       float x4, float y4,
                                                       unsigned level)
    {
        if (level > CURVERECURSIONLIMIT) {
            return;
        }

        // 计算线段的所有中点
        float x12 = (x1 + x2) / FLOATNUM;
        float y12 = (y1 + y2) / FLOATNUM;
        float x23 = (x2 + x3) / FLOATNUM;
        float y23 = (y2 + y3) / FLOATNUM;
        float x34 = (x3 + x4) / FLOATNUM;
        float y34 = (y3 + y4) / FLOATNUM;
        float x123 = (x12 + x23) / FLOATNUM;
        float y123 = (y12 + y23) / FLOATNUM;
        float x234 = (x23 + x34) / FLOATNUM;
        float y234 = (y23 + y34) / FLOATNUM;
        float x1234 = (x123 + x234) / FLOATNUM;
        float y1234 = (y123 + y234) / FLOATNUM;

        // 试着用一条直线近似整个三次曲线
        float deltaX = x4 - x1;
        float deltaY = y4 - y1;

        float delta2 = std::fabs(((x2 - x4) * deltaY - (y2 - y4) * deltaX));
        float delta3 = std::fabs(((x3 - x4) * deltaY - (y3 - y4) * deltaX));

        float da1;
        float da2;
        float gradient;

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
                        da1 = FLOATNUM * PI - da1;
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
                        da1 = FLOATNUM * PI - da1;
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
                if ((delta2 + delta3) * (delta2 + delta3) <=
                        distanceToleranceSquare_ * (deltaX * deltaX + deltaY * deltaY)) {
                    // 如果曲率未超过距离公差值
                    if (angleTolerance_ < CURVEANGLETOLERANCEEPSILON) {
                        points_.Add(PointD(x23, y23));
                        return;
                    }

                    gradient = std::atan2(y3 - y2, x3 - x2);
                    da1 = std::fabs(gradient - std::atan2(y2 - y1, x2 - x1));
                    da2 = std::fabs(std::atan2(y4 - y3, x4 - x3) - gradient);
                    if (da1 >= PI) {
                        da1 = FLOATNUM * PI - da1;
                    }
                    if (da2 >= PI) {
                        da2 = FLOATNUM * PI - da2;
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

    void CubicBezierCurveDividOperate::Bezier(float x1, float y1,
                                              float x2, float y2,
                                              float x3, float y3,
                                              float x4, float y4)
    {
        points_.Add(PointD(x1, y1));
        RecursiveBezier(x1, y1, x2, y2, x3, y3, x4, y4, 0);
        points_.Add(PointD(x4, y4));
    }
} // namespace OHOS
