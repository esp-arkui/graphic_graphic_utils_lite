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
 * @brief Defines 构建曲线变换管道
 * 对于曲线的描画通常都是使用一系列的短小的线段来近似的，这是描画曲线的唯一高效的方法
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_DEPICT_CURVE_INCLUDED
#define GRAPHIC_GEOMETRY_DEPICT_CURVE_INCLUDED

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"
#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_curves.h"

namespace OHOS {
    /**
     * @template<VertexSource,QuadraticBezierCurve,CubicBezierCurve> class DepictCurve
     * @brief 该DepictCurve类通过PATH_CMD_CURVE3 and PATH_CMD_CURVE4
     * 命令计算生成curve曲线点并且将生成的点利用转换管道保存到
     * move_to/line_to vertex sequence 中
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
         * @brief DepictCurve类的构造函数。.
         * 构造参数为VertexSource 属性决定曲线的顶点源。
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
         * @brief 近似估算曲线的方法。.
         * 定义估算二次或者三次（贝塞尔）曲线的方法有2种。
         * 一种是curve_inc,即通过curve曲线递增的方法估算
         * 二种是curve_div 即通过将curve曲线 等分为N个div段
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
         * @brief 最终决定估算的精度。.
         * 在实际应用中，我们需要从点的世界坐标转换到屏幕坐标，因此总会存在一定的缩放因子。
         * 曲线通常是在世界坐标系中处理的，而进行估算时又要转换为像素值。
         * 一般看起来会是这样的：m_curved.approximation_scale(m_transform.scale());
         * 这里，m_transform 是一个仿型映射的矩阵，里面包含了所有的转换，包括视点和缩放。
         * @since 1.0
         * @version 1.0
         */
        void ApproximationScale(double aScale)
        {
            m_quadratic_bezier.SetApproximationScale(aScale);
            m_cubic_bezier.SetApproximationScale(aScale);
        }

        double ApproximationScale() const
        {
            return m_cubic_bezier.ApproximationScale();
        }

        /**
         * @brief 以弧度来设置角度估算值。这个值越少，在曲线的转弯处的估算就越精确。
         * 不过，如果设置为0，那么表示完全 不考虑角度条件。
         * @since 1.0
         * @version 1.0
         */
        void AngleTolerance(double angleRate)
        {
            m_quadratic_bezier.AngleTolerance(angleRate);
            m_cubic_bezier.AngleTolerance(angleRate);
        }

        double AngleTolerance() const
        {
            return m_cubic_bezier.AngleTolerance();
        }

        /**
         * @brief 一个以弧度来设置的角度。如果是0，
         * 那么只有真正的尖端(cusp)才会有 bevel cut。如果大于0，
         * 那么它会限制曲线的弯曲度，值越大，曲线锐利的转弯处被切得就越少。
         * 一般，这个值不应该大于10-15度。。
         * @since 1.0
         * @version 1.0
         */
        void CuspLimit(double v)
        {
            m_quadratic_bezier.CuspLimit(v);
            m_cubic_bezier.CuspLimit(v);
        }

        double CuspLimit() const
        {
            return m_cubic_bezier.CuspLimit();
        }
        /*
        * 重置某段路径的状态属性
        * @path_id 是某段路径id，从0计算
        * @since 1.0
        * @version 1.0
        */
        void Rewind(unsigned path_id);

        /*
        * 根据PATH_CMD 命令，返回各个阶段产生的顶点坐标
        * @since 1.0
        * @version 1.0
        */
        unsigned Vertex(double* x, double* y);

    private:
        DepictCurve(const self_type&);
        const self_type& operator=(const self_type&);

        VertexSource* m_source;
        double m_last_x;
        double m_last_y;
        QuadraticBezierCurveType m_quadratic_bezier;
        CubicBezierCurveType m_cubic_bezier;
    };

    /*
        * 重置某段路径的状态属性
        * @path_id 是某段路径id，从0计算
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

    /*
        * 根据PATH_CMD 命令，返回各个阶段产生的顶点坐标
        * @since 1.0
        * @version 1.0
        */
    template <class VertexSource, class QuadraticBezierCurve, class CubicBezierCurve>
    unsigned DepictCurve<VertexSource, QuadraticBezierCurve, CubicBezierCurve>::Vertex(double* x, double* y)
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

        double ct2_x = 0;
        double ct2_y = 0;
        double end_x = 0;
        double end_y = 0;

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
