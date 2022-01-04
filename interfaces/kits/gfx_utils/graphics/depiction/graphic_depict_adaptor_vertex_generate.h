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
* @file graphic_geometry_depict_adaptor_vcgen.h
*
* @brief Defines 构建几何图元顶点生成过程适配器.
* 利用坐标转换管道，改变顶点源产生的顶点，包括坐标、命令、产生新顶点等
* @since 1.0
* @version 1.0
*/

#ifndef GRAPHIC_GEOMETRY_DEPICT_ADAPTOR_VERTEX_GENERATE_INCLUDED
#define GRAPHIC_GEOMETRY_DEPICT_ADAPTOR_VERTEX_GENERATE_INCLUDED
#include <typeinfo>

#include "gfx_utils/graphics/common/graphic_common_basics.h"
#include "gfx_utils/graphics/vertexgenerate/graphic_vertex_generate_dash.h"

namespace OHOS {
    /**
     * @struct EmptyMarkers
     * @brief 构建默认空的几何图元结构体.
     * 该结构体主要是为了给stroke等图元设置默认空标记符号所用
     * @see template<class VertexSource, class Markers = EmptyMarkers>
     * struct DepictStroke
     * @since 1.0
     * @version 1.0
     */
    struct EmptyMarkers {
        /**
         * @brief 清空所有的marker符号元素.
         * @since 1.0
         * @version 1.0
         */
        void RemoveAll()
        {}

        /**
         * @brief 添加或插入marker符号元素.
         * @since 1.0
         * @version 1.0
         */
        void AddVertex(double, double, unsigned)
        {}

        /**
         * @brief 利用该结构体构建Markers符号之前做相关的准备工作.
         * @since 1.0
         * @version 1.0
         */
        void PrepareSrc()
        {}

        /**
         * @brief 一堆操作处理重新设置新的状态.
         * @since 1.0
         * @version 1.0
         */
        void Rewind(unsigned)
        {}

        /**
         * @brief 根据不同而状态移动顶点或者产生新的顶点.
         * 返回当前的操作状态,做后续处理
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(double*, double*)
        {
            return PATH_CMD_STOP;
        }
    };

    /**
     * @template class DepictAdaptorVertexGenerator
     * @brief 该模板类是顶点生成适配器的基类.
     * 主要是利用顶点构建和生成阶段
     * 改变顶点源产生的顶点，包括坐标、命令、产生新顶点等。
     * template 参数包括顶点源，特定图元生成器，标记符号等
     * @since 1.0
     * @version 1.0
     */
    template <class VertexSource,
              class Generator,
              class Markers = EmptyMarkers>
    class DepictAdaptorVertexGenerator {
        /**
         * @brief 设置顶点适配生成器的状态.
         * 基本包括初始化准备,累积生成基点，生成差值新点状态
         * @since 1.0
         * @version 1.0
         */
        enum VertexGeneratorEnum {
            INITIAL,
            ACCUMULATE,
            GENERATE
        };

    public:
        /**
         * @brief template class DepictAdaptorVertexGenerator的构造函数.
         * 初始化顶点源类，并置状态为初始状态
         * @since 1.0
         * @version 1.0
         */
        explicit DepictAdaptorVertexGenerator(VertexSource& source)
            : m_source(&source), m_status(INITIAL)
        {}

        /**
         * @brief 直接设置顶点源.
         * @since 1.0
         * @version 1.0
         */
        void Attach(VertexSource& source)
        {
            m_source = &source;
        }

        /**
         * @brief 返回可变生成器引用
         * @since 1.0
         * @version 1.0
         */
        Generator& GetGenerator()
        {
            return m_generator;
        }

        /**
         * @brief 返回不可变生成器引用
         * @since 1.0
         * @version 1.0
         */
        const Generator& GetGenerator() const
        {
            return m_generator;
        }

        /**
         * @brief 返回可变标记符号引用
         * @since 1.0
         * @version 1.0
         */
        Markers& GetMarkers()
        {
            return m_markers;
        }

        /**
         * @brief 返回不可变标记符号引用
         * @since 1.0
         * @version 1.0
         */
        const Markers& GetMarkers() const
        {
            return m_markers;
        }

        /**
         * @brief 重置顶点状态
         * @since 1.0
         * @version 1.0
         */
        void Rewind(unsigned path_id)
        {
            m_source->Rewind(path_id);
            m_status = INITIAL;
        }

        /**
         * @brief 根据不同而状态移动顶点或者产生新的顶点.
         * 返回当前的操作状态,做后续处理
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(double* x, double* y);

    private:
        DepictAdaptorVertexGenerator(const DepictAdaptorVertexGenerator<VertexSource, Generator, Markers>&);
        const DepictAdaptorVertexGenerator<VertexSource, Generator, Markers>&
            operator=(const DepictAdaptorVertexGenerator<VertexSource, Generator, Markers>&);

        VertexSource* m_source;
        Generator m_generator;
        Markers m_markers;
        VertexGeneratorEnum m_status;
        unsigned m_last_cmd;
        double m_start_x;
        double m_start_y;
    };

    /**
    * @brief 根据不同而状态移动顶点或者产生新的顶点.
    * 返回当前的操作状态,做后续处理
    * @since 1.0
    * @version 1.0
    */
    template <class VertexSource, class Generator, class Markers>
    unsigned DepictAdaptorVertexGenerator<VertexSource, Generator, Markers>::Vertex(double* x, double* y)
    {
        unsigned cmd = PATH_CMD_STOP;
        bool done = false;
        while (!done) {
            switch (m_status) {
                case INITIAL:
                    m_last_cmd = m_source->Vertex(&m_start_x, &m_start_y);
                    m_markers.RemoveAll();
                    m_status = ACCUMULATE;
                    break;
                case ACCUMULATE:
                    if (IsStop(m_last_cmd))
                        return PATH_CMD_STOP;

                    m_markers.AddVertex(m_start_x, m_start_y, PATH_CMD_MOVE_TO);
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
                    m_generator.RemoveAll();
                    m_generator.AddVertex(m_start_x, m_start_y, PATH_CMD_MOVE_TO);

#else
                    if (typeid(m_generator) != typeid(VertexGenerateDash)) {
                        m_generator.RemoveAll();
                        m_generator.AddVertex(m_start_x, m_start_y, PATH_CMD_MOVE_TO);
                    }
#endif
                    while (1) {
                        cmd = m_source->Vertex(x, y);
                        if (IsVertex(cmd)) {
                            m_last_cmd = cmd;
                            if (IsMoveTo(cmd)) {
                                m_start_y = *y;
                                m_start_x = *x;
                                break;
                            }
                            m_markers.AddVertex(*x, *y, PATH_CMD_LINE_TO);
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
                            m_generator.AddVertex(*x, *y, cmd);
#else
                            if (typeid(m_generator) != typeid(VertexGenerateDash)) {
                                m_generator.AddVertex(*x, *y, cmd);
                            }
#endif
                        } else {
                            if (IsStop(cmd)) {
                                m_last_cmd = PATH_CMD_STOP;
                                break;
                            }
                            if (IsEndPoly(cmd)) {
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
                                m_generator.AddVertex(*x, *y, cmd);
#else
                                if (typeid(m_generator) != typeid(VertexGenerateDash)) {
                                    m_generator.AddVertex(*x, *y, cmd);
                                }

#endif
                                break;
                            }
                        }
                    }
                    if (typeid(m_generator) != typeid(VertexGenerateDash)) {
                        m_generator.Rewind(0);
                    }
                    m_status = GENERATE;
                    break;
                case GENERATE:
                    cmd = m_generator.Vertex(x, y);
                    if (IsStop(cmd)) {
                        m_status = ACCUMULATE;
                        break;
                    }
                    done = true;
                    break;
                default: break;
            }
        }
        return cmd;
    }
} // namespace OHOS
#endif
