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

#ifndef AGG_VCGEN_STROKE_INCLUDED
#define AGG_VCGEN_STROKE_INCLUDED

#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_math_stroke.h"

namespace OHOS
{
    //============================================================vcgen_stroke
    //
    // See Implementation agg_vcgen_stroke.cpp
    // Stroke generator
    //
    //------------------------------------------------------------------------
    class vcgen_stroke
    {
        enum status_e
        {
            initial,
            ready,
            cap1,
            cap2,
            outline1,
            close_first,
            outline2,
            out_vertices,
            end_poly1,
            end_poly2,
            stop
        };

    public:
        typedef vertex_sequence<vertex_dist, 6> vertex_storage;
        typedef pod_bvector<point_d, 6> coord_storage;

        vcgen_stroke();

        void SetLineCap(LineCap lc)
        {
            m_stroker.SetLineCap(lc);
        }
        void SetLineJoin(LineJoin lj)
        {
            m_stroker.SetLineJoin(lj);
        }
        void SetInnerJoin(InnerJoin ij)
        {
            m_stroker.SetInnerJoin(ij);
        }

        LineCap GetLineCap() const
        {
            return m_stroker.GetLineCap();
        }
        LineJoin GetLineJoin() const
        {
            return m_stroker.GetLineJoin();
        }
        InnerJoin GetInnerJoin() const
        {
            return m_stroker.GetInnerJoin();
        }

        void width(double w)
        {
            m_stroker.width(w);
        }
        void SetMiterLimit(double ml)
        {
            m_stroker.SetMiterLimit(ml);
        }
        void SetApproximationScale(double as)
        {
            m_stroker.SetApproximationScale(as);
        }

        double width() const
        {
            return m_stroker.width();
        }
        double GetMiterLimit() const
        {
            return m_stroker.GetMiterLimit();
        }
        double GetApproximationScale() const
        {
            return m_stroker.GetApproximationScale();
        }

        void shorten(double s)
        {
            m_shorten = s;
        }
        double shorten() const
        {
            return m_shorten;
        }

        // Vertex Generator Interface
        void remove_all();
        void add_vertex(double x, double y, unsigned cmd);

        // Vertex Source Interface
        void rewind(unsigned path_id);
        unsigned vertex(double* x, double* y);

    private:
        vcgen_stroke(const vcgen_stroke&);
        const vcgen_stroke& operator=(const vcgen_stroke&);

        MathStroke<coord_storage> m_stroker;
        vertex_storage m_src_vertices;
        coord_storage m_out_vertices;
        double m_shorten;
        unsigned m_closed;
        status_e m_status;
        status_e m_prev_status;
        unsigned m_src_vertex;
        unsigned m_out_vertex;
    };

} // namespace OHOS

#endif
