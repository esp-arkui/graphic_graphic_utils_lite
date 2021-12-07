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

        void line_cap(LineCap lc)     { base_type::generator().LineCap(lc);  }
        void line_join(LineJoin lj)   { base_type::generator().LineJoin(lj); }
        void inner_join(InnerJoin ij) { base_type::generator().InnerJoin(ij); }

        LineCap   line_cap()   const { return base_type::generator().line_cap();  }
        LineJoin  line_join()  const { return base_type::generator().line_join(); }
        InnerJoin inner_join() const { return base_type::generator().inner_join(); }

        void width(double w) { base_type::generator().Width(w); }
        void miter_limit(double ml) { base_type::generator().MiterLimit(ml); }
        void approximation_scale(double as) { base_type::generator().ApproximationScale(as); }

        double width() const { return base_type::generator().width(); }
        double miter_limit() const { return base_type::generator().MiterLimit(); }
        double approximation_scale() const { return base_type::generator().ApproximationScale(); }


    private:
       conv_stroke(const conv_stroke<VertexSource, Markers>&);
       const conv_stroke<VertexSource, Markers>& 
           operator = (const conv_stroke<VertexSource, Markers>&);

    };

}

#endif
