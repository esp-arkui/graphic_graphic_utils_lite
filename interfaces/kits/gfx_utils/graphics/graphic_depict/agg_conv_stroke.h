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

#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "gfx_utils/graphics/graphic_vertex_generate/agg_vcgen_stroke.h"
#include "agg_conv_adaptor_vcgen.h"

namespace OHOS
{

    //-------------------------------------------------------------conv_stroke
    template<class VertexSource, class Markers=null_markers> 
    struct conv_stroke : 
    public conv_adaptor_vcgen<VertexSource, VCGenStroke, Markers>
    {
        typedef Markers marker_type;
        typedef conv_adaptor_vcgen<VertexSource, VCGenStroke, Markers> base_type;

        conv_stroke(VertexSource& vs) : 
            conv_adaptor_vcgen<VertexSource, VCGenStroke, Markers>(vs)
        {
        }

        void line_cap(line_cap_e lc)     { base_type::generator().LineCap(lc);  }
        void line_join(line_join_e lj)   { base_type::generator().LineJoin(lj); }
        void inner_join(inner_join_e ij) { base_type::generator().InnerJoin(ij); }

        line_cap_e   line_cap()   const { return base_type::generator().Line_cap();  }
        line_join_e  line_join()  const { return base_type::generator().LineJoin(); }
        inner_join_e inner_join() const { return base_type::generator().InnerJoin(); }

        void width(double w) { base_type::generator().Width(w); }
        void miter_limit(double ml) { base_type::generator().MiterLimit(ml); }
        void miter_limit_theta(double t) { base_type::generator().MiterLimitTheta(t); }
        void inner_miter_limit(double ml) { base_type::generator().InnerMiterLimit(ml); }
        void approximation_scale(double as) { base_type::generator().ApproximationScale(as); }

        double width() const { return base_type::generator().Width(); }
        double miter_limit() const { return base_type::generator().MiterLimit(); }
        double inner_miter_limit() const { return base_type::generator().InnerMiterLimit(); }
        double approximation_scale() const { return base_type::generator().ApproximationAcale(); }

        void Shorten(double s) { base_type::generator().Shorten(s); }
        double Shorten() const { return base_type::generator().Shorten(); }

    private:
       conv_stroke(const conv_stroke<VertexSource, Markers>&);
       const conv_stroke<VertexSource, Markers>& 
           operator = (const conv_stroke<VertexSource, Markers>&);

    };

}

#endif
