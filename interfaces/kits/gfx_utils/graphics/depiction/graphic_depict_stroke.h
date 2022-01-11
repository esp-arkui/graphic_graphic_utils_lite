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

#ifndef AGG_CONV_STROKE_INCLUDED
#define AGG_CONV_STROKE_INCLUDED

#include "gfx_utils/graphics/common/graphic_common_basics.h"
#include "gfx_utils/graphics/vertexgenerate/graphic_vertex_generate_stroke.h"
#include "graphic_depict_adaptor_vertex_generate.h"
namespace OHOS {
    /**
     * @template<class VertexSource,class Markers> struct DepictStroke
     * @brief The structure is mainly a template structure for contour line transformation,
     * including dotted line transformation
     * @since 1.0
     * @version 1.0
     */
    template <class VertexSource, class Markers = EmptyMarkers>
    struct DepictStroke
        : public DepictAdaptorVertexGenerator<VertexSource, VertexGenerateStroke, Markers> {
        typedef Markers marker_type;
        typedef DepictAdaptorVertexGenerator<VertexSource, VertexGenerateStroke, Markers>
            base_type;

        /**
         * @brief DepictStroke Class constructor
         * The construction parameter is the VertexSource property,
         * which determines the processing of expanding or shrinking contour lines.
         * @since 1.0
         * @version 1.0
         */
        DepictStroke(VertexSource& vs)
            : DepictAdaptorVertexGenerator<VertexSource, VertexGenerateStroke, Markers>(vs)
        {
        }
#if GRAPHIC_GEOMETYR_ENABLE_LINECAP_STYLES_VERTEX_SOURCE
        /**
         * ineCap Property sets the style of the line end cap.
         * butt Default. Add a straight edge to each end of the line.
         * round Add a circular cap to each end of the line.
         * square Add a square cap to each end of the line.
         * "round" And "square" make the line slightly longer
         */
        void LineCap(LineCapEnum lineCap)
        {
            base_type::GetGenerator().LineCap(lineCap);
        }

        /**
         * ineCap Property returns the style of the line end cap.
         * butt Default. Add a straight edge to each end of the line.
         * round Add a circular cap to each end of the line.
         * square Add a square cap to each end of the line.
         * "round" and "square" make the line slightly longer
         */
        LineCapEnum LineCap() const
        {
            return base_type::GetGenerator().LineCap();
        }
#endif
#if GRAPHIC_GEOMETYR_ENABLE_LINEJOIN_STYLES_VERTEX_SOURCE
        /**
         * lineJoin Property sets the type of corner created. When two lines meet,
         * Mainly including bevel Create a bevel. round Create a fillet.
         * miter Default. Create sharp corners.
         */
        void LineJoin(LineJoinEnum lineJoin)
        {
            base_type::GetGenerator().LineJoin(lineJoin);
        }

        /**
         * lineJoin Property returns the type of corner created. When two lines meet,
         * Mainly including bevel Create a bevel. round Create a fillet.
         * miter Default. Create sharp corners.
         */
        LineJoinEnum LineJoin() const
        {
            return base_type::GetGenerator().LineJoin();
        }

        /**
         * miterLimit Property to set the maximum miter length.
         * Miter length refers to the distance between the inner and outer corners at the intersection of two lines
         * Miterlimit is valid only when the linejoin attribute is "miter".
         * Miterlimit is valid only when the linejoin attribute is "miter".
         * To avoid the miter length being too long, we can use the miterlimit attribute.
         */
        void MiterLimit(double miterLimit)
        {
            base_type::GetGenerator().MiterLimit(miterLimit);
        }
        /**
         * miterLimit Property returns the maximum miter length.
         * Miter length refers to the distance between the inner and outer corners at the intersection of two lines
         * Miterlimit is valid only when the linejoin attribute is "miter".
         * The smaller the angle of the corner, the greater the miter length.
         * To avoid the miter length being too long, we can use the miterlimit attribute.
         */
        double MiterLimit() const
        {
            return base_type::GetGenerator().MiterLimit();
        }
#endif

        /** Contour line mainly sets the lineweight of geometric lines */
        void Width(double width)
        {
            base_type::GetGenerator().Width(width);
        }

        /**
         * @brief Finally determine the accuracy of the estimation.
         * In practical application, we need to convert the world coordinates of points to screen coordinates,
         * so there will always be a certain scaling factor.
         * Curves are usually processed in the world coordinate system and converted to pixel values when estimating.
         * It usually looks like this: m_curved.approximation_scale(m_transform.scale());
         * Here, m_transformis a matrix of affine mapping, which contains all transformations, including viewpoint and scaling.
         * @since 1.0
         * @version 1.0
         */
        void ApproximationScale(double aScale)
        {
            base_type::GetGenerator().ApproximationScale(aScale);
        }
        // Contour lines mainly return the lineweight of geometric lines
        double Width() const
        {
            return base_type::GetGenerator().Width();
        }

        /**
         * @brief Returns the precision of the final decision estimate
         * In practical application, we need to convert the world coordinates of points to screen coordinates,
         * so there will always be a certain scaling factor.
         * Curves are usually processed in the world coordinate system and converted to pixel values when estimating.
         * It usually looks like this: m_curved.approximation_scale(m_transform.scale());
         * Here, m_transformis a matrix of affine mapping, which contains all transformations, including viewpoint and scaling.
         * @since 1.0
         * @version 1.0
         */
        double ApproximationScale() const
        {
            return base_type::GetGenerator().ApproximationScale();
        }

        void Shorten(double shorter)
        {
            base_type::GetGenerator().Shorten(shorter);
        }
        double Shorten() const
        {
            return base_type::GetGenerator().Shorten();
        }

    private:
        DepictStroke(const DepictStroke<VertexSource, Markers>&);
        const DepictStroke<VertexSource, Markers>& operator=(
            const DepictStroke<VertexSource, Markers>&);
    };
} // namespace OHOS
#endif
