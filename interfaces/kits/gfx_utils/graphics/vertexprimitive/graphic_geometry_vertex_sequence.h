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
     * @brief Defines Define the vertexsequence class.
     *
     * @since 1.0
     * @version 1.0
     */
    template <class T, unsigned S = BLOCK_SHIFT_SIZE>
    class VertexSequence : public GeometryPlainDataBlockVector<T, S> {
    public:
        using BaseType = GeometryPlainDataBlockVector<T, S>;
        /**
         * @brief Closed vertex source.
         *
         * @param remove_flag Is it closed.
         * @since 1.0
         * @version 1.0
         */
        void Close(bool remove_flag);

        /**
         * @brief Add a point.
         *
         * @param val vertex.
         * @since 1.0
         * @version 1.0
         */
        void Add(const T& val);

        /**
         * @brief Modify the last vertex.
         *
         * @param t vertex.
         * @since 1.0
         * @version 1.0
         */
        void ModifyLast(const T& val);
    };

    template <class T, unsigned S>
    void VertexSequence<T, S>::Close(bool closed)
    {
        while (BaseType::Size() > 1) {
            if ((*this)[BaseType::Size() - TWO_STEP]((*this)[BaseType::Size() - 1])) {
                break;
            }
            T t = (*this)[BaseType::Size() - 1];
            BaseType::RemoveLast();
            ModifyLast(t);
        }

        if (closed) {
            while (BaseType::Size() > 1) {
                if ((*this)[BaseType::Size() - 1]((*this)[0])) { // 计算两顶点距离
                    break;
                }
                BaseType::RemoveLast();
            }
        }
    }

    template <class T, unsigned S>
    void VertexSequence<T, S>::Add(const T& val)
    {
        if (BaseType::Size() > 1) {
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
        float vertexXCoord;
        float vertexYCoord;
        float vertexDistance;

        VertexDist()
        {}
        /**
         * @brief Construct vertexdist.
         *
         * @param x_,y_ Vertex coordinates.
         * @since 1.0
         * @version 1.0
         */
        VertexDist(float x, float y)
            : vertexXCoord(x), vertexYCoord(y), vertexDistance(0.0)
        {
        }
        /**
         * @brief Calculate whether the distance between the two points is very close.
         *
         * @param vertex.
         * @return If the two points are close, false is returned.
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
         * @brief Construct vertexdist.
         *
         * @param x_,y_ Vertex coordinates, cmd_ Connection command.
         * @since 1.0
         * @version 1.0
         */
        VertexDistCmd(float x_, float y_, unsigned cmd_)
            : VertexDist(x_, y_), cmd(cmd_)
        {
        }
    };
} // namespace OHOS
#endif
