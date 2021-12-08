//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------
//
// conv_stroke
//
//----------------------------------------------------------------------------
#ifndef AGG_CONV_STROKE_INCLUDED
#define AGG_CONV_STROKE_INCLUDED

#include "agg_conv_adaptor_vcgen.h"
#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "gfx_utils/graphics/graphic_vertex_generate/agg_vcgen_stroke.h"

namespace OHOS
{
    //-------------------------------------------------------------conv_stroke
    template <class VertexSource, class Markers = null_markers>
    struct conv_stroke : public conv_adaptor_vcgen<VertexSource, vcgen_stroke, Markers>
    {
        typedef Markers marker_type;
        typedef conv_adaptor_vcgen<VertexSource, vcgen_stroke, Markers> base_type;

        conv_stroke(VertexSource& vs) :
            conv_adaptor_vcgen<VertexSource, vcgen_stroke, Markers>(vs)
        {
        }

        void SetLineCap(LineCap lc)
        {
            base_type::generator().SetLineCap(lc);
        }
        void SetLineJoin(LineJoin lj)
        {
            base_type::generator().SetLineJoin(lj);
        }
        void SetInnerJoin(InnerJoin ij)
        {
            base_type::generator().SetInnerJoin(ij);
        }

        LineCap GetLineCap() const
        {
            return base_type::generator().GetLineCap();
        }
        LineJoin GetLineJoin() const
        {
            return base_type::generator().GetLineJoin();
        }
        InnerJoin GetInnerJoin() const
        {
            return base_type::generator().GetInnerJoin();
        }

        void width(double w)
        {
            base_type::generator().width(w);
        }
        void SetMiterLimit(double ml)
        {
            base_type::generator().SetMiterLimit(ml);
        }
        void SetApproximationScale(double as)
        {
            base_type::generator().SetApproximationScale(as);
        }

        double width() const
        {
            return base_type::generator().width();
        }
        double GetMiterLimit() const
        {
            return base_type::generator().GetMiterLimit();
        }
        double GetApproximationScale() const
        {
            return base_type::generator().GetApproximationScale();
        }

        void shorten(double s)
        {
            base_type::generator().shorten(s);
        }
        double shorten() const
        {
            return base_type::generator().shorten();
        }

    private:
        conv_stroke(const conv_stroke<VertexSource, Markers>&);
        const conv_stroke<VertexSource, Markers>&
            operator=(const conv_stroke<VertexSource, Markers>&);
    };

} // namespace OHOS

#endif
