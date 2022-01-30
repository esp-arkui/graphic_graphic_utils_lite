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

#ifndef GRAPHIC_VCGEN_STROKE_INCLUDED
#define GRAPHIC_VCGEN_STROKE_INCLUDED

#include "gfx_utils/diagram/vertexprimitive/graphic_geometry_math_stroke.h"
#include "draw/clip_utils.h"
namespace OHOS {
class GraphicVertexGenerateStroke {
    enum Status {
        INITIAL,
        READY,
        CAP1,
        CAP2,
        OUTLINE1,
        CLOSE_FIRST,
        OUTLINE2,
        OUT_VERTICES,
        END_POLY1,
        END_POLY2,
        STOP
    };

public:
    using VertexStorage = GraphicGeometryVertexSequence<VertexDist, BLOCK_SHIFT_SIZE>;
    using CoordStorage = GraphicGeometryPlainDataBlockVector<PointF, BLOCK_SHIFT_SIZE>;

    GraphicVertexGenerateStroke();

#if GRAPHIC_ENABLE_LINECAP_FLAG
    void LineCap(LineCapEnum lc)
    {
        stroker_.SetLineCap(lc);
    }
    enum LineCapEnum LineCap() const
    {
        return stroker_.GetLineCap();
    }
#endif
#if GRAPHIC_ENABLE_LINEJOIN_FLAG
    void LineJoin(LineJoinEnum lj)
    {
        stroker_.SetLineJoin(lj);
    }

    void MiterLimit(float ml)
    {
        stroker_.SetMiterLimit(ml);
    }
    enum LineJoinEnum LineJoin() const
    {
        return stroker_.GetLineJoin();
    }
    float MiterLimit() const
    {
        return stroker_.GetMiterLimit();
    }
#endif
    void Width(float w)
    {
        stroker_.width(w);
    }

    void ApproximationScale(float as)
    {
        stroker_.SetApproximationScale(as);
    }

    float Width() const
    {
        return stroker_.width();
    }

    float ApproximationScale() const
    {
        return stroker_.GetApproximationScale();
    }

    void Shorten(float s)
    {
        shorten_ = s;
    }

    float Shorten() const
    {
        return shorten_;
    }

    void RemoveAll();

    void AddVertex(float x, float y, unsigned cmd);

    void Rewind(unsigned pathId);
    unsigned Vertex(float* x, float* y);

private:
    GraphicVertexGenerateStroke(const GraphicVertexGenerateStroke&);
    const GraphicVertexGenerateStroke& operator=(const GraphicVertexGenerateStroke&);
    GraphicGeometryMathStroke<CoordStorage> stroker_;
    VertexStorage srcVertices_;
    CoordStorage outVertices_;
    float shorten_;
    unsigned closed_;
    Status status_;
    Status prevStatus_;
    unsigned srcVertex_;
    unsigned outVertex_;

    void VertexSwitchCaseReady(const unsigned& verticesNum, unsigned& cmd);
    void VertexSwitchCaseCap1();
    void VertexSwitchCaseCap2(const unsigned& verticesNum);
    void VertexSwitchCaseOutLine1();
    void VertexSwitchCaseOutLine2();
    void VertexSwitchCaseCloseFirst(unsigned& cmd);
    bool VertexSwitchCaseOutVertices(float* x, float* y);
};
} // namespace OHOS

#endif
