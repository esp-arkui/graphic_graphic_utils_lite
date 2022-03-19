/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#ifndef GRAPHIC_LITE_DEPICT_STROKE_H
#define GRAPHIC_LITE_DEPICT_STROKE_H

#include "depict_adaptor_vertex_generate.h"
#include "gfx_utils/diagram/common/common_basics.h"
#include "gfx_utils/diagram/vertexgenerate/vertex_generate_stroke.h"

namespace OHOS {
/**
 * @template<class VertexSource,class Markers> struct DepictStroke
 * @brief The structure is mainly a template structure for contour line transformation,
 * including dotted line transformation
 * @since 1.0
 * @version 1.0
 */
template <class VertexSource>
struct DepictStroke
    : public DepictAdaptorVertexGenerate<VertexSource, VertexGenerateStroke> {
    using BaseType = DepictAdaptorVertexGenerate<VertexSource, VertexGenerateStroke>;

    /**
     * @brief DepictStroke Class constructor
     * The construction parameter is the VertexSource property,
     * which determines the processing of expanding or shrinking contour lines.
     * @since 1.0
     * @version 1.0
     */
    DepictStroke(VertexSource& vs)
        : DepictAdaptorVertexGenerate<VertexSource, VertexGenerateStroke>(vs)
    {
    }
#if GRAPHIC_ENABLE_LINECAP_FLAG
    /**
     * ineCap Property sets the style of the line end cap.
     * butt Default. Add a straight edge to each end of the line.
     * round Add a circular cap to each end of the line.
     * square Add a square cap to each end of the line.
     * "round" And "square" make the line slightly longer
     */
    void LineCap(LineCap lineCap)
    {
        BaseType::GetGenerator().LineCap(lineCap);
    }

    /**
     * ineCap Property returns the style of the line end cap.
     * butt Default. Add a straight edge to each end of the line.
     * round Add a circular cap to each end of the line.
     * square Add a square cap to each end of the line.
     * "round" and "square" make the line slightly longer
     */
    enum LineCap LineCap() const
    {
        return BaseType::GetGenerator().LineCap();
    }
#endif
#if GRAPHIC_ENABLE_LINEJOIN_FLAG
    /**
     * lineJoin Property sets the type of corner created. When two lines meet,
     * Mainly including bevel Create a bevel. round Create a fillet.
     * miter Default. Create sharp corners.
     */
    void LineJoin(LineJoin lineJoin)
    {
        BaseType::GetGenerator().LineJoin(lineJoin);
    }

    /**
     * lineJoin Property returns the type of corner created. When two lines meet,
     * Mainly including bevel Create a bevel. round Create a fillet.
     * miter Default. Create sharp corners.
     */
    enum LineJoin LineJoin() const
    {
        return BaseType::GetGenerator().LineJoin();
    }

    /**
     * miterLimit Property to set the maximum miter length.
     * Miter length refers to the distance between the inner and outer corners at the intersection of two lines
     * Miterlimit is valid only when the linejoin attribute is "miter".
     * To avoid the miter length being too long, we can use the miterlimit attribute.
     */
    void MiterLimit(float miterLimit)
    {
        BaseType::GetGenerator().MiterLimit(miterLimit);
    }
    /**
     * miterLimit Property returns the maximum miter length.
     * Miter length refers to the distance between the inner and outer corners at the intersection of two lines
     * Miterlimit is valid only when the linejoin attribute is "miter".
     * The smaller the angle of the corner, the greater the miter length.
     * To avoid the miter length being too long, we can use the miterlimit attribute.
     */
    float MiterLimit() const
    {
        return BaseType::GetGenerator().MiterLimit();
    }
#endif

    /** Contour line mainly sets the lineweight of geometric lines */
    void SetWidth(float width)
    {
        BaseType::GetGenerator().SetWidth(width);
    }

    /**
     * @brief Finally determine the accuracy of the estimation.
     * In practical application, we need to convert the world coordinates of points to screen coordinates,
     * so there will always be a certain scaling factor.
     * Curves are usually processed in the world coordinate system and converted to pixel values when estimating.
     * It usually looks like this: m_curved.approximation_scale(transform_.scale());
     * Here, transform_is a matrix of affine mapping,
     * which contains all transformations,
     * including viewpoint and scaling.
     * @since 1.0
     * @version 1.0
     */
    void ApproximationScale(float aScale)
    {
        BaseType::GetGenerator().ApproximationScale(aScale);
    }
    // Contour lines mainly return the lineweight of geometric lines
    float Width() const
    {
        return BaseType::GetGenerator().Width();
    }

    /**
     * @brief Returns the precision of the final decision estimate
     * In practical application, we need to convert the world coordinates of points to screen coordinates,
     * so there will always be a certain scaling factor.
     * Curves are usually processed in the world coordinate system and converted to pixel values when estimating.
     * It usually looks like this: m_curved.approximation_scale(transform_.scale());
     * Here, transform_is a matrix of affine mapping,
     * which contains all transformations,
     * including viewpoint and scaling.
     * @since 1.0
     * @version 1.0
     */
    float ApproximationScale() const
    {
        return BaseType::GetGenerator().ApproximationScale();
    }

    void SetStrokeShorten(float strokeShorten)
    {
        BaseType::GetGenerator().SetStrokeShorten(strokeShorten);
    }
    float GetStrokeShorten() const
    {
        return BaseType::GetGenerator().GetStrokeShorten();
    }

private:
    DepictStroke(const DepictStroke<VertexSource>&);
    const DepictStroke<VertexSource>& operator= (const DepictStroke<VertexSource>&);
};
} // namespace OHOS
#endif
