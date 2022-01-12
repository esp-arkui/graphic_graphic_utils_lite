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
 * @file graphic_geometry_depict_transform.h
 *
 * @brief Defines Building vertex matrix transformation pipeline
 * ransformation matrix. Through the operation of vertex coordinates and matrix, we can get new coordinates
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_DEPICT_TRANSFORM_INCLUDED
#define GRAPHIC_GEOMETRY_DEPICT_TRANSFORM_INCLUDED

#include "gfx_utils/diagram/common/graphic_common_basics.h"
#include "gfx_utils/trans_affine.h"
namespace OHOS {
/**
 * @template<VertexSource,Transformer> class DepictTransform
 * @brief The depicttransform class forms a 2 * 3 matrix through six variables,
 * A new coordinate is obtained after calculation with the coordinate.
 * @since 1.0
 * @version 1.0
 */
template <class VertexSource, class Transformer = TransAffine>
class GraphicDepictTransform {
public:
    /**
     * @brief DepictTransform Class constructor
     * The construction parameters are VertexSource and Transformer attributes,
     * which determine the vertex source of the curve.
     * @since 1.0
     * @version 1.0
     */
    GraphicDepictTransform(VertexSource& source, Transformer& tr)
        : source_(&source), trans_(&tr)
    {}

    void Attach(VertexSource& source)
    {
        source_ = &source;
    }

    void Rewind(unsigned pathId)
    {
        source_->Rewind(pathId);
    }

    unsigned Vertex(float* x, float* y)
    {
        unsigned cmd = source_->Vertex(x, y);
        if (IsVertex(cmd)) {
            trans_->Transform(x, y);
        }
        return cmd;
    }

    void GetTransformer(Transformer& tr)
    {
        trans_ = &tr;
    }

private:
    GraphicDepictTransform(const GraphicDepictTransform<VertexSource>&);
    const GraphicDepictTransform<VertexSource>& operator=(const GraphicDepictTransform<VertexSource>&);
    VertexSource* source_;
    Transformer* trans_;
};
} // namespace OHOS
#endif
