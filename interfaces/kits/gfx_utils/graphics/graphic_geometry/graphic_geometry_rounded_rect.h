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
 * @brief Defines RoundedRect.
 *
 * @since 1.0
 * @version 1.0
 */


#ifndef GRAPHIC_GEOMETRY_ROUNDED_RECT_INCLUDED
#define GRAPHIC_GEOMETRY_ROUNDED_RECT_INCLUDED

#include "graphic_geometry_arc.h"
#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"

namespace OHOS {
    /**
     * @file graphic_geometry_bounding_rect.h
     *
     * @brief Defines 圆角矩形类.
     *
     * @since 1.0
     * @version 1.0
     */
    class RoundedRect : public HeapBase {
    public:
        /**
         * @brief 构造圆角矩形.
         *
         * @param x1,y1 矩形左上角顶点,x2,y2 矩形右下角顶点,r 圆角半径.
         * @since 1.0
         * @version 1.0
         */
        RoundedRect(double x1, double y1, double x2, double y2, double r);

        RoundedRect()
        {}

        /**
         * @brief 设置圆角矩形顶点圆弧的横向半径与纵向半径.
         *
         * @param rx横向半径,纵向半径.
         * @since 1.0
         * @version 1.0
         */
        void Radius(double rx, double ry);

        /**
         * @brief 设置圆角矩形四个顶点圆弧的横向半径与纵向半径.
         *
         * @param rxBottom下方两个点横向半径,ryBottom下方两个点纵向半径,rxTop上方两个点横向半径,ryTop上方两个点纵向半径.
         * @since 1.0
         * @version 1.0
         */
        void Radius(double rxBottom, double ryBottom, double rxTop, double ryTop);

        /**
         * @brief 分别设置圆角矩形四个顶点圆弧的横向半径与纵向半径.
         *
         * @param 分别对应四个顶点,rx 横向半径,ry 纵向半径.
         * @since 1.0
         * @version 1.0
         */
        void Radius(double rx1, double ry1, double rx2, double ry2,
                    double rx3, double ry3, double rx4, double ry4);

        /**
         * @brief 设置顶点弧形的伸缩比
         *
         * @param 伸缩比.
         * @since 1.0
         * @version 1.0
         */
        void ApproximationScale(double scale)
        {
            arc_.ApproximationScale(scale);
        }

        /**
         * @brief 设置矩形的左上角与右下角两个顶点.
         *
         * @param x1与y1左上角顶点,x2与y2右下角顶点.
         * @since 1.0
         * @version 1.0
         */
        void Rect(double x1, double y1, double x2, double y2);

        /**
         * @brief 设置圆角矩形顶点圆弧的半径.
         *
         * @param r 半径.
         * @since 1.0
         * @version 1.0
         */
        void Radius(double r);

        /**
         * @brief 规范化圆角矩形的弧形半径.
         *
         * @since 1.0
         * @version 1.0
         */
        void NormalizeRadius();

        /**
         * @brief 回到最开始步骤.
         *
         * @since 1.0
         * @version 1.0
         */
        void Rewind(unsigned);

        /**
         * @brief 获取顶点弧形的伸缩比
         * @return 伸缩比.
         * @since 1.0
         * @version 1.0
         */
        double ApproximationScale() const
        {
            return arc_.ApproximationScale();
        }

        /**
         * @brief 取出顶点源用于坐标转换重组通道.
         * @param x,y 顶点源
         * @return 连接命令
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(double* x, double* y);

    private:
        enum RoundRectVertexStatus {
            VERTEX_STATUS = 0,
            VERTEX_STATUS1,
            VERTEX_STATUS2,
            VERTEX_STATUS3,
            VERTEX_STATUS4,
            VERTEX_STATUS5,
            VERTEX_STATUS6,
            VERTEX_STATUS7,
            VERTEX_STATUS8
        };

        unsigned status_;
        Arc arc_;
        double x1_;
        double y1_;
        double x2_;
        double y2_;
        double rx1_;
        double ry1_;
        double rx2_;
        double ry2_;
        double rx3_;
        double ry3_;
        double rx4_;
        double ry4_;
    };
} // namespace OHOS

#endif
