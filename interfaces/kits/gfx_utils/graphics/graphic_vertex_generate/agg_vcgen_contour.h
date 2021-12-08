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

#ifndef AGG_VCGEN_CONTOUR_INCLUDED
#define AGG_VCGEN_CONTOUR_INCLUDED

#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_math_stroke.h"

namespace OHOS
{
    //----------------------------------------------------------vcgen_contour
    //
    // See Implementation agg_vcgen_contour.cpp
    //
    class vcgen_contour
    {
        enum status_e
        {
            initial,
            ready,
            outline,
            out_vertices,
            end_poly,
            stop
        };

    public:
        typedef vertex_sequence<vertex_dist, 6> vertex_storage;
        typedef pod_bvector<point_d, 6> coord_storage;

        vcgen_contour();

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
            m_stroker.width(m_width = w);
        }
        void SetMiterLimit(double ml)
        {
            m_stroker.SetMiterLimit(ml);
        }

        double width() const
        {
            return m_width;
        }
        double GetMiterLimit() const
        {
            return m_stroker.GetMiterLimit();
        }

        void auto_detect_orientation(bool v)
        {
            m_auto_detect = v;
        }
        bool auto_detect_orientation() const
        {
            return m_auto_detect;
        }

        // Generator interface
        void remove_all();
        void add_vertex(double x, double y, unsigned cmd);

        // Vertex Source Interface
        void rewind(unsigned path_id);
        unsigned vertex(double* x, double* y);

    private:
        vcgen_contour(const vcgen_contour&);
        const vcgen_contour& operator=(const vcgen_contour&);

        MathStroke<coord_storage> m_stroker;
        double m_width;
        vertex_storage m_src_vertices;
        coord_storage m_out_vertices;
        status_e m_status;
        unsigned m_src_vertex;
        unsigned m_out_vertex;
        unsigned m_closed;
        unsigned m_orientation;
        bool m_auto_detect;
    };

} // namespace OHOS

#endif
