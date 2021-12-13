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
#ifndef GRAPHIC_VCGEN_DASH_INCLUDED
#define GRAPHIC_VCGEN_DASH_INCLUDED

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"
#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_vertex_sequence.h"

namespace OHOS {
    class VCGenDash {
        enum MaxDashes
        {
            MAX_DASHES = 32
        };

        enum Status
        {
            INITIAL,
            READY,
            POLYLINE,
            STOP
        };

    public:
        typedef VertexSequence<VertexDist, 6> VertexStorage;

        VCGenDash();
        /**
         * @brief 去除所有破折号
         * 
         * @since 1.0
         * @version 1.0
         */
        void RemoveAllDashes();
        /**
         * @brief 添加破折号
         * 
         * @param dashLen 线段长度
         * @param gapLen 缺口长度
         * @since 1.0
         * @version 1.0
         */
        void AddDash(double dashLen, double gapLen);
        /**
         * @brief 设定dashStart_
         * 
         * @param 设定值
         * @since 1.0
         * @version 1.0
         */
        void DashStart(double ds);
        /**
         * @brief 设定shorten_
         * 
         * @param 设定值
         * @since 1.0
         * @version 1.0
         */
        void Shorten(double s)
        {
            shorten_ = s;
        }
        /**
         * @brief 获取storten_
         * 
         * @return shorten_
         * @since 1.0
         * @version 1.0
         */
        double Shorten() const
        {
            return shorten_;
        }
        /**
         * @brief 去除所有顶点
         * 
         * @since 1.0
         * @version 1.0
         */
        void RemoveAll();
        /**
         * @brief 添加顶点
         * 
         * @param x x轴坐标
         * @param y y轴坐标
         * @param cmd 指令类型
         * @since 1.0
         * @version 1.0
         */
        void AddVertex(double x, double y, unsigned cmd);

        void Rewind(unsigned pathId);

        unsigned Vertex(double* x, double* y);

    private:
        VCGenDash(const VCGenDash&);
        const VCGenDash& operator=(const VCGenDash&);

        void CalcDashStart(double ds);

        double dashes_[MAX_DASHES];
        double totalDashLen_;
        unsigned numDashes_;
        double dashStart_;
        double shorten_;
        double currDashStart_;
        unsigned currDash_;
        double currRest;
        const VertexDist* vertexDist1_;
        const VertexDist* vertexDist2_;

        VertexStorage srcVertices_;
        unsigned closed_;
        Status status_;
        unsigned srcVertex_;
    };

} // namespace OHOS

#endif
