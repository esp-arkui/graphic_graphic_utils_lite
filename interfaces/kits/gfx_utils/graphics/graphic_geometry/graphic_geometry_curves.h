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

#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_array.h"

namespace OHOS {
    enum CurveApproximationMethodEnum {
        CURVEINC,
        CURVEDIV
    };

    class Curve3Inc {
    public:
        Curve3Inc()
            :numSteps_(0), step_(0), scale_(1.0)
        {}

        Curve3Inc(double x1, double y1,
                  double x2, double y2,
                  double x3, double y3) :
            numSteps_(0),
            step_(0), scale_(1.0)
        {
            Init(x1, y1, x2, y2, x3, y3);
        }

        void Init(double x1, double y1,
                  double x2, double y2,
                  double x3, double y3);

        void Reset()
        {
            numSteps_ = 0;
            step_ = -1;
        }

        void ApproximationMethod(CurveApproximationMethodEnum)
        {}
        CurveApproximationMethodEnum ApproximationMethod() const
        {
            return CURVEINC;
        }

        void ApproximationScale(double scale);
        double ApproximationScale() const;

        void AngleTolerance(double)
        {}
        double AngleTolerance() const
        {
            return 0.0;
        }

        void CuspLimit(double)
        {}
        double CuspLimit() const
        {
            return 0.0;
        }

        void Rewind(unsigned pathId);
        unsigned Vertex(double* x, double* y);

    private:
        int numSteps_;
        int step_;
        double scale_;
        double startX_;
        double startY_;
        double endX_;
        double endY_;
        double fx_;
        double fy_;
        double dfx_;
        double dfy_;
        double ddfx_;
        double ddfy_;
        double savedFx_;
        double savedFy_;
        double savedDfx_;
        double savedDfy_;
    };

    class Curve3Div {
    public:
        Curve3Div() :
            approximationScale_(1.0), angleTolerance_(0.0), count_(0)
        {}

        Curve3Div(double x1, double y1,
                  double x2, double y2,
                  double x3, double y3) :
            approximationScale_(1.0),
            angleTolerance_(0.0), count_(0)
        {
            Init(x1, y1, x2, y2, x3, y3);
        }

        void Init(double x1, double y1, double x2, double y2, double x3, double y3);

        void Reset()
        {
            points_.RemoveAll();
            count_ = 0;
        }

        void ApproximationMethod(CurveApproximationMethodEnum)
        {}
        CurveApproximationMethodEnum ApproximationMethod() const
        {
            return CURVEDIV;
        }

        void ApproximationScale(double scale)
        {
            approximationScale_ = scale;
        }
        double ApproximationScale() const
        {
            return approximationScale_;
        }

        void AngleTolerance(double angle)
        {
            angleTolerance_ = angle;
        }
        double AngleTolerance() const
        {
            return angleTolerance_;
        }

        void CuspLimit(double)
        {}
        double CuspLimit() const
        {
            return 0.0;
        }

        void Rewind(unsigned)
        {
            count_ = 0;
        }

        unsigned Vertex(double* x, double* y)
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
        void Bezier(double x1, double y1,
                    double x2, double y2,
                    double x3, double y3);
        void RecursiveBezier(double x1, double y1,
                             double x2, double y2,
                             double x3, double y3,
                             unsigned level);

        double approximationScale_;
        double distanceToleranceSquare_;
        double angleTolerance_;
        unsigned count_;
        PodBvector<PointD> points_;
    };

    struct Curve4Points {
        double cpArray[8];

        Curve4Points()
        {}
        Curve4Points(double x1, double y1,
                     double x2, double y2,
                     double x3, double y3,
                     double x4, double y4)
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

        void Init(double x1, double y1,
                  double x2, double y2,
                  double x3, double y3,
                  double x4, double y4)
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

        double operator[](unsigned i) const
        {
            return cpArray[i];
        }
        double& operator[](unsigned i)
        {
            return cpArray[i];
        }
    };

    class Curve4Inc {
    public:
        Curve4Inc() :
            numSteps_(0), step_(0), scale_(1.0)
        {}

        Curve4Inc(double x1, double y1, double x2, double y2,
                  double x3, double y3, double x4, double y4) :
            numSteps_(0),
            step_(0), scale_(1.0)
        {
            Init(x1, y1, x2, y2, x3, y3, x4, y4);
        }

        Curve4Inc(const Curve4Points& curve4Points) :
            numSteps_(0), step_(0), scale_(1.0)
        {
            Init(curve4Points[0], curve4Points[1], curve4Points[2], curve4Points[3],
                 curve4Points[4], curve4Points[5], curve4Points[6], curve4Points[7]);
        }

        void Init(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
        void Init(const Curve4Points& curve4Points)
        {
            Init(curve4Points[0], curve4Points[1], curve4Points[2], curve4Points[3],
                 curve4Points[4], curve4Points[5], curve4Points[6], curve4Points[7]);
        }

        void Reset()
        {
            numSteps_ = 0;
            step_ = -1;
        }

        void ApproximationMethod(CurveApproximationMethodEnum)
        {}
        CurveApproximationMethodEnum ApproximationMethod() const
        {
            return CURVEINC;
        }

        void ApproximationScale(double scale);
        double ApproximationScale() const;

        void AngleTolerance(double)
        {}
        double AngleTolerance() const
        {
            return 0.0;
        }

        void CuspLimit(double)
        {}
        double CuspLimit() const
        {
            return 0.0;
        }

        void Rewind(unsigned pathId);
        unsigned Vertex(double* x, double* y);

    private:
        int numSteps_;
        int step_;
        double scale_;
        double startX_;
        double startY_;
        double endX_;
        double endY_;
        double fx_;
        double fy_;
        double dfx_;
        double dfy_;
        double ddfx_;
        double ddfy_;
        double dddfx_;
        double dddfy_;
        double savedFx_;
        double savedFy_;
        double savedDfx_;
        double savedDfy_;
        double savedDdfx_;
        double savedDdfy_;
    };

    inline Curve4Points CatromToBezier(double x1, double y1,
                                       double x2, double y2,
                                       double x3, double y3,
                                       double x4, double y4)
    {
        return Curve4Points(
            x2,
            y2,
            (-x1 + OHOS::SIX_TIMES * x2 + x3) / OHOS::SIX_TIMES,
            (-y1 + OHOS::SIX_TIMES * y2 + y3) / OHOS::SIX_TIMES,
            (x2 + OHOS::SIX_TIMES * x3 - x4) / OHOS::SIX_TIMES,
            (y2 + OHOS::SIX_TIMES * y3 - y4) / OHOS::SIX_TIMES,
            x3,
            y3);
    }

    inline Curve4Points CatromToBezier(const Curve4Points& curve4Points)
    {
        return CatromToBezier(curve4Points[0], curve4Points[1], curve4Points[2], curve4Points[3],
                              curve4Points[4], curve4Points[5], curve4Points[6], curve4Points[7]);
    }

    inline Curve4Points UbsplineToBezier(double x1, double y1,
                                         double x2, double y2,
                                         double x3, double y3,
                                         double x4, double y4)
    {
        return Curve4Points(
            (x1 + OHOS::FOUR_TIMES * x2 + x3) / OHOS::SIX_TIMES,
            (y1 + OHOS::FOUR_TIMES * y2 + y3) / OHOS::SIX_TIMES,
            (OHOS::FOUR_TIMES * x2 + OHOS::TWO_TIMES * x3) / OHOS::SIX_TIMES,
            (OHOS::FOUR_TIMES * y2 + OHOS::TWO_TIMES * y3) / OHOS::SIX_TIMES,
            (OHOS::TWO_TIMES * x2 + OHOS::FOUR_TIMES * x3) / OHOS::SIX_TIMES,
            (OHOS::TWO_TIMES * y2 + OHOS::FOUR_TIMES * y3) / OHOS::SIX_TIMES,
            (x2 + OHOS::FOUR_TIMES * x3 + x4) / OHOS::SIX_TIMES,
            (y2 + OHOS::FOUR_TIMES * y3 + y4) / OHOS::SIX_TIMES);
    }

    inline Curve4Points UbsplineToBezier(const Curve4Points& curve4Points)
    {
        return UbsplineToBezier(curve4Points[0], curve4Points[1], curve4Points[2], curve4Points[3],
                                curve4Points[4], curve4Points[5], curve4Points[6], curve4Points[7]);
    }

    inline Curve4Points HermiteToBezier(double x1, double y1,
                                        double x2, double y2,
                                        double x3, double y3,
                                        double x4, double y4)
    {
        return Curve4Points(
            x1,
            y1,
            (OHOS::THREE_TIMES * x1 + x3) / OHOS::THREE_TIMES,
            (OHOS::THREE_TIMES * y1 + y3) / OHOS::THREE_TIMES,
            (OHOS::THREE_TIMES * x2 - x4) / OHOS::THREE_TIMES,
            (OHOS::THREE_TIMES * y2 - y4) / OHOS::THREE_TIMES,
            x2,
            y2);
    }

    inline Curve4Points HermiteToBezier(const Curve4Points& curve4Points)
    {
        return HermiteToBezier(curve4Points[OHOS::INDEX_ZERO], curve4Points[OHOS::INDEX_ONE],
                                   curve4Points[OHOS::INDEX_TWO], curve4Points[OHOS::INDEX_THREE],
                                   curve4Points[OHOS::INDEX_FOUR], curve4Points[OHOS::INDEX_FIVE],
                                   curve4Points[OHOS::INDEX_SIX], curve4Points[OHOS::INDEX_SEVEN]);
    }

    class Curve4Div {
    public:
        Curve4Div() :
            approximationScale_(1.0), angleTolerance_(0.0), cuspLimit_(0.0), count_(0)
        {}

        Curve4Div(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) :
            approximationScale_(1.0), angleTolerance_(0.0), cuspLimit_(0.0), count_(0)
        {
            Init(x1, y1, x2, y2, x3, y3, x4, y4);
        }

        Curve4Div(const Curve4Points& curve4Points) :
            approximationScale_(1.0),
            angleTolerance_(0.0),
            count_(0)
        {
            Init(curve4Points[OHOS::INDEX_ZERO], curve4Points[OHOS::INDEX_ONE],
                    curve4Points[OHOS::INDEX_TWO], curve4Points[OHOS::INDEX_THREE],
                    curve4Points[OHOS::INDEX_FOUR], curve4Points[OHOS::INDEX_FIVE],
                    curve4Points[OHOS::INDEX_SIX], curve4Points[OHOS::INDEX_SEVEN]);
        }

        void Init(double x1, double y1,
                  double x2, double y2,
                  double x3, double y3,
                  double x4, double y4);

        void Init(const Curve4Points& curve4Points)
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
            return CURVEDIV;
        }

        void ApproximationScale(double scale)
        {
            approximationScale_ = scale;
        }
        double ApproximationScale() const
        {
            return approximationScale_;
        }

        void AngleTolerance(double a)
        {
            angleTolerance_ = a;
        }
        double AngleTolerance() const
        {
            return angleTolerance_;
        }

        void CuspLimit(double angleValue)
        {
            cuspLimit_ = (angleValue == 0.0) ? 0.0 : PI - angleValue;
        }

        double CuspLimit() const
        {
            return (cuspLimit_ == 0.0) ? 0.0 : PI - cuspLimit_;
        }

        void Rewind(unsigned)
        {
            count_ = 0;
        }

        unsigned Vertex(double* x, double* y)
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
        void Bezier(double x1, double y1,
                    double x2, double y2,
                    double x3, double y3,
                    double x4, double y4);

        void RecursiveBezier(double x1, double y1,
                             double x2, double y2,
                             double x3, double y3,
                             double x4, double y4,
                             unsigned level);

        double approximationScale_;
        double distanceToleranceSquare_;
        double angleTolerance_;
        double cuspLimit_;
        unsigned count_;
        PodBvector<PointD> points_;
    };

    /**
     * @brief 二次贝塞尔曲线
     *
     * 根据起点、拐点、终点三个点，绘制出一条曲线
     *
     * @see Curve3
     * @since 1.0
     * @version 1.0
     */
    class Curve3 {
    public:
        Curve3() :
            approximationMethod_(CURVEDIV)
        {}

        Curve3(double x1, double y1, double x2, double y2, double x3, double y3) :
            approximationMethod_(CURVEDIV)
        {
            Init(x1, y1, x2, y2, x3, y3);
        }

        void Init(double x1, double y1, double x2, double y2, double x3, double y3)
        {
            if (approximationMethod_ == CURVEINC) {
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

        void SetApproximationScale(double scale)
        {
            curveInc_.ApproximationScale(scale);
            curveDiv_.ApproximationScale(scale);
        }

        double ApproximationScale() const
        {
            return curveInc_.ApproximationScale();
        }

        void AngleTolerance(double angle)
        {
            curveDiv_.AngleTolerance(angle);
        }

        double AngleTolerance() const
        {
            return curveDiv_.AngleTolerance();
        }

        void CuspLimit(double angleValue)
        {
            curveDiv_.CuspLimit(angleValue);
        }

        double CuspLimit() const
        {
            return curveDiv_.CuspLimit();
        }

        void Rewind(unsigned pathId)
        {
            if (approximationMethod_ == CURVEINC) {
                curveInc_.Rewind(pathId);
            } else {
                curveDiv_.Rewind(pathId);
            }
        }

        unsigned Vertex(double* x, double* y)
        {
            if (approximationMethod_ == CURVEINC) {
                return curveInc_.Vertex(x, y);
            }
            return curveDiv_.Vertex(x, y);
        }

    private:
        Curve3Inc curveInc_;
        Curve3Div curveDiv_;
        CurveApproximationMethodEnum approximationMethod_;
    };

    /**
     * @brief 三次贝塞尔曲线
     *
     * 根据起点、控制点、终点绘制出一条曲线
     *
     * @see Curve4
     * @since 1.0
     * @version 1.0
     */
    class Curve4 {
    public:
        Curve4() :
            approximationMethod_(CURVEDIV)
        {}

        Curve4(double x1, double y1,
               double x2, double y2,
               double x3, double y3,
               double x4, double y4) :
            approximationMethod_(CURVEDIV)
        {
            Init(x1, y1, x2, y2, x3, y3, x4, y4);
        }

        Curve4(const Curve4Points& curve4Points) :
            approximationMethod_(CURVEDIV)
        {
            Init(curve4Points[0], curve4Points[1], curve4Points[2], curve4Points[3],
                 curve4Points[4], curve4Points[5], curve4Points[6], curve4Points[7]);
        }

        void Init(double x1, double y1,
                  double x2, double y2,
                  double x3, double y3,
                  double x4, double y4)
        {
            if (approximationMethod_ == CURVEINC) {
                curveInc_.Init(x1, y1, x2, y2, x3, y3, x4, y4);
            } else {
                curveDiv_.Init(x1, y1, x2, y2, x3, y3, x4, y4);
            }
        }

        void Init(const Curve4Points& curve4Points)
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

        void SetApproximationScale(double scale)
        {
            curveInc_.ApproximationScale(scale);
            curveDiv_.ApproximationScale(scale);
        }
        double ApproximationScale() const
        {
            return curveInc_.ApproximationScale();
        }

        void AngleTolerance(double angleValue)
        {
            curveDiv_.AngleTolerance(angleValue);
        }

        double AngleTolerance() const
        {
            return curveDiv_.AngleTolerance();
        }

        void CuspLimit(double angleValue)
        {
            curveDiv_.CuspLimit(angleValue);
        }

        double CuspLimit() const
        {
            return curveDiv_.CuspLimit();
        }

        void Rewind(unsigned pathId)
        {
            if (approximationMethod_ == CURVEINC) {
                curveInc_.Rewind(pathId);
            } else {
                curveDiv_.Rewind(pathId);
            }
        }

        unsigned Vertex(double* x, double* y)
        {
            if (approximationMethod_ == CURVEINC) {
                return curveInc_.Vertex(x, y);
            }
            return curveDiv_.Vertex(x, y);
        }

    private:
        Curve4Inc curveInc_;
        Curve4Div curveDiv_;
        CurveApproximationMethodEnum approximationMethod_;
    };
} // namespace OHOS

#endif
