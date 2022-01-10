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

#include "gfx_utils/graphics/common/graphic_common_math.h"
#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_vertex_sequence.h"

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
        MathStroke()
            : strokeWidth(OHOS::ALPHAHALF),
            strokeWidthUsingAbs(OHOS::ALPHAHALF),
            strokeWidthPercentDivision(OHOS::ALPHAHALF / BUF_SIZE),
            strokeWidthSignal(1),
#if GRAPHIC_GEOMETYR_ENABLE_LINECAP_STYLES_VERTEX_SOURCE
            lineCapEnum(BUTT_CAP),
#endif
#if GRAPHIC_GEOMETYR_ENABLE_LINEJOIN_STYLES_VERTEX_SOURCE
            miterLimitMeasure(OHOS::DEFAULTMITERLIMIT),
            lineJoinEnum(MITER_JOIN),
#endif
          approxScaleRadio(1.0)
        {
        }
#if GRAPHIC_GEOMETYR_ENABLE_LINECAP_STYLES_VERTEX_SOURCE
        /**
         * @brief SetLineCap 定义线条的结束端点样式
         */
        void SetLineCap(LineCapEnum lineCapE)
        {
            lineCapEnum = lineCapE;
        }

        LineCapEnum GetLineCap() const
        {
            return lineCapEnum;
        }
        /**
         * @brief 计算端点样式
         * 注意在拐角的两端要做90度的旋转处理
         */
        void CalcCap(VertexConsumer& vertexConsumer, const VertexDist& vd0, const VertexDist& vd1, float len)
        {
            vertexConsumer.RemoveAll();
            if(len == 0.f) {
                len += VERTEX_DIST_EPSILON;
            }
            float dx1 = (vd1.vertexYCoord - vd0.vertexYCoord) / len;
            float dy1 = (vd1.vertexXCoord - vd0.vertexXCoord) / len;
            float dx2 = 0;
            float dy2 = 0;

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
                float deltaAngle =
                    std::acos(strokeWidthUsingAbs /
                              (strokeWidthUsingAbs + RADDALETAELPS / approxScaleRadio)) * TWO_TIMES;
                float angleStart;
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
#endif

#if GRAPHIC_GEOMETYR_ENABLE_LINEJOIN_STYLES_VERTEX_SOURCE
        /**
         * @brief SetLineJoin 定义两条线相交时，所创建的拐角类型
         * 注意在拐角的两端要做90度的旋转处理
         */
        void SetLineJoin(LineJoinEnum lineJoinE)
        {
            lineJoinEnum = lineJoinE;
        }

        /**
         * @brief SetMiterLimit 设置最大斜接长度
         */
        void SetMiterLimit(float miterLimit)
        {
            miterLimitMeasure = miterLimit;
        }

        /**
         * @brief 计算相交和拐角
         * 注意在拐角的两端要做90度的旋转处理
         */
        void CalcJoin(VertexConsumer& vertexConsumer,
                      const VertexDist& vertexDistBegin,
                      const VertexDist& vertexDistMiddle,
                      const VertexDist& vertexDistLast,
                      float deltaLengthPrev,
                      float deltaLengthLast)
        {
            if(deltaLengthPrev == 0.f) {
                deltaLengthPrev += VERTEX_DIST_EPSILON;
            }
            if(deltaLengthLast == 0.f) {
                deltaLengthPrev += VERTEX_DIST_EPSILON;
            }
            float dx1 = strokeWidth * (vertexDistMiddle.vertexYCoord - vertexDistBegin.vertexYCoord) / deltaLengthPrev;
            float dy1 = strokeWidth * (vertexDistMiddle.vertexXCoord - vertexDistBegin.vertexXCoord) / deltaLengthPrev;
            float dx2 = strokeWidth * (vertexDistLast.vertexYCoord - vertexDistMiddle.vertexYCoord) / deltaLengthLast;
            float dy2 = strokeWidth * (vertexDistLast.vertexXCoord - vertexDistMiddle.vertexXCoord) / deltaLengthLast;

            vertexConsumer.RemoveAll();

            float crossProduct =
                CrossProduct(vertexDistBegin.vertexXCoord, vertexDistBegin.vertexYCoord, vertexDistMiddle.vertexXCoord,
                             vertexDistMiddle.vertexYCoord, vertexDistLast.vertexXCoord, vertexDistLast.vertexYCoord);
            if (crossProduct != 0 && (crossProduct > 0) == (strokeWidth > 0)) {
                float limit =
                    ((deltaLengthPrev < deltaLengthLast) ? deltaLengthPrev : deltaLengthLast) / strokeWidthUsingAbs;
                CalcMiter(vertexConsumer, vertexDistBegin, vertexDistMiddle, vertexDistLast, dx1, dy1, dx2, dy2,
                          MITER_JOIN_REVERT, limit, 0);
            } else {
                float dx = (dx1 + dx2) * HALFNUM;
                float dy = (dy1 + dy2) * HALFNUM;
                float dbevel = std::sqrt(dx * dx + dy * dy);
                float lim = strokeWidthUsingAbs * miterLimitMeasure;
                bool isIntersection =
                    CalcIntersection(vertexDistBegin.vertexXCoord + dx1, vertexDistBegin.vertexYCoord - dy1,
                                     vertexDistMiddle.vertexXCoord + dx1, vertexDistMiddle.vertexYCoord - dy1,
                                     vertexDistMiddle.vertexXCoord + dx2, vertexDistMiddle.vertexYCoord - dy2,
                                     vertexDistLast.vertexXCoord + dx2, vertexDistLast.vertexYCoord - dy2, &dx, &dy);
                LineJoinEnum _lineJoinEnum = lineJoinEnum;
                if (_lineJoinEnum == MITER_JOIN) {
                    if (CalcDistance(vertexDistMiddle.vertexXCoord, vertexDistMiddle.vertexYCoord, dx, dy) > lim) {
                        _lineJoinEnum = BEVEL_JOIN;
                    }
                }
                if (_lineJoinEnum == ROUND_JOIN || _lineJoinEnum == BEVEL_JOIN) {
                    if (approxScaleRadio * (strokeWidthUsingAbs - dbevel) < strokeWidthPercentDivision) {
                        if (isIntersection) {
                            AddVertex(vertexConsumer, dx, dy);
                        } else {
                            AddVertex(vertexConsumer, vertexDistMiddle.vertexXCoord + dx1,
                                      vertexDistMiddle.vertexYCoord - dy1);
                        }
                        return;
                    }
                }

                switch (_lineJoinEnum) {
                    case MITER_JOIN:
                    case MITER_JOIN_REVERT:
                    case MITER_JOIN_ROUND:
                        CalcMiter(vertexConsumer, vertexDistBegin, vertexDistMiddle,
                                  vertexDistLast, dx1, dy1, dx2, dy2,
                                  lineJoinEnum, miterLimitMeasure, dbevel);
                        break;
                    case ROUND_JOIN:
                        CalcArc(vertexConsumer, vertexDistMiddle.vertexXCoord,
                                vertexDistMiddle.vertexYCoord, dx1, -dy1,
                                dx2, -dy2);
                        break;

                    default:
                        AddVertex(vertexConsumer, vertexDistMiddle.vertexXCoord + dx1,
                                  vertexDistMiddle.vertexYCoord - dy1);
                        AddVertex(vertexConsumer, vertexDistMiddle.vertexXCoord + dx2,
                                  vertexDistMiddle.vertexYCoord - dy2);
                        break;
                }
            }
        }

        /**
         * @brief 计算斜接长度
         */
        void CalcMiter(VertexConsumer& vertexConsumer,
                       const VertexDist& vd0,
                       const VertexDist& vd1,
                       const VertexDist& vd2,
                       float dx1,
                       float dy1,
                       float dx2,
                       float dy2,
                       LineJoinEnum linejoin,
                       float mlimit,
                       float dbevel)
        {
            float xi = vd1.vertexXCoord;
            float yi = vd1.vertexYCoord;
            float di = 1;
            float lim = strokeWidthUsingAbs * mlimit;
            bool miterLimitExceeded = true;
            bool intersectionFailed = true;
            if (CalcIntersection(vd0.vertexXCoord + dx1, vd0.vertexYCoord - dy1, vd1.vertexXCoord + dx1,
                                 vd1.vertexYCoord - dy1, vd1.vertexXCoord + dx2, vd1.vertexYCoord - dy2,
                                 vd2.vertexXCoord + dx2, vd2.vertexYCoord - dy2, &xi, &yi)) {
                di = CalcDistance(vd1.vertexXCoord, vd1.vertexYCoord, xi, yi);
                if (di <= lim) {
                    AddVertex(vertexConsumer, xi, yi);
                    miterLimitExceeded = false;
                }
                intersectionFailed = false;
            } else {
                float x2 = vd1.vertexXCoord + dx1;
                float y2 = vd1.vertexYCoord - dy1;
                if ((CrossProduct(vd0.vertexXCoord, vd0.vertexYCoord,
                                  vd1.vertexXCoord, vd1.vertexYCoord, x2, y2) < 0.0f) ==
                    (CrossProduct(vd1.vertexXCoord, vd1.vertexYCoord,
                                  vd2.vertexXCoord, vd2.vertexYCoord, x2, y2) < 0.0f)) {
                    AddVertex(vertexConsumer, vd1.vertexXCoord + dx1, vd1.vertexYCoord - dy1);
                    miterLimitExceeded = false;
                }
            }

            if (miterLimitExceeded) {
                switch (linejoin) {
                    case MITER_JOIN_REVERT:
                        AddVertex(vertexConsumer, vd1.vertexXCoord + dx1, vd1.vertexYCoord - dy1);
                        AddVertex(vertexConsumer, vd1.vertexXCoord + dx2, vd1.vertexYCoord - dy2);
                        break;
                    case MITER_JOIN_ROUND:
                        CalcArc(vertexConsumer, vd1.vertexXCoord, vd1.vertexYCoord, dx1, -dy1, dx2, -dy2);
                        break;
                    default:
                        if (intersectionFailed) {
                            mlimit *= strokeWidthSignal;
                            AddVertex(vertexConsumer, vd1.vertexXCoord + dx1 + dy1 * mlimit,
                                      vd1.vertexYCoord - dy1 + dx1 * mlimit);
                            AddVertex(vertexConsumer, vd1.vertexXCoord + dx2 - dy2 * mlimit,
                                      vd1.vertexYCoord - dy2 - dx2 * mlimit);
                        } else {
                            float x1 = vd1.vertexXCoord + dx1;
                            float y1 = vd1.vertexYCoord - dy1;
                            float x2 = vd1.vertexXCoord + dx2;
                            float y2 = vd1.vertexYCoord - dy2;
                            di = (lim - dbevel) / (di - dbevel);
                            AddVertex(vertexConsumer, x1 + (xi - x1) * di, y1 + (yi - y1) * di);
                            AddVertex(vertexConsumer, x2 + (xi - x2) * di, y2 + (yi - y2) * di);
                        }
                        break;
                }
            }
        }
        void CalcArc(VertexConsumer& vertexConsumer, float x, float y, float dx1, float dy1, float dx2, float dy2)
        {
            const float limitScale = 0.125;
            float angleStart = std::atan2(dy1 * strokeWidthSignal, dx1 * strokeWidthSignal);
            float angleEnd = std::atan2(dy2 * strokeWidthSignal, dx2 * strokeWidthSignal);
            float deltaAngle = angleStart - angleEnd;
            int nIndex, divNumber;

            deltaAngle = std::acos(strokeWidthUsingAbs / (strokeWidthUsingAbs + limitScale / approxScaleRadio))
                    * TWO_TIMES;

            AddVertex(vertexConsumer, x + dx1, y + dy1);
            if (strokeWidthSignal > 0) {
                if (angleStart > angleEnd) {
                    angleEnd += TWO_TIMES * PI;
                }
                divNumber = int((angleEnd - angleStart) / deltaAngle);
                deltaAngle = (angleEnd - angleStart) / (divNumber + 1);
                angleStart += deltaAngle;
                for (nIndex = 0; nIndex < divNumber; nIndex++) {
                    AddVertex(vertexConsumer, x + std::cos(angleStart) * strokeWidth,
                              y + std::sin(angleStart) * strokeWidth);
                    angleStart += deltaAngle;
                }
            } else {
                if (angleStart < angleEnd) {
                    angleEnd -= TWO_TIMES * PI;
                }
                divNumber = int((angleStart - angleEnd) / deltaAngle);
                deltaAngle = (angleStart - angleEnd) / (divNumber + 1);
                angleStart -= deltaAngle;
                for (nIndex = 0; nIndex < divNumber; nIndex++) {
                    AddVertex(vertexConsumer, x + std::cos(angleStart) * strokeWidth,
                              y + std::sin(angleStart) * strokeWidth);
                    angleStart -= deltaAngle;
                }
            }
            AddVertex(vertexConsumer, x + dx2, y + dy2);
        }
        LineJoinEnum GetLineJoin() const
        {
            return lineJoinEnum;
        }
        /**
         * @brief GetMiterLimit 返回最大斜接长度
         */
        float GetMiterLimit() const
        {
            return miterLimitMeasure;
        }
#endif

        /**
         * @brief width 设置区域宽
         */
        void width(float width)
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
         * @brief 添加近似值
         */
        void SetApproximationScale(float approximationScale)
        {
            approxScaleRadio = approximationScale;
        }

        /**
         * @brief width 返回宽度
         */
        float width() const
        {
            return strokeWidth * TWO_TIMES;
        }

        /**
         * @brief 返回设定的近似值
         */
        float GetApproximationScale() const
        {
            return approxScaleRadio;
        }

    private:
        GRAPHIC_GEOMETRY_INLINE void AddVertex(VertexConsumer& vertexConsumer, float x, float y)
        {
            vertexConsumer.Add(coord_type(x, y));
        }

        float strokeWidth;
        float strokeWidthUsingAbs;
        float strokeWidthPercentDivision;
        int strokeWidthSignal;

        float approxScaleRadio;
#if GRAPHIC_GEOMETYR_ENABLE_LINECAP_STYLES_VERTEX_SOURCE
        LineCapEnum lineCapEnum;
#endif
#if GRAPHIC_GEOMETYR_ENABLE_LINEJOIN_STYLES_VERTEX_SOURCE
        LineJoinEnum lineJoinEnum;
        float miterLimitMeasure;
#endif
    };
} // namespace OHOS

#endif
