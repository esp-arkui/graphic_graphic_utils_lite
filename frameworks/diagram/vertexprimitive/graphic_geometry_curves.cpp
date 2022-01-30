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

#include "gfx_utils/diagram/vertexprimitive/graphic_geometry_curves.h"

#include "gfx_utils/diagram/common/graphic_common_math.h"
#include "draw/clip_utils.h"
namespace OHOS {
#define CALC_BEZIER_POINT \
    /** Calculates All Midpoints Of a Segment */ \
    float x12 = (x1 + x2) / FLOATNUM; \
    float y12 = (y1 + y2) / FLOATNUM; \
    float x23 = (x2 + x3) / FLOATNUM; \
    float y23 = (y2 + y3) / FLOATNUM; \
    float x34 = (x3 + x4) / FLOATNUM; \
    float y34 = (y3 + y4) / FLOATNUM; \
    float x123 = (x12 + x23) / FLOATNUM; \
    float y123 = (y12 + y23) / FLOATNUM; \
    float x234 = (x23 + x34) / FLOATNUM; \
    float y234 = (y23 + y34) / FLOATNUM; \
    float x1234 = (x123 + x234) / FLOATNUM; \
    float y1234 = (y123 + y234) / FLOATNUM; \
    /** Try to Approximate the Entire Cubic Curve With a Straight Line */  \
    float delta41X = x4 - x1; \
    float delta41Y = y4 - y1; \
    float delta2 = std::fabs(((x2 - x4) * delta41Y - (y2 - y4) * delta41X)); \
    float delta3 = std::fabs(((x3 - x4) * delta41Y - (y3 - y4) * delta41X)); \
    float dx21Or31; \
    float dy21Or31; \
    float delta41

#define CALC_BEZIER_POINT_COLLINEAR \
    delta41 = 1 / delta41; \
    dx21Or31 = x2 - x1; \
    dy21Or31 = y2 - y1; \
    delta2 = delta41 * (dx21Or31 * delta41X + dy21Or31 * delta41Y); \
    dx21Or31 = x3 - x1; \
    dy21Or31 = y3 - y1; \
    delta3 = delta41 * (dx21Or31 * delta41X + dy21Or31 * delta41Y)

#define CALC_BEZIER_POINT_COLLINEAR_SUB \
    if (delta2 <= 0) { \
        delta2 = CalcSqDistance(x2, y2, x1, y1); \
    } else if (delta2 >= 1) { \
        delta2 = CalcSqDistance(x2, y2, x4, y4); \
    } else { \
        delta2 = CalcSqDistance(x2, y2, x1 + delta2 * delta41X, y1 + delta2 * delta41Y); \
    } \
    if (delta3 <= 0) { \
        delta3 = CalcSqDistance(x3, y3, x1, y1); \
    } else if (delta3 >= 1) { \
        delta3 = CalcSqDistance(x3, y3, x4, y4); \
    } else { \
        delta3 = CalcSqDistance(x3, y3, x1 + delta3 * delta41X, y1 + delta3 * delta41Y); \
    }

// Calculates All Midpoints Of a Segment
#define RECURSIVE_BEZIER_VALUE \
    float x12 = (x1 + x2) / FLOATNUM; \
    float y12 = (y1 + y2) / FLOATNUM; \
    float x23 = (x2 + x3) / FLOATNUM; \
    float y23 = (y2 + y3) / FLOATNUM; \
    float x123 = (x12 + x23) / FLOATNUM; \
    float y123 = (y12 + y23) / FLOATNUM; \
    float deltaX = x3 - x1; \
    float deltaY = y3 - y1; \
    float distance = std::fabs(((x2 - x3) * deltaY - (y2 - y3) * deltaX)); \
    float da

#define RECURSIVE_BEZIER_COLLINEAR \
    if (delta2 > delta3) { \
        if (delta2 < distanceToleranceSquare_) { \
            points_.Add(PointF(x2, y2)); \
            isEnabled = false; \
        } \
    } else { \
        if (delta3 < distanceToleranceSquare_) { \
            points_.Add(PointF(x3, y3)); \
            isEnabled = false; \
        } \
    }

#define RECURSIVE_BEZIER_COLLINEAR1 \
    if (delta3 * delta3 <= distanceToleranceSquare_ * (delta41X * delta41X + delta41Y * delta41Y)) { \
        if (angleTolerance_ < CURVE_ANGLE_TO_LERANCE_EPSILON) { \
            points_.Add(PointF(x23, y23)); \
            isEnabled = false; \
        } \
        if (isEnabled) { \
            dx21Or31 = std::fabs(std::atan2(y4 - y3, x4 - x3) - std::atan2(y3 - y2, x3 - x2)); \
            if (dx21Or31 >= PI) { \
                dx21Or31 = FLOATNUM * PI - dx21Or31; \
            } \
            if (dx21Or31 < angleTolerance_) { \
                points_.Add(PointF(x2, y2)); \
                points_.Add(PointF(x3, y3)); \
                isEnabled = false; \
            } \
            if (isEnabled && cuspLimit_ != 0.0f && dx21Or31 > cuspLimit_) { \
                points_.Add(PointF(x3, y3)); \
                isEnabled = false; \
            } \
        } \
    }

#define RECURSIVE_BEZIER_COLLINEAR2 \
    if (delta2 * delta2 <= distanceToleranceSquare_ * (delta41X * delta41X + delta41Y * delta41Y)) { \
        if (angleTolerance_ < CURVE_ANGLE_TO_LERANCE_EPSILON) { \
            points_.Add(PointF(x23, y23)); \
            isEnabled = false; \
        } \
        if (isEnabled) { \
            dx21Or31 = std::fabs(std::atan2(y3 - y2, x3 - x2) - std::atan2(y2 - y1, x2 - x1)); \
            if (dx21Or31 >= PI) { \
                dx21Or31 = FLOATNUM * PI - dx21Or31; \
            } \
            if (dx21Or31 < angleTolerance_) { \
                points_.Add(PointF(x2, y2)); \
                points_.Add(PointF(x3, y3)); \
                isEnabled = false; \
            } \
            if (isEnabled && cuspLimit_ != 0.0f && dx21Or31 > cuspLimit_) { \
                points_.Add(PointF(x2, y2)); \
                isEnabled = false; \
            } \
        } \
    }

#define RECURSIVE_BEZIER_COLLINEAR3 \
    if ((delta2 + delta3) * (delta2 + delta3) <= \
            distanceToleranceSquare_ * (delta41X * delta41X + delta41Y * delta41Y)) { \
        if (angleTolerance_ < CURVE_ANGLE_TO_LERANCE_EPSILON) { \
            points_.Add(PointF(x23, y23)); \
            isEnabled = false; \
        } \
        delta41 = std::atan2(y3 - y2, x3 - x2); \
        dx21Or31 = std::fabs(delta41 - std::atan2(y2 - y1, x2 - x1)); \
        dy21Or31 = std::fabs(std::atan2(y4 - y3, x4 - x3) - delta41); \
        if (dx21Or31 >= PI) { \
            dx21Or31 = FLOATNUM * PI - dx21Or31; \
        } \
        if (dy21Or31 >= PI) { \
            dy21Or31 = FLOATNUM * PI - dy21Or31; \
        } \
        if (isEnabled) { \
            if (dx21Or31 + dy21Or31 < angleTolerance_) { \
                points_.Add(PointF(x23, y23)); \
                isEnabled = false; \
            } \
            if (isEnabled && cuspLimit_ != 0.0f && dx21Or31 > cuspLimit_) { \
                points_.Add(PointF(x2, y2)); \
                isEnabled = false; \
            } \
            if (isEnabled && cuspLimit_ != 0.0f && dy21Or31 > cuspLimit_) { \
                points_.Add(PointF(x3, y3)); \
                isEnabled = false; \
            } \
        } \
    }

const float CURVE_COLLINEARITY_EPSILON = 1e-30;
const float CURVE_ANGLE_TO_LERANCE_EPSILON = 0.01f;
const int CURVE_RECURSION_LIMIT = 32;
const float CURVES_NUM_STEP_LEN = 0.25f;
float QuadraticBezierCurveIncrement::ApproximationScale() const
{
    return approximationScale_;
}

void QuadraticBezierCurveIncrement::ApproximationScale(float scale)
{
    approximationScale_ = scale;
}

void QuadraticBezierCurveIncrement::Init(float x1, float y1,
                                         float x2, float y2,
                                         float x3, float y3)
{
    startXCoordinate_ = x1;
    startYCoordinate_ = y1;
    endXCoordinate_ = x3;
    endYCoordinate_ = y3;

    float deltaX1 = x2 - x1;
    float deltaY1 = y2 - y1;
    float deltaX2 = x3 - x2;
    float deltaY2 = y3 - y2;

    float len = std::sqrt(deltaX1 * deltaX1 + deltaY1 * deltaY1) +
                 std::sqrt(deltaX2 * deltaX2 + deltaY2 * deltaY2);

    numberSteps_ = MATH_UROUND(len * CURVES_NUM_STEP_LEN * approximationScale_);
    const int numStepsMax = 4;
    if (numberSteps_ < numStepsMax) {
        numberSteps_ = numStepsMax;
    }
    if (numberSteps_ == 0) {
        return;
    }
    float subdivideStep = 1.0f / numberSteps_;
    float subdivideStep2 = subdivideStep * subdivideStep;

    float tmpX = (x1 - x2 * FLOATNUM + x3) * subdivideStep2;
    float tmpY = (y1 - y2 * FLOATNUM + y3) * subdivideStep2;

    savedFinalXCoordinate_ = x1;
    finalXCoordinate_ = x1;
    savedFinalYCoordinate_ = y1;
    finalYCoordinate_ = y1;

    savedDeltaFinalXCoordinate_ = tmpX + (x2 - x1) * (FLOATNUM * subdivideStep);
    deltaFinalXCoordinate_ = tmpX + (x2 - x1) * (FLOATNUM * subdivideStep);
    savedDeltaFinalYCoordinate_ = tmpY + (y2 - y1) * (FLOATNUM * subdivideStep);
    deltaFinalYCoordinate_ = tmpY + (y2 - y1) * (FLOATNUM * subdivideStep);

    copyDeltaFinalXCoordinate_ = tmpX * FLOATNUM;
    copyDeltaFinalYCoordinate_ = tmpY * FLOATNUM;

    currentStep_ = numberSteps_;
}

void QuadraticBezierCurveIncrement::Rewind(unsigned)
{
    if (numberSteps_ == 0) {
        currentStep_ = -1;
        return;
    }

    currentStep_ = numberSteps_;
    finalXCoordinate_ = savedFinalXCoordinate_;
    finalYCoordinate_ = savedFinalYCoordinate_;
    deltaFinalXCoordinate_ = savedDeltaFinalXCoordinate_;
    deltaFinalYCoordinate_ = savedDeltaFinalYCoordinate_;
}

unsigned QuadraticBezierCurveIncrement::Vertex(float* x, float* y)
{
    if (currentStep_ < 0) {
        return PATH_CMD_STOP;
    }
    if (currentStep_ == numberSteps_) {
        *x = startXCoordinate_;
        *y = startYCoordinate_;
        --currentStep_;
        return PATH_CMD_MOVE_TO;
    }
    if (currentStep_ == 0) {
        *x = endXCoordinate_;
        *y = endYCoordinate_;
        --currentStep_;
        return PATH_CMD_LINE_TO;
    }

    finalXCoordinate_ += deltaFinalXCoordinate_;
    finalYCoordinate_ += deltaFinalYCoordinate_;
    deltaFinalXCoordinate_ += copyDeltaFinalXCoordinate_;
    deltaFinalYCoordinate_ += copyDeltaFinalYCoordinate_;
    *x = finalXCoordinate_;
    *y = finalYCoordinate_;
    --currentStep_;
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
    if (level > CURVE_RECURSION_LIMIT) {
        return;
    }
    // Calculates All Midpoints Of a Segment
    RECURSIVE_BEZIER_VALUE;
    if (distance > CURVE_COLLINEARITY_EPSILON) {
        if (distance * distance <= distanceToleranceSquare_ * (deltaX * deltaX + deltaY * deltaY)) {
            // If The Curvature Does Not Exceed The Distance Tolerance Value
            if (angleTolerance_ < CURVE_ANGLE_TO_LERANCE_EPSILON) {
                points_.Add(PointF(x123, y123));
                return;
            }

            da = std::fabs(std::atan2(y3 - y2, x3 - x2) - std::atan2(y2 - y1, x2 - x1));
            if (da >= PI) {
                da = TWO_TIMES * PI - da;
            }

            if (da < angleTolerance_) {
                points_.Add(PointF(x123, y123));
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
                // Simple Collinear Case，1---2---3
                // We Can Leave Only Two Endpoints
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
            points_.Add(PointF(x2, y2));
            return;
        }
    }

    // Continue Subdivision
    RecursiveBezier(x1, y1, x12, y12, x123, y123, level + 1);
    RecursiveBezier(x123, y123, x23, y23, x3, y3, level + 1);
}

void QuadraticBezierCurveDividOperate::Bezier(float x1, float y1,
                                              float x2, float y2,
                                              float x3, float y3)
{
    points_.Add(PointF(x1, y1));
    RecursiveBezier(x1, y1, x2, y2, x3, y3, 0);
    points_.Add(PointF(x3, y3));
}

void CubicBezierCurveIncrement::ApproximationScale(float scale)
{
    approximationScale_ = scale;
}

float CubicBezierCurveIncrement::ApproximationScale() const
{
    return approximationScale_;
}

void CubicBezierCurveIncrement::Init(float x1, float y1,
                                     float x2, float y2,
                                     float x3, float y3,
                                     float x4, float y4)
{
    startXCoordinate_ = x1;
    startYCoordinate_ = y1;
    endXCoordinate_ = x4;
    endYCoordinate_ = y4;

    float deltaX1 = x2 - x1;
    float deltaY1 = y2 - y1;
    float deltaX2 = x3 - x2;
    float deltaY2 = y3 - y2;
    float deltaX3 = x4 - x3;
    float deltaY3 = y4 - y3;

    float len = (std::sqrt(deltaX1 * deltaX1 + deltaY1 * deltaY1) +
                  std::sqrt(deltaX2 * deltaX2 + deltaY2 * deltaY2) +
                  std::sqrt(deltaX3 * deltaX3 + deltaY3 * deltaY3)) *
                 CURVES_NUM_STEP_LEN * approximationScale_;

    numberSteps_ = MATH_UROUND(len);
    const int cuvereNumStep = 4;
    if (numberSteps_ < cuvereNumStep) {
        numberSteps_ = cuvereNumStep;
    }
    if (numberSteps_ == 0) {
        return;
    }
    float subdivideStep = 1.0f / numberSteps_;
    float subdivideStep2 = subdivideStep * subdivideStep;
    float subdivideStep3 = subdivideStep * subdivideStep * subdivideStep;
    const float prelMin = 3.0f;
    const float prelMax = 6.0f;

    float pre1 = prelMin * subdivideStep;
    float pre2 = prelMin * subdivideStep2;
    float pre4 = prelMax * subdivideStep2;
    float pre5 = prelMax * subdivideStep3;

    float tmp1X = x1 - x2 * FLOATNUM + x3;
    float tmp1Y = y1 - y2 * FLOATNUM + y3;

    float tmp2X = (x2 - x3) * prelMin - x1 + x4;
    float tmp2Y = (y2 - y3) * prelMin - y1 + y4;

    savedFinalXCoordinate_ = x1;
    finalXCoordinate_ = x1;
    savedFinalYCoordinate_ = y1;
    finalYCoordinate_ = y1;

    savedDeltaFinalXCoordinate_ = (x2 - x1) * pre1 + tmp1X * pre2 + tmp2X * subdivideStep3;
    deltaFinalXCoordinate_ = (x2 - x1) * pre1 + tmp1X * pre2 + tmp2X * subdivideStep3;
    savedDeltaFinalYCoordinate_ = (y2 - y1) * pre1 + tmp1Y * pre2 + tmp2Y * subdivideStep3;
    deltaFinalYCoordinate_ = (y2 - y1) * pre1 + tmp1Y * pre2 + tmp2Y * subdivideStep3;

    savedCopyDeltaFinalXCoordinate_ = tmp1X * pre4 + tmp2X * pre5;
    copyDeltaFinalXCoordinate_ = tmp1X * pre4 + tmp2X * pre5;
    savedCopyDeltaFinalYCoordinate_ = tmp1Y * pre4 + tmp2Y * pre5;
    copyDeltaFinalYCoordinate_ = tmp1Y * pre4 + tmp2Y * pre5;

    repeatCopyDeltaFinalXCoordinate_ = tmp2X * pre5;
    repeatCopyDeltaFinalYCoordinate_ = tmp2Y * pre5;

    currentStep_ = numberSteps_;
}

void CubicBezierCurveIncrement::Rewind(unsigned)
{
    if (numberSteps_ == 0) {
        currentStep_ = -1;
        return;
    }

    currentStep_ = numberSteps_;
    finalXCoordinate_ = savedFinalXCoordinate_;
    finalYCoordinate_ = savedFinalYCoordinate_;
    deltaFinalXCoordinate_ = savedDeltaFinalXCoordinate_;
    deltaFinalYCoordinate_ = savedDeltaFinalYCoordinate_;
    copyDeltaFinalXCoordinate_ = savedCopyDeltaFinalXCoordinate_;
    copyDeltaFinalYCoordinate_ = savedCopyDeltaFinalYCoordinate_;
}

unsigned CubicBezierCurveIncrement::Vertex(float* x, float* y)
{
    if (currentStep_ < 0) {
        return PATH_CMD_STOP;
    }
    if (currentStep_ == numberSteps_) {
        *x = startXCoordinate_;
        *y = startYCoordinate_;
        --currentStep_;
        return PATH_CMD_MOVE_TO;
    }

    if (currentStep_ == 0) {
        *x = endXCoordinate_;
        *y = endYCoordinate_;
        --currentStep_;
        return PATH_CMD_LINE_TO;
    }

    finalXCoordinate_ += deltaFinalXCoordinate_;
    finalYCoordinate_ += deltaFinalYCoordinate_;
    deltaFinalXCoordinate_ += copyDeltaFinalXCoordinate_;
    deltaFinalYCoordinate_ += copyDeltaFinalYCoordinate_;
    copyDeltaFinalXCoordinate_ += repeatCopyDeltaFinalXCoordinate_;
    copyDeltaFinalYCoordinate_ += repeatCopyDeltaFinalYCoordinate_;

    *x = finalXCoordinate_;
    *y = finalYCoordinate_;
    --currentStep_;
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
    if (level > CURVE_RECURSION_LIMIT) {
        return;
    }
    if (level > CURVE_RECURSION_LIMIT) {
        return;
    }
    CALC_BEZIER_POINT;
    bool isEnabled = true;
    switch ((int(delta2 > CURVE_COLLINEARITY_EPSILON) << 1) + int(delta3 > CURVE_COLLINEARITY_EPSILON)) {
        case COLLINEAR:
            /** All are Collinear or p1 == p4 */
            delta41 = delta41X * delta41X + delta41Y * delta41Y;
            if (delta41 == 0) {
                delta2 = CalcSqDistance(x1, y1, x2, y2);
                delta3 = CalcSqDistance(x4, y4, x3, y3);
            } else {
                CALC_BEZIER_POINT_COLLINEAR;
                if (delta2 > 0 && delta2 < 1 && delta3 > 0 && delta3 < 1) {
                    /** Simple Collinear Case，1--2--3--4 */
                    /** We Can Leave Only Two Endpoints */
                    return;
                }
                CALC_BEZIER_POINT_COLLINEAR_SUB
            }
            RECURSIVE_BEZIER_COLLINEAR
            break;
        case COLLINEAR1:
            /** p1、p2、p4 is Collinear */
            RECURSIVE_BEZIER_COLLINEAR1
            break;
        case COLLINEAR2:
            /** p1、p3、p4 is Collinear. */
            RECURSIVE_BEZIER_COLLINEAR2
            break;
        case COLLINEAR3:
            RECURSIVE_BEZIER_COLLINEAR3
            break;
    }
    if (!isEnabled) {
        return;
    }
    RecursiveBezier(x1, y1, x12, y12, x123, y123, x1234, y1234, level + 1);
    RecursiveBezier(x1234, y1234, x234, y234, x34, y34, x4, y4, level + 1);
}

void CubicBezierCurveDividOperate::Bezier(float x1, float y1,
                                          float x2, float y2,
                                          float x3, float y3,
                                          float x4, float y4)
{
    points_.Add(PointF(x1, y1));
    RecursiveBezier(x1, y1, x2, y2, x3, y3, x4, y4, 0);
    points_.Add(PointF(x4, y4));
}
} // namespace OHOS
