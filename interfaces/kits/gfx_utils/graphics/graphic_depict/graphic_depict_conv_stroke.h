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

#include "graphic_depict_conv_adaptor_vcgen.h"
#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"
#include "gfx_utils/graphics/graphic_vertex_generate/graphic_vertex_generate_stroke.h"
namespace OHOS {

    /**
    * @template<class VertexSource,class Markers> struct DepictStroke
    * @brief 该结构体主要是进行轮廓轮廓线条包括点划线变换的模板结构体.
    * @since 1.0
    * @version 1.0
    */
    template <class VertexSource, class Markers = EmptyMarkers>
    struct DepictStroke
        : public DepictAdaptorVertexGenerator<VertexSource, VCGenStroke, Markers> {
        typedef Markers marker_type;
        typedef DepictAdaptorVertexGenerator<VertexSource, VCGenStroke, Markers>
            base_type;

        /**
        * @brief DepictStroke类的构造函数。.
        * 构造参数为VertexSource 属性决定扩展或收缩轮廓线条的处理。
        * @since 1.0
        * @version 1.0
        */
        DepictStroke(VertexSource& vs) :
            DepictAdaptorVertexGenerator<VertexSource, VCGenStroke, Markers>(vs)
        {
        }
        /*
        * ineCap 属性设置线条末端线帽的样式。
        * butt	默认。向线条的每个末端添加平直的边缘。
        * round	向线条的每个末端添加圆形线帽。
        * square	向线条的每个末端添加正方形线帽。
        * "round" 和 "square" 会使线条略微变长
        */
        void LineCap(LineCapEnum lineCap)
        {
            base_type::GetGenerator().LineCap(lineCap);
        }

        /*
        * lineJoin 属性设置所创建边角的类型，当两条线交汇时,
        * 主要包括bevel	创建斜角。round	创建圆角。
        * miter	默认。创建尖角。
        */
        void LineJoin(LineJoinEnum lineJoin)
        {
            base_type::GetGenerator().LineJoin(lineJoin);
        }
        /*
        * ineCap 属性返回线条末端线帽的样式。
        * butt	默认。向线条的每个末端添加平直的边缘。
        * round	向线条的每个末端添加圆形线帽。
        * square	向线条的每个末端添加正方形线帽。
        * "round" 和 "square" 会使线条略微变长
        */
        LineCapEnum LineCap() const
        {
            return base_type::GetGenerator().LineCap();
        }
        /*
        * lineJoin 属性返回所创建边角的类型，当两条线交汇时,
        * 主要包括bevel	创建斜角。round	创建圆角。
        * miter	默认。创建尖角。
        */
        LineJoinEnum LineJoin() const
        {
            return base_type::GetGenerator().LineJoin();
        }
        /*
        * 轮廓线主要设置几何线条的线宽
        */
        void Width(double width)
        {
            base_type::GetGenerator().Width(width);
        }
        /*
        * miterLimit 属性设置最大斜接长度。
        * 斜接长度指的是在两条线交汇处内角和外角之间的距离
        * 只有当 lineJoin 属性为 "miter" 时，miterLimit 才有效。
        * 边角的角度越小，斜接长度就会越大。
        * 为了避免斜接长度过长，我们可以使用 miterLimit 属性。
        */
        void MiterLimit(double miterLimit)
        {
            base_type::GetGenerator().MiterLimit(miterLimit);
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
            base_type::GetGenerator().ApproximationScale(aScale);
        }
        /*
    * 轮廓线主要返回几何线条的线宽
    */
        double Width() const
        {
            return base_type::GetGenerator().Width();
        }
        /*
    * miterLimit 属性返回最大斜接长度。
    * 斜接长度指的是在两条线交汇处内角和外角之间的距离
    * 只有当 lineJoin 属性为 "miter" 时，miterLimit 才有效。
    * 边角的角度越小，斜接长度就会越大。
    * 为了避免斜接长度过长，我们可以使用 miterLimit 属性。
    */
        double MiterLimit() const
        {
            return base_type::GetGenerator().MiterLimit();
        }
        /**
        * @brief 返回最终决定估算的精度。.
        * 在实际应用中，我们需要从点的世界坐标转换到屏幕坐标，因此总会存在一定的缩放因子。
        * 曲线通常是在世界坐标系中处理的，而进行估算时又要转换为像素值。
        * 一般看起来会是这样的：m_curved.approximation_scale(m_transform.scale());
        * 这里，m_transform 是一个仿型映射的矩阵，里面包含了所有的转换，包括视点和缩放。
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
