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

#include "gfx_utils/graphics/common/graphic_common_basics.h"
#include "gfx_utils/graphics/vertexgenerate/graphic_vertex_generate_dash.h"
#include "graphic_depict_adaptor_vertex_generate.h"

namespace OHOS {
    /**
     * @template<VertexSource,Markers> struct DepictDash
     * @brief The dependdash structure is used for vertexsource through the dash class
     * Carry out specific treatment of dotted line.
     * @since 1.0
     * @version 1.0
     */
    template <class VertexSource, class Markers = EmptyMarkers>
    struct DepictDash : public DepictAdaptorVertexGenerator<VertexSource, VertexGenerateDash, Markers> {
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
        typedef Markers marker_type;
        typedef DepictAdaptorVertexGenerator<VertexSource, VertexGenerateDash, Markers> base_type;

        DepictDash(VertexSource& vs)
            : DepictAdaptorVertexGenerator<VertexSource, VertexGenerateDash, Markers>(vs)
        {
        }

        void RemoveAllDashes()
        {
            base_type::GetGenerator().RemoveAllDashes();
        }

        void AddDash(double dash_len, double gap_len)
        {
            base_type::GetGenerator().AddDash(dash_len, gap_len);
        }

        void DashStart(double dStart)
        {
            base_type::GetGenerator().DashStart(dStart);
        }

        void Shorten(double shortValue)
        {
            base_type::GetGenerator().Shorten(shortValue);
        }
        double Shorten() const
        {
            return base_type::GetGenerator().Shorten();
        }

    private:
        DepictDash(const DepictDash<VertexSource, Markers>&);
        const DepictDash<VertexSource, Markers>& operator=(const DepictDash<VertexSource, Markers>&);
#endif
    };
} // namespace OHOS

#endif
