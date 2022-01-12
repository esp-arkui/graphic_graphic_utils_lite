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
 * @file graphic_geometry_depict_dash.h
 *
 * @brief Defines Build dash transform pipe
 * For the drawing of dotted lines, the basic appearance of
 * the curve is usually determined by inputting dash array and size
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_DEPICT_DASH_INCLUDED
#define GRAPHIC_GEOMETRY_DEPICT_DASH_INCLUDED

#include "gfx_utils/diagram/common/graphic_common_basics.h"
#include "gfx_utils/diagram/vertexgenerate/graphic_vertex_generate_dash.h"
#include "graphic_depict_adaptor_vertex_generate.h"

namespace OHOS {
using GenerateDash = GraphicVertexGenerateDash;
/**
 * @template<VertexSource,Markers> struct DepictDash
 * @brief The dependdash structure is used for vertexsource through the dash class
 * Carry out specific treatment of dotted line.
 * @since 1.0
 * @version 1.0
 */
template <class VertexSource, class Markers = EmptyMarkers>
struct DepictDash : public GraphicDepictAdaptorVertexGenerate<VertexSource, GenerateDash, Markers> {
#if GRAPHIC_ENABLE_DASH_GENERATE_FLAG
    using BaseType = GraphicDepictAdaptorVertexGenerate<VertexSource, GenerateDash, Markers>;

    DepictDash(VertexSource& vs)
        : GraphicDepictAdaptorVertexGenerate<VertexSource, GenerateDash, Markers>(vs)
    {
    }

    void RemoveAllDashes()
    {
        BaseType::GetGenerator().RemoveAllDashes();
    }

    void AddDash(float dashLen, float gapLen)
    {
        BaseType::GetGenerator().AddDash(dashLen, gapLen);
    }

    void DashStart(float dStart)
    {
        BaseType::GetGenerator().DashStart(dStart);
    }

    void Shorten(float shortValue)
    {
        BaseType::GetGenerator().Shorten(shortValue);
    }
    float Shorten() const
    {
        return BaseType::GetGenerator().Shorten();
    }

private:
    DepictDash(const DepictDash<VertexSource, Markers>&);
    const DepictDash<VertexSource, Markers>& operator=(const DepictDash<VertexSource, Markers>&);
#endif
    };
} // namespace OHOS
#endif
