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
 * @file graphic_geometry_depict_curve.h
 *
 * @brief Defines Building curve transformation pipes
 * The curve drawing is usually approximated by a series of short line segments,
 * which is the only efficient method to draw the curve
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_DEPICT_CURVE_INCLUDED
#define GRAPHIC_GEOMETRY_DEPICT_CURVE_INCLUDED

#include "gfx_utils/graphics/common/graphic_common_basics.h"
#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_curves.h"

namespace OHOS {
    /**
     * @template<VertexSource,QuadraticBezierCurve,CubicBezierCurve> class DepictCurve
     * @brief By PATH_CMD_CURVE3 and PATH_CMD_CURVE4
     * The command calculates the generated curve points and
     * saves the generated points to the curve using the conversion pipe
     * move_to/line_to vertex sequence
     * @since 1.0
     * @version 1.0
     */
    template <class VertexSource,
              class QuadraticBezierCurve = QuadraticBezierCurve,
              class CubicBezierCurve = CubicBezierCurve>
    class DepictCurve {
    public:
        typedef QuadraticBezierCurve QuadraticBezierCurveType;
        typedef CubicBezierCurve CubicBezierCurveType;
        typedef DepictCurve<VertexSource, QuadraticBezierCurve, CubicBezierCurve> self_type;

        /**
         * @brief DepictCurve Class constructor
         * The construction parameter is the vertexsource attribute, which determines the vertex source of the curve.
         * @since 1.0
         * @version 1.0
         */
        explicit DepictCurve(VertexSource& source) :
            m_source(&source), m_last_x(0.0), m_last_y(0.0)
        {
        }

        void Attach(VertexSource& source)
        {
            m_source = &source;
        }

        /**
         * @brief Method for approximate estimation of curve
         * There are two methods for defining and estimating quadratic or cubic (Bessel) curves.
         * One is curve_ Inc, which is estimated by increasing the curve
         * The second is curve_ Div is by equally dividing the curve into n div segments
         * @since 1.0
         * @version 1.0
         */
        void ApproximationMethod(CurveApproximationMethodEnum curvApproxiMethod)
        {
            m_quadratic_bezier.ApproximationMethod(curvApproxiMethod);
            m_cubic_bezier.ApproximationMethod(curvApproxiMethod);
        }

        CurveApproximationMethodEnum ApproximationMethod() const
        {
            return m_cubic_bezier.ApproximationMethod();
        }

        /**
         * @brief Finally determine the accuracy of the estimate
         * In practical application, we need to convert the world coordinates of points to screen coordinates,
         * so there will always be a certain scaling factor.
         * Curves are usually processed in the world coordinate system and converted to pixel values when estimating.
         * It usually looks like this: m_curved.approximation_scale(m_transform.scale());
         * Here, m_transform is a matrix of affine mapping, which contains all transformations, including viewpoint and scaling.
         * @since 1.0
         * @version 1.0
         */
        void ApproximationScale(float aScale)
        {
            m_quadratic_bezier.SetApproximationScale(aScale);
            m_cubic_bezier.SetApproximationScale(aScale);
        }

        float ApproximationScale() const
        {
            return m_cubic_bezier.ApproximationScale();
        }

        /**
         * @brief Sets the angle estimate in radians. The less this value,
         * the more accurate the estimation at the turn of the curve.
         * However, if set to 0, it means that the angle condition is not considered at all.
         * @since 1.0
         * @version 1.0
         */
        void AngleTolerance(float angleRate)
        {
            m_quadratic_bezier.AngleTolerance(angleRate);
            m_cubic_bezier.AngleTolerance(angleRate);
        }

        float AngleTolerance() const
        {
            return m_cubic_bezier.AngleTolerance();
        }

        /**
         * @brief An angle set in radians. If 0,
         * Then only the real cusp will have a bevel cut. If greater than 0,
         * Then it will limit the curvature of the curve. The higher the value,
         * the less sharp corners of the curve will be cut.
         * Generally, this value should not be greater than 10-15 degrees.
         * @since 1.0
         * @version 1.0
         */
        void CuspLimit(float v)
        {
            m_quadratic_bezier.CuspLimit(v);
            m_cubic_bezier.CuspLimit(v);
        }

        float CuspLimit() const
        {
            return m_cubic_bezier.CuspLimit();
        }
        /**
         * Reset the status attribute of a path segment
         * @path_id Is a path ID, calculated from 0
         * @since 1.0
         * @version 1.0
         */
        void Rewind(unsigned path_id);

        /**
         * According to PATH_CMD command returns the vertex coordinates generated in each stage
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(float* x, float* y);

    private:
        DepictCurve(const self_type&);
        const self_type& operator=(const self_type&);

        VertexSource* m_source;
        float m_last_x;
        float m_last_y;
        QuadraticBezierCurveType m_quadratic_bezier;
        CubicBezierCurveType m_cubic_bezier;
    };

    /**
     * Reset the status attribute of a path segment
     * @path_id is a path ID, calculated from 0
     * @since 1.0
     * @version 1.0
     */
    template <class VertexSource, class QuadraticBezierCurve, class CubicBezierCurve>
    void DepictCurve<VertexSource, QuadraticBezierCurve, CubicBezierCurve>::Rewind(unsigned path_id)
    {
        m_source->Rewind(path_id);
        m_last_x = 0.0;
        m_last_y = 0.0;
        m_quadratic_bezier.Reset();
        m_cubic_bezier.Reset();
    }

    /**
     * According to PATH_CMD command returns the vertex coordinates generated in each stage
     * @since 1.0
     * @version 1.0
     */
    template <class VertexSource, class QuadraticBezierCurve, class CubicBezierCurve>
    unsigned DepictCurve<VertexSource, QuadraticBezierCurve, CubicBezierCurve>::Vertex(float* x, float* y)
    {
        if (!IsStop(m_quadratic_bezier.Vertex(x, y))) {
            m_last_x = *x;
            m_last_y = *y;
            return PATH_CMD_LINE_TO;
        }

        if (!IsStop(m_cubic_bezier.Vertex(x, y))) {
            m_last_x = *x;
            m_last_y = *y;
            return PATH_CMD_LINE_TO;
        }

        float ct2_x = 0;
        float ct2_y = 0;
        float end_x = 0;
        float end_y = 0;

        unsigned cmd = m_source->Vertex(x, y);
        switch (cmd) {
            case PATH_CMD_CURVE3:
                m_source->Vertex(&end_x, &end_y);

                m_quadratic_bezier.Init(m_last_x, m_last_y, *x, *y, end_x, end_y);

                m_quadratic_bezier.Vertex(x, y); // First call returns path_cmd_move_to
                m_quadratic_bezier.Vertex(x, y); // This is the first vertex of the curve
                cmd = PATH_CMD_LINE_TO;
                break;

            case PATH_CMD_CURVE4:
                m_source->Vertex(&ct2_x, &ct2_y);
                m_source->Vertex(&end_x, &end_y);

                m_cubic_bezier.Init(m_last_x, m_last_y, *x, *y, ct2_x, ct2_y, end_x, end_y);

                m_cubic_bezier.Vertex(x, y); // First call returns path_cmd_move_to
                m_cubic_bezier.Vertex(x, y); // This is the first vertex of the curve
                cmd = PATH_CMD_LINE_TO;
                break;
        }
        m_last_x = *x;
        m_last_y = *y;
        return cmd;
    }
} // namespace OHOS
#endif
