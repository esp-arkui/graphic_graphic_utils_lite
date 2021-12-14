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
* @file graphic_geometry_MathStroke.h
* @brief Defines
* @since 1.0
* @version 1.0
*/

#ifndef STROKE_MATH_INCLUDED
#define STROKE_MATH_INCLUDED

#include "gfx_utils/graphics/graphic_common/graphic_common_math.h"
#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_vertex_sequence.h"

namespace OHOS {
    /**
     * @brief 线条末端线帽的样式。
     */
    enum LineCapEnum {
        /** 向线条的每个末端添加平直的边缘 */
        BUTT_CAP,
        /** 向线条的每个末端添加正方形线帽 */
        SQUARE_CAP,
        /** 向线条的每个末端添加圆形线帽 */
        ROUND_CAP
    };

    /**
     * @brief 两条线相交时，所创建的拐角类型
     */
    enum LineJoinEnum {
        /** 创建尖角 */
        MITER_JOIN = 0,
        MITER_JOIN_REVERT = 1,
        /** 创建圆角 */
        ROUND_JOIN = 2,
        /** 创建斜角 */
        BEVEL_JOIN = 3,
        MITER_JOIN_ROUND = 4
    };

    template <class VertexConsumer>
    class MathStroke {
    public:
        typedef typename VertexConsumer::ValueType coord_type;
        MathStroke() :
            strokeWidth(OHOS::ALPHAHALF),
            strokeWidthUsingAbs(OHOS::ALPHAHALF),
            strokeWidthPercentDivision(OHOS::ALPHAHALF / BUF_SIZE),
            strokeWidthSignal(1),
            miterLimitMeasure(OHOS::DEFAULTMITERLIMIT),
            approxScaleRadio(1.0),
            lineCapEnum(BUTT_CAP),
            lineJoinEnum(MITER_JOIN)
        {
        }

        /**
         * @brief SetLineCap 定义线条的结束端点样式
         */
        void SetLineCap(LineCapEnum lineCapE)
        {
            lineCapEnum = lineCapE;
        }
        /**
         * @brief SetLineJoin 定义两条线相交时，所创建的拐角类型
         */
        void SetLineJoin(LineJoinEnum lineJoinE)
        {
            lineJoinEnum = lineJoinE;
        }

        LineCapEnum GetLineCap() const
        {
            return lineCapEnum;
        }

        LineJoinEnum GetLineJoin() const
        {
            return lineJoinEnum;
        }

        /**
         * @brief width 设置区域宽
         */
        void width(double width)
        {
            strokeWidth = width * OHOS::ALPHAHALF;
            if (strokeWidth < 0) {
                strokeWidthUsingAbs = -strokeWidth;
                strokeWidthSignal = -1;
            } else {
                strokeWidthUsingAbs = strokeWidth;
                strokeWidthSignal = 1;
            }
            strokeWidthPercentDivision = strokeWidth / BUF_SIZE;
        }

        /**
         * @brief SetMiterLimit 设置最大斜接长度
         */
        void SetMiterLimit(double miterLimit)
        {
            miterLimitMeasure = miterLimit;
        }

        /**
         * @brief 添加近似值
         */
        void SetApproximationScale(double approximationScale)
        {
            approxScaleRadio = approximationScale;
        }

        /**
         * @brief width 返回宽度
         */
        double width() const
        {
            return strokeWidth * TWO_TIMES;
        }

        /**
         * @brief GetMiterLimit 返回最大斜接长度
         */
        double GetMiterLimit() const
        {
            return miterLimitMeasure;
        }

        /**
         * @brief 返回设定的近似值
         */
        double GetApproximationScale() const
        {
            return approxScaleRadio;
        }

        /**
         * @brief 计算端点样式
         */
        void CalcCap(VertexConsumer& vertexConsumer, const VertexDist& vd0, const VertexDist& vd1, double len)
        {
            vertexConsumer.RemoveAll();

            double dx1 = (vd1.vertexYCoord - vd0.vertexYCoord) / len;
            double dy1 = (vd1.vertexXCoord - vd0.vertexXCoord) / len;
            double dx2 = 0;
            double dy2 = 0;

            dx1 *= strokeWidth;
            dy1 *= strokeWidth;

            if (lineCapEnum != ROUND_CAP) {
                if (lineCapEnum == SQUARE_CAP) {
                    dx2 = dy1 * strokeWidthSignal;
                    dy2 = dx1 * strokeWidthSignal;
                }
                AddVertex(vertexConsumer, vd0.vertexXCoord - dx1 - dx2, vd0.vertexYCoord + dy1 - dy2);
                AddVertex(vertexConsumer, vd0.vertexXCoord + dx1 - dx2, vd0.vertexYCoord - dy1 - dy2);
            } else {
                double deltaAngle = std::acos(strokeWidthUsingAbs / (strokeWidthUsingAbs + RADDALETAELPS / approxScaleRadio)) * TWO_TIMES;
                double angleStart;
                int nIndex;
                int divNumber = int(PI / deltaAngle);

                deltaAngle = PI / (divNumber + 1);
                AddVertex(vertexConsumer, vd0.vertexXCoord - dx1, vd0.vertexYCoord + dy1);
                if (strokeWidthSignal > 0) {
                    angleStart = std::atan2(dy1, -dx1);
                    angleStart += deltaAngle;
                    for (nIndex = 0; nIndex < divNumber; nIndex++) {
                        AddVertex(vertexConsumer, vd0.vertexXCoord + std::cos(angleStart) * strokeWidth,
                                  vd0.vertexYCoord + std::sin(angleStart) * strokeWidth);
                        angleStart += deltaAngle;
                    }
                } else {
                    angleStart = std::atan2(-dy1, dx1);
                    angleStart -= deltaAngle;
                    for (nIndex = 0; nIndex < divNumber; nIndex++) {
                        AddVertex(vertexConsumer, vd0.vertexXCoord + std::cos(angleStart) * strokeWidth,
                                  vd0.vertexYCoord + std::sin(angleStart) * strokeWidth);
                        angleStart -= deltaAngle;
                    }
                }
                AddVertex(vertexConsumer, vd0.vertexXCoord + dx1, vd0.vertexYCoord - dy1);
            }
        }

        /**
         * @brief 计算相交和拐角
         */
        void CalcJoin(VertexConsumer& vertexConsumer, const VertexDist& vertexDistBegin, const VertexDist& vertexDistMiddle,
                      const VertexDist& vertexDistLast, double deltaLengthPrev, double deltaLengthLast)
        {
            double dx1 = strokeWidth * (vertexDistMiddle.vertexYCoord - vertexDistBegin.vertexYCoord) / deltaLengthPrev;
            double dy1 = strokeWidth * (vertexDistMiddle.vertexXCoord - vertexDistBegin.vertexXCoord) / deltaLengthPrev;
            double dx2 = strokeWidth * (vertexDistLast.vertexYCoord - vertexDistMiddle.vertexYCoord) / deltaLengthLast;
            double dy2 = strokeWidth * (vertexDistLast.vertexXCoord - vertexDistMiddle.vertexXCoord) / deltaLengthLast;

            vertexConsumer.RemoveAll();

            double crossProduct = CrossProduct(vertexDistBegin.vertexXCoord, vertexDistBegin.vertexYCoord, vertexDistMiddle.vertexXCoord, vertexDistMiddle.vertexYCoord, vertexDistLast.vertexXCoord, vertexDistLast.vertexYCoord);
            if (crossProduct != 0 && (crossProduct > 0) == (strokeWidth > 0)) {
                double limit = ((deltaLengthPrev < deltaLengthLast) ? deltaLengthPrev : deltaLengthLast) / strokeWidthUsingAbs;
                CalcMiter(vertexConsumer, vertexDistBegin, vertexDistMiddle, vertexDistLast, dx1, dy1, dx2, dy2, MITER_JOIN_REVERT, limit, 0);
            } else {
                double dx = (dx1 + dx2) * HALFNUM;
                double dy = (dy1 + dy2) * HALFNUM;
                double dbevel = std::sqrt(dx * dx + dy * dy);

                if (lineJoinEnum == ROUND_JOIN || lineJoinEnum == BEVEL_JOIN) {
                    if (approxScaleRadio * (strokeWidthUsingAbs - dbevel) < strokeWidthPercentDivision) {
                        if (CalcIntersection(vertexDistBegin.vertexXCoord + dx1, vertexDistBegin.vertexYCoord - dy1,
                                             vertexDistMiddle.vertexXCoord + dx1, vertexDistMiddle.vertexYCoord - dy1,
                                             vertexDistMiddle.vertexXCoord + dx2, vertexDistMiddle.vertexYCoord - dy2,
                                             vertexDistLast.vertexXCoord + dx2, vertexDistLast.vertexYCoord - dy2,
                                             &dx, &dy)) {
                            AddVertex(vertexConsumer, dx, dy);
                        } else {
                            AddVertex(vertexConsumer, vertexDistMiddle.vertexXCoord + dx1, vertexDistMiddle.vertexYCoord - dy1);
                        }
                        return;
                    }
                }

                switch (lineJoinEnum) {
                    case MITER_JOIN:
                    case MITER_JOIN_REVERT:
                    case MITER_JOIN_ROUND:
                        CalcMiter(vertexConsumer, vertexDistBegin, vertexDistMiddle, vertexDistLast, dx1, dy1, dx2, dy2, lineJoinEnum, miterLimitMeasure, dbevel);
                        break;
                    case ROUND_JOIN:
                        CalcArc(vertexConsumer, vertexDistMiddle.vertexXCoord, vertexDistMiddle.vertexYCoord, dx1, -dy1, dx2, -dy2);
                        break;

                    default:
                        AddVertex(vertexConsumer, vertexDistMiddle.vertexXCoord + dx1, vertexDistMiddle.vertexYCoord - dy1);
                        AddVertex(vertexConsumer, vertexDistMiddle.vertexXCoord + dx2, vertexDistMiddle.vertexYCoord - dy2);
                        break;
                }
            }
        }

    private:
        GRAPHIC_GEOMETRY_INLINE void AddVertex(VertexConsumer& vertexConsumer, double x, double y)
        {
            vertexConsumer.Add(coord_type(x, y));
        }

        void CalcArc(VertexConsumer& vertexConsumer,
                     double x, double y,
                     double dx1, double dy1,
                     double dx2, double dy2)
        {
            const float limitScale = 0.125;
            double angleStart = std::atan2(dy1 * strokeWidthSignal, dx1 * strokeWidthSignal);
            double angleEnd = std::atan2(dy2 * strokeWidthSignal, dx2 * strokeWidthSignal);
            double deltaAngle = angleStart - angleEnd;
            int nIndex, divNumber;

            deltaAngle = std::acos(strokeWidthUsingAbs / (strokeWidthUsingAbs + limitScale / approxScaleRadio)) * TWO_TIMES;

            AddVertex(vertexConsumer, x + dx1, y + dy1);
            if (strokeWidthSignal > 0) {
                if (angleStart > angleEnd)
                    angleEnd += TWO_TIMES * PI;
                divNumber = int((angleEnd - angleStart) / deltaAngle);
                deltaAngle = (angleEnd - angleStart) / (divNumber + 1);
                angleStart += deltaAngle;
                for (nIndex = 0; nIndex < divNumber; nIndex++) {
                    AddVertex(vertexConsumer, x + std::cos(angleStart) * strokeWidth, y + std::sin(angleStart) * strokeWidth);
                    angleStart += deltaAngle;
                }
            } else {
                if (angleStart < angleEnd)
                    angleEnd -= TWO_TIMES * PI;
                divNumber = int((angleStart - angleEnd) / deltaAngle);
                deltaAngle = (angleStart - angleEnd) / (divNumber + 1);
                angleStart -= deltaAngle;
                for (nIndex = 0; nIndex < divNumber; nIndex++) {
                    AddVertex(vertexConsumer, x + std::cos(angleStart) * strokeWidth, y + std::sin(angleStart) * strokeWidth);
                    angleStart -= deltaAngle;
                }
            }
            AddVertex(vertexConsumer, x + dx2, y + dy2);
        }

        /**
         * @brief 计算斜接长度
         */
        void CalcMiter(VertexConsumer& vc,
                       const VertexDist& vd0,
                       const VertexDist& vd1,
                       const VertexDist& vd2,
                       double dx1, double dy1,
                       double dx2, double dy2,
                       LineJoinEnum linejoin,
                       double mlimit,
                       double dbevel)
        {
            double xi = vd1.vertexXCoord;
            double yi = vd1.vertexYCoord;
            double di = 1;
            double lim = strokeWidthUsingAbs * mlimit;
            bool miterLimitExceeded = true;
            bool intersectionFailed = true;
            if (CalcIntersection(vd0.vertexXCoord + dx1, vd0.vertexYCoord - dy1,
                                 vd1.vertexXCoord + dx1, vd1.vertexYCoord - dy1,
                                 vd1.vertexXCoord + dx2, vd1.vertexYCoord - dy2,
                                 vd2.vertexXCoord + dx2, vd2.vertexYCoord - dy2,
                                 &xi, &yi)) {
                di = CalcDistance(vd1.vertexXCoord, vd1.vertexYCoord, xi, yi);
                if (di <= lim) {
                    AddVertex(vc, xi, yi);
                    miterLimitExceeded = false;
                }
                intersectionFailed = false;
            } else {
                double x2 = vd1.vertexXCoord + dx1;
                double y2 = vd1.vertexYCoord - dy1;
                if ((CrossProduct(vd0.vertexXCoord, vd0.vertexYCoord, vd1.vertexXCoord, vd1.vertexYCoord, x2, y2) < 0.0) ==
                    (CrossProduct(vd1.vertexXCoord, vd1.vertexYCoord, vd2.vertexXCoord, vd2.vertexYCoord, x2, y2) < 0.0)) {
                    AddVertex(vc, vd1.vertexXCoord + dx1, vd1.vertexYCoord - dy1);
                    miterLimitExceeded = false;
                }
            }

            if (miterLimitExceeded) {
                switch (linejoin) {
                    case MITER_JOIN_REVERT:
                        AddVertex(vc, vd1.vertexXCoord + dx1, vd1.vertexYCoord - dy1);
                        AddVertex(vc, vd1.vertexXCoord + dx2, vd1.vertexYCoord - dy2);
                        break;

                    case MITER_JOIN_ROUND:
                        CalcArc(vc, vd1.vertexXCoord, vd1.vertexYCoord, dx1, -dy1, dx2, -dy2);
                        break;

                    default:
                        if (intersectionFailed) {
                            mlimit *= strokeWidthSignal;
                            AddVertex(vc, vd1.vertexXCoord + dx1 + dy1 * mlimit, vd1.vertexYCoord - dy1 + dx1 * mlimit);
                            AddVertex(vc, vd1.vertexXCoord + dx2 - dy2 * mlimit, vd1.vertexYCoord - dy2 - dx2 * mlimit);
                        } else {
                            double x1 = vd1.vertexXCoord + dx1;
                            double y1 = vd1.vertexYCoord - dy1;
                            double x2 = vd1.vertexXCoord + dx2;
                            double y2 = vd1.vertexYCoord - dy2;
                            di = (lim - dbevel) / (di - dbevel);
                            AddVertex(vc, x1 + (xi - x1) * di, y1 + (yi - y1) * di);
                            AddVertex(vc, x2 + (xi - x2) * di, y2 + (yi - y2) * di);
                        }
                        break;
                }
            }
        }

        double strokeWidth;
        double strokeWidthUsingAbs;
        double strokeWidthPercentDivision;
        int strokeWidthSignal;
        double miterLimitMeasure;
        double approxScaleRadio;
        LineCapEnum lineCapEnum;
        LineJoinEnum lineJoinEnum;
    };
} // namespace OHOS

#endif
