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

#include "gfx_utils/graphics/graphic_common/agg_math.h"
#include "gfx_utils/graphics/graphic_geometry/agg_vertex_sequence.h"

namespace OHOS
{
    /**
     * @brief 线条末端线帽的样式。
     */
    enum LineCap
    {
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
    enum LineJoin
    {
        /** 创建尖角 */
        MITER_JOIN = 0,
        MITER_JOIN_REVERT = 1,
        /** 创建圆角 */
        ROUND_JOIN = 2,
        /** 创建斜角 */
        BEVEL_JOIN = 3,
        MITER_JOIN_ROUND = 4
    };

    enum InnerJoin
    {
        INNER_BEVEL,
        INNER_MITER,
        INNER_JAG,
        INNER_ROUND
    };

    template <class VertexConsumer>
    class MathStroke
    {
    public:
        typedef typename VertexConsumer::value_type coord_type;
        MathStroke() :
            width_(0.5),
            widthAbs_(0.5),
            widthEps_(0.5 / 1024.0),
            widthSign_(1),
            miterLimit_(4.0),
            approxScale_(1.0),
            lineCapEnum(BUTT_CAP),
            lineJoinEnum(MITER_JOIN),
            innerJoinEnum(INNER_MITER)
        {
        }

        /**
         * @brief SetLineCap 定义线条的结束端点样式
         */
        void SetLineCap(LineCap lineCapE)
        {
            lineCapEnum = lineCapE;
        }
        /**
         * @brief SetLineJoin 定义两条线相交时，所创建的拐角类型
         */
        void SetLineJoin(LineJoin lineJoinE)
        {
            lineJoinEnum = lineJoinE;
        }

        void SetInnerJoin(InnerJoin innerJoinE)
        {
            innerJoinEnum = innerJoinE;
        }

        LineCap GetLineCap() const
        {
            return lineCapEnum;
        }

        LineJoin GetLineJoin() const
        {
            return lineJoinEnum;
        }

        InnerJoin GetInnerJoin() const
        {
            return innerJoinEnum;
        }

        /**
         * @brief width 设置区域宽
         */
        void width(double width)
        {
            width_ = width * 0.5;
            if (width_ < 0)
            {
                widthAbs_ = -width_;
                widthSign_ = -1;
            }
            else
            {
                widthAbs_ = width_;
                widthSign_ = 1;
            }
            widthEps_ = width_ / 1024.0;
        }

        /**
         * @brief SetMiterLimit 设置最大斜接长度
         */
        void SetMiterLimit(double miterLimit)
        {
            miterLimit_ = miterLimit;
        }

        /**
         * @brief 添加近似值
         */
        void SetApproximationScale(double approximationScale)
        {
            approxScale_ = approximationScale;
        }

        /**
         * @brief width 返回宽度
         */
        double width() const
        {
            return width_ * 2.0;
        }

        /**
         * @brief GetMiterLimit 返回最大斜接长度
         */
        double GetMiterLimit() const
        {
            return miterLimit_;
        }

        /**
         * @brief 返回设定的近似值
         */
        double GetApproximationScale() const
        {
            return approxScale_;
        }

        /**
         * @brief 计算端点样式
         */
        void CalcCap(VertexConsumer& vc, const vertex_dist& vd0, const vertex_dist& vd1, double len)
        {
            vc.remove_all();

            double dx1 = (vd1.y - vd0.y) / len;
            double dy1 = (vd1.x - vd0.x) / len;
            double dx2 = 0;
            double dy2 = 0;

            dx1 *= width_;
            dy1 *= width_;

            if (lineCapEnum != ROUND_CAP)
            {
                if (lineCapEnum == SQUARE_CAP)
                {
                    dx2 = dy1 * widthSign_;
                    dy2 = dx1 * widthSign_;
                }
                AddVertex(vc, vd0.x - dx1 - dx2, vd0.y + dy1 - dy2);
                AddVertex(vc, vd0.x + dx1 - dx2, vd0.y - dy1 - dy2);
            }
            else
            {
                double da = std::acos(widthAbs_ / (widthAbs_ + 0.125 / approxScale_)) * 2;
                double a1;
                int i;
                int n = int(pi / da);

                da = pi / (n + 1);
                AddVertex(vc, vd0.x - dx1, vd0.y + dy1);
                if (widthSign_ > 0)
                {
                    a1 = std::atan2(dy1, -dx1);
                    a1 += da;
                    for (i = 0; i < n; i++)
                    {
                        AddVertex(vc, vd0.x + std::cos(a1) * width_, vd0.y + std::sin(a1) * width_);
                        a1 += da;
                    }
                }
                else
                {
                    a1 = std::atan2(-dy1, dx1);
                    a1 -= da;
                    for (i = 0; i < n; i++)
                    {
                        AddVertex(vc, vd0.x + std::cos(a1) * width_,
                                  vd0.y + std::sin(a1) * width_);
                        a1 -= da;
                    }
                }
                AddVertex(vc, vd0.x + dx1, vd0.y - dy1);
            }
        }

        /**
         * @brief 计算相交和拐角
         */
        void CalcJoin(VertexConsumer& vc, const vertex_dist& vd0, const vertex_dist& vd1,
                      const vertex_dist& v2, double len1, double len2)
        {
            double dx1 = width_ * (vd1.y - vd0.y) / len1;
            double dy1 = width_ * (vd1.x - vd0.x) / len1;
            double dx2 = width_ * (v2.y - vd1.y) / len2;
            double dy2 = width_ * (v2.x - vd1.x) / len2;

            vc.remove_all();

            double cp = cross_product(vd0.x, vd0.y, vd1.x, vd1.y, v2.x, v2.y);
            if (cp != 0 && (cp > 0) == (width_ > 0))
            {
                double limit = ((len1 < len2) ? len1 : len2) / widthAbs_;
                switch (innerJoinEnum)
                {
                    default:
                        AddVertex(vc, vd1.x + dx1, vd1.y - dy1);
                        AddVertex(vc, vd1.x + dx2, vd1.y - dy2);
                        break;
                    case INNER_MITER:
                        CalcMiter(vc, vd0, vd1, v2, dx1, dy1, dx2, dy2, MITER_JOIN_REVERT, limit, 0);
                        break;
                    case INNER_JAG:
                    case INNER_ROUND:
                        cp = (dx1 - dx2) * (dx1 - dx2) + (dy1 - dy2) * (dy1 - dy2);
                        if (cp < len1 * len1 && cp < len2 * len2)
                        {
                            CalcMiter(vc, vd0, vd1, v2, dx1, dy1, dx2, dy2, MITER_JOIN_REVERT, limit, 0);
                        }
                        else
                        {
                            if (innerJoinEnum == INNER_JAG)
                            {
                                AddVertex(vc, vd1.x + dx1, vd1.y - dy1);
                                AddVertex(vc, vd1.x, vd1.y);
                                AddVertex(vc, vd1.x + dx2, vd1.y - dy2);
                            }
                            else
                            {
                                AddVertex(vc, vd1.x + dx1, vd1.y - dy1);
                                AddVertex(vc, vd1.x, vd1.y);
                                CalcArc(vc, vd1.x, vd1.y, dx2, -dy2, dx1, -dy1);
                                AddVertex(vc, vd1.x, vd1.y);
                                AddVertex(vc, vd1.x + dx2, vd1.y - dy2);
                            }
                        }
                        break;
                }
            }
            else
            {
                double dx = (dx1 + dx2) / 2;
                double dy = (dy1 + dy2) / 2;
                double dbevel = std::sqrt(dx * dx + dy * dy);

                if (lineJoinEnum == ROUND_JOIN || lineJoinEnum == BEVEL_JOIN)
                {
                    if (approxScale_ * (widthAbs_ - dbevel) < widthEps_)
                    {
                        if (calc_intersection(vd0.x + dx1, vd0.y - dy1,
                                              vd1.x + dx1, vd1.y - dy1,
                                              vd1.x + dx2, vd1.y - dy2,
                                              v2.x + dx2, v2.y - dy2,
                                              &dx, &dy))
                        {
                            AddVertex(vc, dx, dy);
                        }
                        else
                        {
                            AddVertex(vc, vd1.x + dx1, vd1.y - dy1);
                        }
                        return;
                    }
                }

                switch (lineJoinEnum)
                {
                    case MITER_JOIN:
                    case MITER_JOIN_REVERT:
                    case MITER_JOIN_ROUND:
                        CalcMiter(vc, vd0, vd1, v2, dx1, dy1, dx2, dy2, lineJoinEnum, miterLimit_, dbevel);
                        break;
                    case ROUND_JOIN:
                        CalcArc(vc, vd1.x, vd1.y, dx1, -dy1, dx2, -dy2);
                        break;

                    default:
                        AddVertex(vc, vd1.x + dx1, vd1.y - dy1);
                        AddVertex(vc, vd1.x + dx2, vd1.y - dy2);
                        break;
                }
            }
        }

    private:
        AGG_INLINE void AddVertex(VertexConsumer& vc, double x, double y)
        {
            vc.add(coord_type(x, y));
        }

        void CalcArc(VertexConsumer& vc,
                     double x, double y,
                     double dx1, double dy1,
                     double dx2, double dy2)
        {
            double a1 = std::atan2(dy1 * widthSign_, dx1 * widthSign_);
            double a2 = std::atan2(dy2 * widthSign_, dx2 * widthSign_);
            double da = a1 - a2;
            int i, n;

            da = std::acos(widthAbs_ / (widthAbs_ + 0.125 / approxScale_)) * 2;

            AddVertex(vc, x + dx1, y + dy1);
            if (widthSign_ > 0)
            {
                if (a1 > a2)
                    a2 += 2 * pi;
                n = int((a2 - a1) / da);
                da = (a2 - a1) / (n + 1);
                a1 += da;
                for (i = 0; i < n; i++)
                {
                    AddVertex(vc, x + std::cos(a1) * width_, y + std::sin(a1) * width_);
                    a1 += da;
                }
            }
            else
            {
                if (a1 < a2)
                    a2 -= 2 * pi;
                n = int((a1 - a2) / da);
                da = (a1 - a2) / (n + 1);
                a1 -= da;
                for (i = 0; i < n; i++)
                {
                    AddVertex(vc, x + std::cos(a1) * width_, y + std::sin(a1) * width_);
                    a1 -= da;
                }
            }
            AddVertex(vc, x + dx2, y + dy2);
        }

        /**
         * @brief 计算斜接长度
         */
        void CalcMiter(VertexConsumer& vc,
                       const vertex_dist& vd0,
                       const vertex_dist& vd1,
                       const vertex_dist& vd2,
                       double dx1, double dy1,
                       double dx2, double dy2,
                       LineJoin linejoin,
                       double mlimit,
                       double dbevel)
        {
            double xi = vd1.x;
            double yi = vd1.y;
            double di = 1;
            double lim = widthAbs_ * mlimit;
            bool miterLimitExceeded = true;
            bool intersectionFailed = true;

            if (calc_intersection(vd0.x + dx1, vd0.y - dy1,
                                  vd1.x + dx1, vd1.y - dy1,
                                  vd1.x + dx2, vd1.y - dy2,
                                  vd2.x + dx2, vd2.y - dy2,
                                  &xi, &yi))
            {
                di = calc_distance(vd1.x, vd1.y, xi, yi);
                if (di <= lim)
                {
                    AddVertex(vc, xi, yi);
                    miterLimitExceeded = false;
                }
                intersectionFailed = false;
            }
            else
            {
                double x2 = vd1.x + dx1;
                double y2 = vd1.y - dy1;
                if ((cross_product(vd0.x, vd0.y, vd1.x, vd1.y, x2, y2) < 0.0) ==
                    (cross_product(vd1.x, vd1.y, vd2.x, vd2.y, x2, y2) < 0.0))
                {
                    AddVertex(vc, vd1.x + dx1, vd1.y - dy1);
                    miterLimitExceeded = false;
                }
            }

            if (miterLimitExceeded)
            {
                switch (linejoin)
                {
                    case MITER_JOIN_REVERT:
                        AddVertex(vc, vd1.x + dx1, vd1.y - dy1);
                        AddVertex(vc, vd1.x + dx2, vd1.y - dy2);
                        break;

                    case MITER_JOIN_ROUND:
                        CalcArc(vc, vd1.x, vd1.y, dx1, -dy1, dx2, -dy2);
                        break;

                    default:
                        if (intersectionFailed)
                        {
                            mlimit *= widthSign_;
                            AddVertex(vc, vd1.x + dx1 + dy1 * mlimit, vd1.y - dy1 + dx1 * mlimit);
                            AddVertex(vc, vd1.x + dx2 - dy2 * mlimit, vd1.y - dy2 - dx2 * mlimit);
                        }
                        else
                        {
                            double x1 = vd1.x + dx1;
                            double y1 = vd1.y - dy1;
                            double x2 = vd1.x + dx2;
                            double y2 = vd1.y - dy2;
                            di = (lim - dbevel) / (di - dbevel);
                            AddVertex(vc, x1 + (xi - x1) * di, y1 + (yi - y1) * di);
                            AddVertex(vc, x2 + (xi - x2) * di, y2 + (yi - y2) * di);
                        }
                        break;
                }
            }
        }

        double width_;
        double widthAbs_;
        double widthEps_;
        int widthSign_;
        double miterLimit_;
        double approxScale_;
        LineCap lineCapEnum;
        LineJoin lineJoinEnum;
        InnerJoin innerJoinEnum;
    };
} // namespace OHOS

#endif
