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

#include "gfx_utils/diagram/common/graphic_common_basics.h"
#include "gfx_utils/diagram/vertexprimitive/graphic_geometry_vertex_sequence.h"

namespace OHOS {
/**
 * @brief dash Segment generator
 * @since 1.0
 * @version 1.0
 */
class GraphicVertexGenerateDash {
#if GRAPHIC_ENABLE_DASH_GENERATE_FLAG
    enum Status {
        INITIAL,
        READY,
        POLYLINE,
        STOP
    };

public:
    typedef GraphicGeometryVertexSequence<VertexDist, BLOCK_SHIFT_SIZE> VertexStorage;

    GraphicVertexGenerateDash();
    /**
     * @brief Remove all dashes
     * @since 1.0
     * @version 1.0
     */
    void RemoveAllDashes();
    /**
     * @brief Add dash
     * @param dashLen segment length
     * @param gapLen Notch length
     * @since 1.0
     * @version 1.0
     */
    void AddDash(float dashLen, float gapLen);
    /**
     * @brief Set dashstart_
     * @param Set value
     * @since 1.0
     * @version 1.0
     */
    void DashStart(float ds);
    /**
     * @brief Set shortcut_
     * @param Set value
     * @since 1.0
     * @version 1.0
     */
    void Shorten(float s)
    {
        shorten_ = s;
    }
    /**
     * @brief Get storten_
     * @return shorten_
     * @since 1.0
     * @version 1.0
     */
    float Shorten() const
    {
        return shorten_;
    }
    /**
     * @brief Remove all vertices
     * @since 1.0
     * @version 1.0
     */
    void RemoveAll();
    /**
     * @brief add vertex
     * @param x x-coordinate
     * @param y y-coordinate
     * @param cmd Instruction type
     * @since 1.0
     * @version 1.0
     */
    void AddVertex(float x, float y, unsigned cmd);

    void Rewind(unsigned pathId);

    unsigned Vertex(float* x, float* y);

    void CompareSize();

    void CompareVertexSize();
private:
    GraphicVertexGenerateDash(const GraphicVertexGenerateDash&);
    const GraphicVertexGenerateDash& operator=(const GraphicVertexGenerateDash&);

    void CalcDashStart(float ds);

    float dashes_[MAX_DASHES];
    float totalDashLen_;
    unsigned numDashes_;
    float dashStart_;
    float shorten_;
    float currDashStart_;
    unsigned currDash_;
    float currRest_;
    const VertexDist* vertexDist1_;
    const VertexDist* vertexDist2_;

    VertexStorage srcVertices_;
    unsigned closed_;
    Status status_;
    unsigned srcVertex_;
#endif
};
} // namespace OHOS

#endif
