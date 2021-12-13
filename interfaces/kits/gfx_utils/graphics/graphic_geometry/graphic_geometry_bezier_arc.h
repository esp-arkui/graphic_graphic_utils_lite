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
 * @addtogroup GraphicGeometry
 * @{
 *
 * @brief Defines ArcToBezier BezierArc BezierArcSvg.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_BEZIER_ARC_INCLUDED
#define GRAPHIC_GEOMETRY_BEZIER_ARC_INCLUDED

#include "gfx_utils/graphics/graphic_depict/graphic_depict_transform.h"
#include "gfx_utils/heap_base.h"
namespace OHOS {
    const int BEZIER_ARC_VERTEX_NUM = 26; // 贝塞尔弧顶点坐标数
    /**
     * @brief 弧形转换为贝塞尔曲线.
     *
     * @since 1.0
     * @version 1.0
     */
    void ArcToBezier(double cx, double cy, double rx, double ry,
                     double startAngle, double sweepAngle,
                     double* curve);

    /**
     * @file graphic_geometry_bezier_arc.h
     *
     * @brief Defines 贝塞尔弧结构类.
     *
     * @since 1.0
     * @version 1.0
     */
    class BezierArc : public HeapBase {
    public:
        BezierArc()
            :vertex_(BEZIER_ARC_VERTEX_NUM), numVertices_(0), cmd_(PATH_CMD_LINE_TO)
        {}

        /**
         * @brief 使用弧线初始化贝塞尔曲线.
         * @Param centerX centerY 圆弧圆心，rx ry 圆弧横纵半径，startAngle sweepAngle 圆弧起止角度
         * @since 1.0
         * @version 1.0
         */
        BezierArc(double centerX, double centerY,
                  double rx, double ry,
                  double startAngle,
                  double sweepAngle)
        {
            Init(centerX, centerY, rx, ry, startAngle, sweepAngle);
        }

        /**
         * @brief 使用弧线初始化贝塞尔曲线.
         * @since 1.0
         * @version 1.0
         */
        void Init(double x, double y,
                  double rx, double ry,
                  double startAngle,
                  double sweepAngle);

        /**
         * @brief 回到最开始步骤.
         *
         * @since 1.0
         * @version 1.0
         */
        void Rewind(unsigned)
        {
            vertex_ = 0;
        }

        /**
         * @brief 在采样阶段调用
         * @param x为点坐标x的值的指针，y为点坐标y的值的指针
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(double* x, double* y)
        {
            if (vertex_ >= numVertices_) {
                return PATH_CMD_STOP;
            }
            *x = vertices_[vertex_];
            *y = vertices_[vertex_ + 1];
            vertex_ = vertex_ + OHOS::TWO_STEP;
            if (vertex_ == OHOS::TWO_STEP) {
                return unsigned(PATH_CMD_MOVE_TO);
            } else {
                return cmd_;
            }
        }

        /**
         * @brief 获取顶点源个数.
         * @return 顶点个数
         * @since 1.0
         * @version 1.0
         */
        unsigned Nuvertices() const
        {
            return numVertices_;
        }

        /**
         * @brief 获取顶点源.
         * @return 顶点源数据
         * @since 1.0
         * @version 1.0
         */
        const double* Vertices() const
        {
            return vertices_;
        }

        /**
         * @brief 获取顶点源.
         * @return 顶点源数据
         * @since 1.0
         * @version 1.0
         */
        double* Vertices()
        {
            return vertices_;
        }

    private:
        unsigned vertex_;
        unsigned numVertices_;
        double vertices_[BEZIER_ARC_VERTEX_NUM];
        unsigned cmd_;
    };

    /**
     * @brief 计算SVG样式的贝塞尔弧.
     *
     * @since 1.0
     * @version 1.0
     */
    class BezierArcSvg : public HeapBase {
    public:
        BezierArcSvg() :
            arc_(), radiiOK_(false)
        {}

        /**
         * @brief 计算从（x1，y1）到（x2，y2）的椭圆弧.
         * 椭圆的方向由两个半径（rx，ry）定义
         * @param x1,y1 起点坐标,x1,y1 终点坐标, rx,ry 长短轴半径,angle 角度,
         * largeArcFlag 大弧线标志,sweepFlag 扫掠角标志
         * @since 1.0
         * @version 1.0
         */
        BezierArcSvg(double x1,
                     double y1,
                     double rx, double ry,
                     double angle,
                     bool largeArcFlag, // 大弧线标志
                     bool sweepFlag,    // 扫掠角标志
                     double x2, double y2) :
            arc_(),
            radiiOK_(false)
        {
            Init(x1, y1, rx, ry, angle, largeArcFlag, sweepFlag, x2, y2);
        }

        bool RadiiOK() const
        {
            return radiiOK_;
        }

        void Init(double x1,
                  double y1,
                  double rx,
                  double ry,
                  double angle,
                  bool largeArcFlag,
                  bool sweepFlag,
                  double x2,
                  double y2);

        /**
        * @brief 回到最开始步骤.
        *
        * @since 1.0
        * @version 1.0
        */
        void Rewind(unsigned)
        {
            arc_.Rewind(0);
        }

        /**
         * @brief 在采样阶段调用
         * @param x为点坐标x的值的指针，y为点坐标y的值的指针
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(double* x, double* y)
        {
            return arc_.Vertex(x, y);
        }

        /**
         * @brief 返回贝塞尔弧的顶点数据。
         * @return 返回顶点源
         * @since 1.0
         * @version 1.0
         */
        const double* Vertices() const
        {
            return arc_.Vertices();
        }

        /**
         * @brief 返回贝塞尔弧的顶点数据.
         * @return 返回贝塞尔弧的顶点数据.
         * @since 1.0
         * @version 1.0
         */
        double* Vertices()
        {
            return arc_.Vertices();
        }

        /**
         * @brief 返回两倍的值顶点数。也就是说，对于1个顶点，它返回2。
         * @since 1.0
         * @version 1.0
         */
        unsigned NumVertices_() const
        {
            return arc_.Nuvertices();
        }

    private:
        BezierArc arc_;
        bool radiiOK_;
    };
} // namespace OHOS

#endif
