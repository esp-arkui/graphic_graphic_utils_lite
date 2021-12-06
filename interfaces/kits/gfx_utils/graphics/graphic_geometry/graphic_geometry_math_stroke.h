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
* @file graphic_geometry_math_stroke.h
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
        MITER_JOIN         = 0,
        MITER_JOIN_REVERT  = 1,
        /** 创建圆角 */
        ROUND_JOIN         = 2,
        /** 创建斜角 */
        BEVEL_JOIN         = 3,
        MITER_JOIN_ROUND   = 4
    };

    enum InnerJoin
    {
        INNER_BEVEL,
        INNER_MITER,
        INNER_JAG,
        INNER_ROUND
    };

    template<class VertexConsumer> class math_stroke
    {
    public:
        typedef typename VertexConsumer::value_type coord_type;
        math_stroke() :
                v_width(0.5),
                widthAbs(0.5),
                widthEps(0.5/1024.0),
                widthSign(1),
                miterLimit(4.0),
                approxScale(1.0),
                lineCap(BUTT_CAP),
                lineJoin(MITER_JOIN),
                innerJoin(INNER_MITER)
            {}

        /**
         * @brief line_cap 定义线条的结束端点样式
         */
        void line_cap(LineCap lineCapE)
        {
            lineCap = lineCapE;
        }
        /**
         * @brief line_join 定义两条线相交时，所创建的拐角类型
         */
        void line_join(LineJoin lineJoinE)
        {
            lineJoin = lineJoinE;
        }

        void inner_join(InnerJoin innerJoinE)
        {
            innerJoin = innerJoinE;
        }

        LineCap line_cap() const
        {
            return lineCap;
        }

        LineJoin line_join() const
        {
            return lineJoin;
        }

        InnerJoin inner_join() const
        {
            return innerJoin;
        }

        /**
         * @brief width 设置区域宽
         */
        void width(double w)
        {
            v_width = w * 0.5;
            if(v_width < 0)
            {
                widthAbs  = -v_width;
                widthSign = -1;
            }
            else
            {
                widthAbs  = v_width;
                widthSign = 1;
            }
            widthEps = v_width / 1024.0;
        }

        /**
         * @brief miter_limit 设置最大斜接长度
         */
        void miter_limit(double v_miterLimit)
        {
            miterLimit = v_miterLimit;
        }

        /**
         * @brief 添加近似值
         */
        void approximation_scale(double approximationScale)
        {
            approxScale = approximationScale;
        }

        /**
         * @brief width 返回宽度
         */
        double width() const
        {
            return v_width * 2.0;
        }

        /**
         * @brief miter_limit 返回最大斜接长度
         */
        double miter_limit() const
        {
            return miterLimit;
        }

        /**
         * @brief 返回设定的近似值
         */
        double approximation_scale() const
        {
            return approxScale;
        }

        /**
         * @brief 计算端点样式
         */
        void calc_cap(VertexConsumer& vc,const vertex_dist& vd0,const vertex_dist& vd1,double len)
        {
            vc.remove_all();

            double dx1 = (vd1.y - vd0.y) / len;
            double dy1 = (vd1.x - vd0.x) / len;
            double dx2 = 0;
            double dy2 = 0;

            dx1 *= v_width;
            dy1 *= v_width;

            if(lineCap != ROUND_CAP)
            {
                if(lineCap == SQUARE_CAP)
                {
                    dx2 = dy1 * widthSign;
                    dy2 = dx1 * widthSign;
                }
                add_vertex(vc, vd0.x - dx1 - dx2, vd0.y + dy1 - dy2);
                add_vertex(vc, vd0.x + dx1 - dx2, vd0.y - dy1 - dy2);
            }
            else
            {
                double da = std::acos(widthAbs / (widthAbs + 0.125 / approxScale)) * 2;
                double a1;
                int i;
                int n = int(pi / da);

                da = pi / (n + 1);
                add_vertex(vc, vd0.x - dx1, vd0.y + dy1);
                if(widthSign > 0)
                {
                    a1 = std::atan2(dy1, -dx1);
                    a1 += da;
                    for(i = 0; i < n; i++)
                    {
                        add_vertex(vc, vd0.x + std::cos(a1) * v_width,vd0.y + std::sin(a1) * v_width);
                        a1 += da;
                    }
                }
                else
                {
                    a1 = std::atan2(-dy1, dx1);
                    a1 -= da;
                    for(i = 0; i < n; i++)
                    {
                        add_vertex(vc, vd0.x + std::cos(a1) * v_width,
                                       vd0.y + std::sin(a1) * v_width);
                        a1 -= da;
                    }
                }
                add_vertex(vc, vd0.x + dx1, vd0.y - dy1);
            }
        }

        /**
         * @brief 计算相交和拐角
         */
        void calc_join(VertexConsumer& vc,const vertex_dist& v0,const vertex_dist& v1,
                       const vertex_dist& v2,double len1,double len2)
        {
            double dx1 = v_width * (v1.y - v0.y) / len1;
            double dy1 = v_width * (v1.x - v0.x) / len1;
            double dx2 = v_width * (v2.y - v1.y) / len2;
            double dy2 = v_width * (v2.x - v1.x) / len2;

            vc.remove_all();

            double cp = cross_product(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);
            if(cp != 0 && (cp > 0) == (v_width > 0))
            {
                double limit = ((len1 < len2) ? len1 : len2) / widthAbs;
                switch(innerJoin)
                {
                default:
                    add_vertex(vc, v1.x + dx1, v1.y - dy1);
                    add_vertex(vc, v1.x + dx2, v1.y - dy2);
                    break;
                case INNER_MITER:
                    calc_miter(vc,v0, v1, v2, dx1, dy1, dx2, dy2,MITER_JOIN_REVERT,limit, 0);
                    break;
                case INNER_JAG:
                case INNER_ROUND:
                    cp = (dx1-dx2) * (dx1-dx2) + (dy1-dy2) * (dy1-dy2);
                    if(cp < len1 * len1 && cp < len2 * len2)
                    {
                        calc_miter(vc,v0, v1, v2, dx1, dy1, dx2, dy2,MITER_JOIN_REVERT,limit, 0);
                    }
                    else
                    {
                        if(innerJoin == INNER_JAG)
                        {
                            add_vertex(vc, v1.x + dx1, v1.y - dy1);
                            add_vertex(vc, v1.x,       v1.y      );
                            add_vertex(vc, v1.x + dx2, v1.y - dy2);
                        }
                        else
                        {
                            add_vertex(vc, v1.x + dx1, v1.y - dy1);
                            add_vertex(vc, v1.x,       v1.y      );
                            calc_arc(vc, v1.x, v1.y, dx2, -dy2, dx1, -dy1);
                            add_vertex(vc, v1.x,       v1.y      );
                            add_vertex(vc, v1.x + dx2, v1.y - dy2);
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

                if(lineJoin == ROUND_JOIN || lineJoin == BEVEL_JOIN)
                {
                    if(approxScale * (widthAbs - dbevel) < widthEps)
                    {
                        if(calc_intersection(v0.x + dx1, v0.y - dy1,
                                             v1.x + dx1, v1.y - dy1,
                                             v1.x + dx2, v1.y - dy2,
                                             v2.x + dx2, v2.y - dy2,
                                             &dx, &dy))
                        {
                            add_vertex(vc, dx, dy);
                        }
                        else
                        {
                            add_vertex(vc, v1.x + dx1, v1.y - dy1);
                        }
                        return;
                    }
                }

                switch(lineJoin)
                {
                case MITER_JOIN:
                case MITER_JOIN_REVERT:
                case MITER_JOIN_ROUND:
                    calc_miter(vc,v0, v1, v2, dx1, dy1, dx2, dy2,lineJoin,miterLimit,dbevel);
                    break;
                case ROUND_JOIN:
                    calc_arc(vc, v1.x, v1.y, dx1, -dy1, dx2, -dy2);
                    break;

                default:
                    add_vertex(vc, v1.x + dx1, v1.y - dy1);
                    add_vertex(vc, v1.x + dx2, v1.y - dy2);
                    break;
                }
            }
        }

    private:
        AGG_INLINE void add_vertex(VertexConsumer& vc, double x, double y)
        {
            vc.add(coord_type(x, y));
        }

        void calc_arc(VertexConsumer& vc,
                      double x,   double y,
                      double dx1, double dy1,
                      double dx2, double dy2)
        {
            double a1 = std::atan2(dy1 * widthSign, dx1 * widthSign);
            double a2 = std::atan2(dy2 * widthSign, dx2 * widthSign);
            double da = a1 - a2;
            int i, n;

            da = std::acos(widthAbs / (widthAbs + 0.125 / approxScale)) * 2;

            add_vertex(vc, x + dx1, y + dy1);
            if(widthSign > 0)
            {
                if(a1 > a2) a2 += 2 * pi;
                n = int((a2 - a1) / da);
                da = (a2 - a1) / (n + 1);
                a1 += da;
                for(i = 0; i < n; i++)
                {
                    add_vertex(vc, x + std::cos(a1) * v_width, y + std::sin(a1) * v_width);
                    a1 += da;
                }
            }
            else
            {
                if(a1 < a2) a2 -= 2 * pi;
                n = int((a1 - a2) / da);
                da = (a1 - a2) / (n + 1);
                a1 -= da;
                for(i = 0; i < n; i++)
                {
                    add_vertex(vc, x + std::cos(a1) * v_width, y + std::sin(a1) * v_width);
                    a1 -= da;
                }
            }
            add_vertex(vc, x + dx2, y + dy2);
        }

        /**
         * @brief 计算斜接长度
         */
        void calc_miter(VertexConsumer& vc,
                        const vertex_dist& v0, 
                        const vertex_dist& v1, 
                        const vertex_dist& v2,
                        double dx1, double dy1, 
                        double dx2, double dy2,
                        LineJoin lj,
                        double mlimit,
                        double dbevel)
        {
            double xi  = v1.x;
            double yi  = v1.y;
            double di  = 1;
            double lim = widthAbs * mlimit;
            bool miter_limit_exceeded = true;
            bool intersection_failed  = true;

            if(calc_intersection(v0.x + dx1, v0.y - dy1,
                                 v1.x + dx1, v1.y - dy1,
                                 v1.x + dx2, v1.y - dy2,
                                 v2.x + dx2, v2.y - dy2,
                                 &xi, &yi))
            {
                di = calc_distance(v1.x, v1.y, xi, yi);
                if(di <= lim)
                {
                    add_vertex(vc, xi, yi);
                    miter_limit_exceeded = false;
                }
                intersection_failed = false;
            }
            else
            {
                double x2 = v1.x + dx1;
                double y2 = v1.y - dy1;
                if((cross_product(v0.x, v0.y, v1.x, v1.y, x2, y2) < 0.0) ==
                   (cross_product(v1.x, v1.y, v2.x, v2.y, x2, y2) < 0.0))
                {
                    add_vertex(vc, v1.x + dx1, v1.y - dy1);
                    miter_limit_exceeded = false;
                }
            }

            if(miter_limit_exceeded)
            {
                switch(lj)
                {
                case MITER_JOIN_REVERT:
                    add_vertex(vc, v1.x + dx1, v1.y - dy1);
                    add_vertex(vc, v1.x + dx2, v1.y - dy2);
                    break;

                case MITER_JOIN_ROUND:
                    calc_arc(vc, v1.x, v1.y, dx1, -dy1, dx2, -dy2);
                    break;

                default:
                    if(intersection_failed)
                    {
                        mlimit *= widthSign;
                        add_vertex(vc, v1.x + dx1 + dy1 * mlimit, v1.y - dy1 + dx1 * mlimit);
                        add_vertex(vc, v1.x + dx2 - dy2 * mlimit, v1.y - dy2 - dx2 * mlimit);
                    }
                    else
                    {
                        double x1 = v1.x + dx1;
                        double y1 = v1.y - dy1;
                        double x2 = v1.x + dx2;
                        double y2 = v1.y - dy2;
                        di = (lim - dbevel) / (di - dbevel);
                        add_vertex(vc, x1 + (xi - x1) * di, y1 + (yi - y1) * di);
                        add_vertex(vc, x2 + (xi - x2) * di, y2 + (yi - y2) * di);
                    }
                    break;
                }
            }
        }

        double       v_width;
        double       widthAbs;
        double       widthEps;
        int          widthSign;
        double       miterLimit;
        double       approxScale;
        LineCap   lineCap;
        LineJoin  lineJoin;
        InnerJoin innerJoin;
    };
}

#endif
