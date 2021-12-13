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
 * @brief Defines 构建点划线变换管道
 * 对于点划线的描画通常都是通过输入dash数组和大小确定曲线的基本面貌
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_DEPICT_DASH_INCLUDED
#define GRAPHIC_GEOMETRY_DEPICT_DASH_INCLUDED

#include "graphic_depict_adaptor_vertex_generate.h"
#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"
#include "gfx_utils/graphics/graphic_vertex_generate/graphic_vertex_generate_dash.h"

namespace OHOS {
    /**
     * @template<VertexSource,Markers> struct DepictDash
     * @brief 该DepictDash结构体通过dash类对于VertexSource
     * 进行点划线的具体处理。
     * @since 1.0
     * @version 1.0
     */
    template <class VertexSource, class Markers = EmptyMarkers>
    struct DepictDash : public DepictAdaptorVertexGenerator<VertexSource, VCGenDash, Markers> {
        typedef Markers marker_type;
        typedef DepictAdaptorVertexGenerator<VertexSource, VCGenDash, Markers> base_type;

        DepictDash(VertexSource& vs) : DepictAdaptorVertexGenerator<VertexSource, VCGenDash, Markers>(vs)
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
        const DepictDash<VertexSource, Markers>&
            operator=(const DepictDash<VertexSource, Markers>&);
    };
} // namespace OHOS

#endif
