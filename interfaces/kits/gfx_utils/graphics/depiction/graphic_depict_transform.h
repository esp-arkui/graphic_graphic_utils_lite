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

#include "gfx_utils/graphics/common/graphic_common_basics.h"
#include "gfx_utils/graphics/transform/graphic_transform_affine.h"
namespace OHOS {
    /**
     * @template<VertexSource,Transformer> class DepictTransform
     * @brief The depicttransform class forms a 2 * 3 matrix through six variables,
     * A new coordinate is obtained after calculation with the coordinate.
     * @since 1.0
     * @version 1.0
     */
    template <class VertexSource, class Transformer = TransAffine>
    class DepictTransform {
    public:
        /**
         * @brief DepictTransform Class constructor
         * The construction parameters are VertexSource and Transformer attributes,
         * which determine the vertex source of the curve.
         * @since 1.0
         * @version 1.0
         */
        DepictTransform(VertexSource& source, Transformer& tr) :
            m_source(&source), m_trans(&tr)
        {}

        void Attach(VertexSource& source)
        {
            m_source = &source;
        }

        void Rewind(unsigned path_id)
        {
            m_source->Rewind(path_id);
        }

        unsigned Vertex(float* x, float* y)
        {
            unsigned cmd = m_source->Vertex(x, y);
            if (IsVertex(cmd)) {
                m_trans->Transform(x, y);
            }
            return cmd;
        }

        void GetTransformer(Transformer& tr)
        {
            m_trans = &tr;
        }

    private:
        DepictTransform(const DepictTransform<VertexSource>&);
        const DepictTransform<VertexSource>& operator=(const DepictTransform<VertexSource>&);
        VertexSource* m_source;
        Transformer* m_trans;
    };
} // namespace OHOS

#endif
