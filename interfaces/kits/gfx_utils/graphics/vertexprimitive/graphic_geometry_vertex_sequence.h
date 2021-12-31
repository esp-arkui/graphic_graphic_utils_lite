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
 * @brief Defines VertexSequence VertexDist VertexDistCmd.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_VERTEX_SEQUENCE_INCLUDED
#define GRAPHIC_GEOMETRY_VERTEX_SEQUENCE_INCLUDED

#include "gfx_utils/graphics/common/graphic_common_basics.h"
#include "gfx_utils/graphics/common/graphic_common_math.h"
#include "graphic_geometry_plaindata_blockvector.h"
namespace OHOS {
    /**
     * @file graphic_geometry_vertex_sequence.h
     *
     * @brief Defines 定义VertexSequence类.
     *
     * @since 1.0
     * @version 1.0
     */
    template <class T, unsigned S = BLOCK_SHIFT_SIZE>
    class VertexSequence : public GeometryPlainDataBlockVector<T, S> {
    public:
        using BaseType = GeometryPlainDataBlockVector<T, S>;
        /**
         * @brief 封闭顶点源.
         *
         * @param remove_flag 是否封闭.
         * @since 1.0
         * @version 1.0
         */
        void Close(bool remove_flag);

        /**
         * @brief 增加一个点.
         *
         * @param val 顶点.
         * @since 1.0
         * @version 1.0
         */
        void Add(const T& val);

        /**
         * @brief 修改最后一个顶点.
         *
         * @param t 顶点.
         * @since 1.0
         * @version 1.0
         */
        void ModifyLast(const T& val);
    };

    template <class T, unsigned S>
    void VertexSequence<T, S>::Close(bool closed)
    {
        while (1 < BaseType::Size()) {
            if ((*this)[BaseType::Size() - TWO_STEP]((*this)[BaseType::Size() - 1])) {
                break;
            }
            T t = (*this)[BaseType::Size() - 1];
            BaseType::RemoveLast();
            ModifyLast(t);
        }

        if (closed) {
            while (1 < BaseType::Size()) {
                if ((*this)[BaseType::Size() - 1]((*this)[0])) { //计算两顶点距离
                    break;
                }
                BaseType::RemoveLast();
            }
        }
    }

    template <class T, unsigned S>
    void VertexSequence<T, S>::Add(const T& val)
    {
        if (1 < BaseType::Size()) {
            if (!(*this)[BaseType::Size() - TWO_STEP]((*this)[BaseType::Size() - 1])) {
                BaseType::RemoveLast();
            }
        }
        BaseType::Add(val);
    }

    template <class T, unsigned S>
    void VertexSequence<T, S>::ModifyLast(const T& val)
    {
        BaseType::RemoveLast();
        Add(val);
    }

    struct VertexDist {
        double vertexXCoord;
        double vertexYCoord;
        double vertexDistance;

        VertexDist()
        {}
        /**
         * @brief 构造VertexDist.
         *
         * @param x_,y_ 顶点坐标.
         * @since 1.0
         * @version 1.0
         */
        VertexDist(double x, double y) :
            vertexXCoord(x), vertexYCoord(y), vertexDistance(0.0)
        {
        }
        /**
         * @brief 计算两个点的距离是否很相近.
         *
         * @param 顶点.
         * @return 两点距离很近返回false.
         * @since 1.0
         * @version 1.0
         */
        bool operator()(const VertexDist& val)
        {
            vertexDistance = CalcDistance(vertexXCoord, vertexYCoord, val.vertexXCoord, val.vertexYCoord);
            bool ret = vertexDistance > VERTEX_DIST_EPSILON;
            if (!ret) {
                vertexDistance = 1.0 / VERTEX_DIST_EPSILON;
            }
            return ret;
        }
    };

    struct VertexDistCmd : public VertexDist {
        unsigned cmd;

        VertexDistCmd()
        {}
        /**
     * @brief 构造VertexDist.
     *
     * @param x_,y_ 顶点坐标, cmd_ 连接命令.
     * @since 1.0
     * @version 1.0
     */
        VertexDistCmd(double x_, double y_, unsigned cmd_) :
            VertexDist(x_, y_), cmd(cmd_)
        {
        }
    };

} // namespace OHOS

#endif
