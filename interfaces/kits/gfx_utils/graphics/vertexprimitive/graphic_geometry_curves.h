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
 * @file graphic_geometry_curves.h
 *
 * @brief 贝塞尔曲线
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_CURVES_INCLUDED
#define GRAPHIC_GEOMETRY_CURVES_INCLUDED

#include "graphic_geometry_plaindata_blockvector.h"

namespace OHOS {
    enum CurveApproximationMethodEnum {
        CURVEINCREMENT,
        CURVEDIVIDOPERATE
    };

    class QuadraticBezierCurveIncrement {
    public:
        QuadraticBezierCurveIncrement()
            : numberSteps(0), currentStep(0), approximationScale(1.0)
        {}

        QuadraticBezierCurveIncrement(float x1, float y1,
                                      float x2, float y2,
                                      float x3, float y3)
            : numberSteps(0),
            currentStep(0), approximationScale(1.0)
        {
            Init(x1, y1, x2, y2, x3, y3);
        }

        void Init(float x1, float y1,
                  float x2, float y2,
                  float x3, float y3);

        void Reset()
        {
            numberSteps = 0;
            currentStep = -1;
        }

        void ApproximationMethod(CurveApproximationMethodEnum)
        {}
        CurveApproximationMethodEnum ApproximationMethod() const
        {
            return CURVEINCREMENT;
        }

        void ApproximationScale(float scale);
        float ApproximationScale() const;

        void AngleTolerance(float)
        {}
        float AngleTolerance() const
        {
            return 0.0;
        }

        void CuspLimit(float)
        {}
        float CuspLimit() const
        {
            return 0.0;
        }

        void Rewind(unsigned pathId);
        unsigned Vertex(float* x, float* y);

    private:
        int numberSteps;
        int currentStep;
        float approximationScale;
        float startXCoordinate;
        float startYCoordinate;
        float endXCoordinate;
        float endYCoordinate;
        float finalXCoordinate;
        float finalYCoordinate;
        float deltaFinalXCoordinate;
        float deltaFinalYCoordinate;
        float floatDeltaFinalXCoordinate;
        float floatDeltaFinalYCoordinate;
        float savedFinalXCoordinate;
        float savedFinalYCoordinate;
        float savedDeltaFinalXCoordinate;
        float savedDeltaFinalYCoordinate;
    };

    class QuadraticBezierCurveDividOperate {
    public:
        QuadraticBezierCurveDividOperate()
            : approximationScale_(1.0), angleTolerance_(0.0), count_(0)
        {}

        QuadraticBezierCurveDividOperate(float x1, float y1,
                                         float x2, float y2,
                                         float x3, float y3)
            : approximationScale_(1.0),
            angleTolerance_(0.0), count_(0)
        {
            Init(x1, y1, x2, y2, x3, y3);
        }

        void Init(float x1, float y1, float x2, float y2, float x3, float y3);

        void Reset()
        {
            points_.RemoveAll();
            count_ = 0;
        }

        void ApproximationMethod(CurveApproximationMethodEnum)
        {}
        CurveApproximationMethodEnum ApproximationMethod() const
        {
            return CURVEDIVIDOPERATE;
        }

        void ApproximationScale(float scale)
        {
            approximationScale_ = scale;
        }
        float ApproximationScale() const
        {
            return approximationScale_;
        }

        void AngleTolerance(float angle)
        {
            angleTolerance_ = angle;
        }
        float AngleTolerance() const
        {
            return angleTolerance_;
        }

        void CuspLimit(float)
        {}
        float CuspLimit() const
        {
            return 0.0;
        }

        void Rewind(unsigned)
        {
            count_ = 0;
        }

        unsigned Vertex(float* x, float* y)
        {
            if (count_ >= points_.Size()) {
                return PATH_CMD_STOP;
            }
            const PointD& point = points_[count_++];
            *x = point.x;
            *y = point.y;
            return (count_ == 1) ? PATH_CMD_MOVE_TO : PATH_CMD_LINE_TO;
        }

    private:
        void Bezier(float x1, float y1,
                    float x2, float y2,
                    float x3, float y3);
        void RecursiveBezier(float x1, float y1,
                             float x2, float y2,
                             float x3, float y3,
                             unsigned level);

        float approximationScale_;
        float distanceToleranceSquare_;
        float angleTolerance_;
        unsigned count_;
        GeometryPlainDataBlockVector<PointD> points_;
    };

    struct CubicBezierCurvePoints {
        float cpArray[8];

        CubicBezierCurvePoints()
        {}
        CubicBezierCurvePoints(float x1, float y1,
                               float x2, float y2,
                               float x3, float y3,
                               float x4, float y4)
        {
            cpArray[OHOS::INDEX_ZERO] = x1;
            cpArray[OHOS::INDEX_ONE] = y1;
            cpArray[OHOS::INDEX_TWO] = x2;
            cpArray[OHOS::INDEX_THREE] = y2;
            cpArray[OHOS::INDEX_FOUR] = x3;
            cpArray[OHOS::INDEX_FIVE] = y3;
            cpArray[OHOS::INDEX_SIX] = x4;
            cpArray[OHOS::INDEX_SEVEN] = y4;
        }

        void Init(float x1, float y1,
                  float x2, float y2,
                  float x3, float y3,
                  float x4, float y4)
        {
            cpArray[OHOS::INDEX_ZERO] = x1;
            cpArray[OHOS::INDEX_ONE] = y1;
            cpArray[OHOS::INDEX_TWO] = x2;
            cpArray[OHOS::INDEX_THREE] = y2;
            cpArray[OHOS::INDEX_FOUR] = x3;
            cpArray[OHOS::INDEX_FIVE] = y3;
            cpArray[OHOS::INDEX_SIX] = x4;
            cpArray[OHOS::INDEX_SEVEN] = y4;
        }

        float operator[](unsigned i) const
        {
            return cpArray[i];
        }
        float& operator[](unsigned i)
        {
            return cpArray[i];
        }
    };

    class CubicBezierCurveIncrement {
    public:
        CubicBezierCurveIncrement()
            : numberSteps(0), currentStep(0), approximationScale(1.0)
        {}

        CubicBezierCurveIncrement(float x1, float y1, float x2, float y2,
                                  float x3, float y3, float x4, float y4)
            : numberSteps(0),
            currentStep(0), approximationScale(1.0)
        {
            Init(x1, y1, x2, y2, x3, y3, x4, y4);
        }

        CubicBezierCurveIncrement(const CubicBezierCurvePoints& curve4Points)
            : numberSteps(0), currentStep(0), approximationScale(1.0)
        {
            Init(curve4Points[0], curve4Points[1], curve4Points[2], curve4Points[3],
                 curve4Points[4], curve4Points[5], curve4Points[6], curve4Points[7]);
        }

        void Init(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
        void Init(const CubicBezierCurvePoints& curve4Points)
        {
            Init(curve4Points[0], curve4Points[1], curve4Points[2], curve4Points[3],
                 curve4Points[4], curve4Points[5], curve4Points[6], curve4Points[7]);
        }

        void Reset()
        {
            numberSteps = 0;
            currentStep = -1;
        }

        void ApproximationMethod(CurveApproximationMethodEnum)
        {}
        CurveApproximationMethodEnum ApproximationMethod() const
        {
            return CURVEINCREMENT;
        }

        void ApproximationScale(float scale);
        float ApproximationScale() const;

        void AngleTolerance(float)
        {}
        float AngleTolerance() const
        {
            return 0.0;
        }

        void CuspLimit(float)
        {}
        float CuspLimit() const
        {
            return 0.0;
        }

        void Rewind(unsigned pathId);
        unsigned Vertex(float* x, float* y);

    private:
        int numberSteps;
        int currentStep;
        float approximationScale;
        float startXCoordinate;
        float startYCoordinate;
        float endXCoordinate;
        float endYCoordinate;
        float finalXCoordinate;
        float finalYCoordinate;
        float deltaFinalXCoordinate;
        float deltaFinalYCoordinate;
        float floatDeltaFinalXCoordinate;
        float floatDeltaFinalYCoordinate;
        float floatfloatDeltaFinalXCoordinate;
        float floatfloatDeltaFinalYCoordinate;
        float savedFinalXCoordinate;
        float savedFinalYCoordinate;
        float savedDeltaFinalXCoordinate;
        float savedDeltaFinalYCoordinate;
        float savedfloatDeltaFinalXCoordinate;
        float savedfloatDeltaFinalYCoordinate;
    };

    inline CubicBezierCurvePoints CatromToBezier(float x1, float y1,
                                                 float x2, float y2,
                                                 float x3, float y3,
                                                 float x4, float y4)
    {
        return CubicBezierCurvePoints(
            x2,
            y2,
            (-x1 + OHOS::SIX_TIMES * x2 + x3) / OHOS::SIX_TIMES,
            (-y1 + OHOS::SIX_TIMES * y2 + y3) / OHOS::SIX_TIMES,
            (x2 + OHOS::SIX_TIMES * x3 - x4) / OHOS::SIX_TIMES,
            (y2 + OHOS::SIX_TIMES * y3 - y4) / OHOS::SIX_TIMES,
            x3,
            y3);
    }

    inline CubicBezierCurvePoints CatromToBezier(const CubicBezierCurvePoints& curve4Points)
    {
        return CatromToBezier(curve4Points[0], curve4Points[1], curve4Points[2], curve4Points[3],
                              curve4Points[4], curve4Points[5], curve4Points[6], curve4Points[7]);
    }
    /**
     * @brief bspline曲线转换为3次贝塞尔曲线
     * @param bspline曲线4个点，首尾和中间2个控制点
     * @since 1.0
     * @version 1.0
     */
    inline CubicBezierCurvePoints UbsplineToBezier(float x1, float y1,
                                                   float x2, float y2,
                                                   float x3, float y3,
                                                   float x4, float y4)
    {
        return CubicBezierCurvePoints(
            (x1 + OHOS::FOUR_TIMES * x2 + x3) / OHOS::SIX_TIMES,
            (y1 + OHOS::FOUR_TIMES * y2 + y3) / OHOS::SIX_TIMES,
            (OHOS::FOUR_TIMES * x2 + OHOS::TWO_TIMES * x3) / OHOS::SIX_TIMES,
            (OHOS::FOUR_TIMES * y2 + OHOS::TWO_TIMES * y3) / OHOS::SIX_TIMES,
            (OHOS::TWO_TIMES * x2 + OHOS::FOUR_TIMES * x3) / OHOS::SIX_TIMES,
            (OHOS::TWO_TIMES * y2 + OHOS::FOUR_TIMES * y3) / OHOS::SIX_TIMES,
            (x2 + OHOS::FOUR_TIMES * x3 + x4) / OHOS::SIX_TIMES,
            (y2 + OHOS::FOUR_TIMES * y3 + y4) / OHOS::SIX_TIMES);
    }
    /**
     * @brief bspline曲线转换为贝塞尔曲线
     * @param bspline曲线4个点，首尾和中间2个控制点
     * @since 1.0
     * @version 1.0
     */
    inline CubicBezierCurvePoints UbsplineToBezier(const CubicBezierCurvePoints& curve4Points)
    {
        return UbsplineToBezier(curve4Points[0], curve4Points[1], curve4Points[2], curve4Points[3],
                                curve4Points[4], curve4Points[5], curve4Points[6], curve4Points[7]);
    }
    /**
     * @brief Hermite曲线转换为3次数贝塞尔曲线
     * @param Hermite曲线2个点，首尾点
     * @since 1.0
     * @version 1.0
     */
    inline CubicBezierCurvePoints HermiteToBezier(float x1, float y1,
                                                  float x2, float y2,
                                                  float x3, float y3,
                                                  float x4, float y4)
    {
        return CubicBezierCurvePoints(
            x1, y1,
            (OHOS::THREE_TIMES * x1 + x3) / OHOS::THREE_TIMES,
            (OHOS::THREE_TIMES * y1 + y3) / OHOS::THREE_TIMES,
            (OHOS::THREE_TIMES * x2 - x4) / OHOS::THREE_TIMES,
            (OHOS::THREE_TIMES * y2 - y4) / OHOS::THREE_TIMES,
            x2, y2);
    }
    /**
     * @brief Hermite曲线转换为3次数贝塞尔曲线
     * @param Hermite曲线2个点，首尾点
     * @since 1.0
     * @version 1.0
     */
    inline CubicBezierCurvePoints HermiteToBezier(const CubicBezierCurvePoints& curve4Points)
    {
        return HermiteToBezier(curve4Points[OHOS::INDEX_ZERO], curve4Points[OHOS::INDEX_ONE],
                               curve4Points[OHOS::INDEX_TWO], curve4Points[OHOS::INDEX_THREE],
                               curve4Points[OHOS::INDEX_FOUR], curve4Points[OHOS::INDEX_FIVE],
                               curve4Points[OHOS::INDEX_SIX], curve4Points[OHOS::INDEX_SEVEN]);
    }

    class CubicBezierCurveDividOperate {
    public:
        CubicBezierCurveDividOperate()
            : approximationScale_(1.0), angleTolerance_(0.0), cuspLimit_(0.0), count_(0)
        {}

        CubicBezierCurveDividOperate(float x1, float y1,
                                     float x2, float y2,
                                     float x3, float y3,
                                     float x4, float y4)
            : approximationScale_(1.0), angleTolerance_(0.0), cuspLimit_(0.0), count_(0)
        {
            Init(x1, y1, x2, y2, x3, y3, x4, y4);
        }

        CubicBezierCurveDividOperate(const CubicBezierCurvePoints& curve4Points)
            : approximationScale_(1.0), angleTolerance_(0.0), count_(0)
        {
            Init(curve4Points[OHOS::INDEX_ZERO], curve4Points[OHOS::INDEX_ONE],
                 curve4Points[OHOS::INDEX_TWO], curve4Points[OHOS::INDEX_THREE],
                 curve4Points[OHOS::INDEX_FOUR], curve4Points[OHOS::INDEX_FIVE],
                 curve4Points[OHOS::INDEX_SIX], curve4Points[OHOS::INDEX_SEVEN]);
        }

        void Init(float x1, float y1,
                  float x2, float y2,
                  float x3, float y3,
                  float x4, float y4);

        void Init(const CubicBezierCurvePoints& curve4Points)
        {
            Init(curve4Points[OHOS::INDEX_ZERO], curve4Points[OHOS::INDEX_ONE],
                 curve4Points[OHOS::INDEX_TWO], curve4Points[OHOS::INDEX_THREE],
                 curve4Points[OHOS::INDEX_FOUR], curve4Points[OHOS::INDEX_FIVE],
                 curve4Points[OHOS::INDEX_SIX], curve4Points[OHOS::INDEX_SEVEN]);
        }

        void Reset()
        {
            points_.RemoveAll();
            count_ = 0;
        }

        void ApproximationMethod(CurveApproximationMethodEnum)
        {}

        CurveApproximationMethodEnum ApproximationMethod() const
        {
            return CURVEDIVIDOPERATE;
        }

        void ApproximationScale(float scale)
        {
            approximationScale_ = scale;
        }
        float ApproximationScale() const
        {
            return approximationScale_;
        }

        void AngleTolerance(float a)
        {
            angleTolerance_ = a;
        }
        float AngleTolerance() const
        {
            return angleTolerance_;
        }

        void CuspLimit(float angleValue)
        {
            cuspLimit_ = (angleValue == 0.0) ? 0.0 : PI - angleValue;
        }

        float CuspLimit() const
        {
            return (cuspLimit_ == 0.0) ? 0.0 : PI - cuspLimit_;
        }

        void Rewind(unsigned)
        {
            count_ = 0;
        }

        unsigned Vertex(float* x, float* y)
        {
            if (count_ >= points_.Size()) {
                return PATH_CMD_STOP;
            }
            const PointD& p = points_[count_++];
            *x = p.x;
            *y = p.y;
            return (count_ == 1) ? PATH_CMD_MOVE_TO : PATH_CMD_LINE_TO;
        }

    private:
        void Bezier(float x1, float y1,
                    float x2, float y2,
                    float x3, float y3,
                    float x4, float y4);

        void RecursiveBezier(float x1, float y1,
                             float x2, float y2,
                             float x3, float y3,
                             float x4, float y4,
                             unsigned level);

        float approximationScale_;
        float distanceToleranceSquare_;
        float angleTolerance_;
        float cuspLimit_;
        unsigned count_;
        GeometryPlainDataBlockVector<PointD> points_;
    };

    /**
     * @brief 二次贝塞尔曲线
     *
     * 根据起点、拐点、终点三个点，绘制出一条曲线
     *
     * @see QuadraticBezierCurve
     * @since 1.0
     * @version 1.0
     */
    class QuadraticBezierCurve {
    public:
        QuadraticBezierCurve()
            : approximationMethod_(CURVEDIVIDOPERATE)
        {}

        QuadraticBezierCurve(float x1, float y1, float x2, float y2, float x3, float y3)
            : approximationMethod_(CURVEDIVIDOPERATE)
        {
            Init(x1, y1, x2, y2, x3, y3);
        }

        void Init(float x1, float y1, float x2, float y2, float x3, float y3)
        {
            if (approximationMethod_ == CURVEINCREMENT) {
                curveInc_.Init(x1, y1, x2, y2, x3, y3);
            } else {
                curveDiv_.Init(x1, y1, x2, y2, x3, y3);
            }
        }

        void Reset()
        {
            curveInc_.Reset();
            curveDiv_.Reset();
        }

        void ApproximationMethod(CurveApproximationMethodEnum curveApproximationMethod)
        {
            approximationMethod_ = curveApproximationMethod;
        }

        CurveApproximationMethodEnum ApproximationMethod() const
        {
            return approximationMethod_;
        }

        void SetApproximationScale(float scale)
        {
            curveInc_.ApproximationScale(scale);
            curveDiv_.ApproximationScale(scale);
        }

        float ApproximationScale() const
        {
            return curveInc_.ApproximationScale();
        }

        void AngleTolerance(float angle)
        {
            curveDiv_.AngleTolerance(angle);
        }

        float AngleTolerance() const
        {
            return curveDiv_.AngleTolerance();
        }

        void CuspLimit(float angleValue)
        {
            curveDiv_.CuspLimit(angleValue);
        }

        float CuspLimit() const
        {
            return curveDiv_.CuspLimit();
        }

        void Rewind(unsigned pathId)
        {
            if (approximationMethod_ == CURVEINCREMENT) {
                curveInc_.Rewind(pathId);
            } else {
                curveDiv_.Rewind(pathId);
            }
        }

        unsigned Vertex(float* x, float* y)
        {
            if (approximationMethod_ == CURVEINCREMENT) {
                return curveInc_.Vertex(x, y);
            }
            return curveDiv_.Vertex(x, y);
        }

    private:
        QuadraticBezierCurveIncrement curveInc_;
        QuadraticBezierCurveDividOperate curveDiv_;
        CurveApproximationMethodEnum approximationMethod_;
    };

    /**
     * @brief 三次贝塞尔曲线
     *
     * 根据起点、控制点、终点绘制出一条曲线
     *
     * @see CubicBezierCurve
     * @since 1.0
     * @version 1.0
     */
    class CubicBezierCurve {
    public:
        CubicBezierCurve()
            : approximationMethod_(CURVEDIVIDOPERATE)
        {}

        CubicBezierCurve(float x1, float y1,
                         float x2, float y2,
                         float x3, float y3,
                         float x4, float y4)
            : approximationMethod_(CURVEDIVIDOPERATE)
        {
            Init(x1, y1, x2, y2, x3, y3, x4, y4);
        }

        CubicBezierCurve(const CubicBezierCurvePoints& curve4Points)
            : approximationMethod_(CURVEDIVIDOPERATE)
        {
            Init(curve4Points[0], curve4Points[1], curve4Points[2], curve4Points[3],
                 curve4Points[4], curve4Points[5], curve4Points[6], curve4Points[7]);
        }

        void Init(float x1, float y1,
                  float x2, float y2,
                  float x3, float y3,
                  float x4, float y4)
        {
            if (approximationMethod_ == CURVEINCREMENT) {
                curveInc_.Init(x1, y1, x2, y2, x3, y3, x4, y4);
            } else {
                curveDiv_.Init(x1, y1, x2, y2, x3, y3, x4, y4);
            }
        }

        void Init(const CubicBezierCurvePoints& curve4Points)
        {
            Init(curve4Points[0], curve4Points[1], curve4Points[2], curve4Points[3],
                 curve4Points[4], curve4Points[5], curve4Points[6], curve4Points[7]);
        }

        void Reset()
        {
            curveInc_.Reset();
            curveDiv_.Reset();
        }

        void ApproximationMethod(CurveApproximationMethodEnum curveApproximationMethod)
        {
            approximationMethod_ = curveApproximationMethod;
        }

        CurveApproximationMethodEnum ApproximationMethod() const
        {
            return approximationMethod_;
        }

        void SetApproximationScale(float scale)
        {
            curveInc_.ApproximationScale(scale);
            curveDiv_.ApproximationScale(scale);
        }
        float ApproximationScale() const
        {
            return curveInc_.ApproximationScale();
        }

        void AngleTolerance(float angleValue)
        {
            curveDiv_.AngleTolerance(angleValue);
        }

        float AngleTolerance() const
        {
            return curveDiv_.AngleTolerance();
        }

        void CuspLimit(float angleValue)
        {
            curveDiv_.CuspLimit(angleValue);
        }

        float CuspLimit() const
        {
            return curveDiv_.CuspLimit();
        }

        void Rewind(unsigned pathId)
        {
            if (approximationMethod_ == CURVEINCREMENT) {
                curveInc_.Rewind(pathId);
            } else {
                curveDiv_.Rewind(pathId);
            }
        }

        unsigned Vertex(float* x, float* y)
        {
            if (approximationMethod_ == CURVEINCREMENT) {
                return curveInc_.Vertex(x, y);
            }
            return curveDiv_.Vertex(x, y);
        }

    private:
        CubicBezierCurveIncrement curveInc_;
        CubicBezierCurveDividOperate curveDiv_;
        CurveApproximationMethodEnum approximationMethod_;
    };
} // namespace OHOS
#endif
