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

#ifndef AGG_CONV_ADAPTOR_VCGEN_INCLUDED
#define AGG_CONV_ADAPTOR_VCGEN_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"

namespace OHOS {
    //------------------------------------------------------------null_markers
    struct null_markers {
        void remove_all()
        {}
        void add_vertex(double, double, unsigned)
        {}
        void prepare_src()
        {}

        void rewind(unsigned)
        {}
        unsigned vertex(double*, double*)
        {
            return PATH_CMD_STOP;
        }
    };

    //------------------------------------------------------conv_adaptor_vcgen
    template <class VertexSource,
              class Generator,
              class Markers = null_markers>
    class conv_adaptor_vcgen {
        enum status
        {
            initial,
            accumulate,
            generate
        };

    public:
        explicit conv_adaptor_vcgen(VertexSource& source) :
            m_source(&source),
            status_(initial)
        {}
        void attach(VertexSource& source)
        {
            m_source = &source;
        }

        Generator& generator()
        {
            return m_generator;
        }
        const Generator& generator() const
        {
            return m_generator;
        }

        Markers& markers()
        {
            return m_markers;
        }
        const Markers& markers() const
        {
            return m_markers;
        }

        void rewind(unsigned path_id)
        {
            m_source->rewind(path_id);
            status_ = initial;
        }

        unsigned vertex(double* x, double* y);

    private:
        // Prohibit copying
        conv_adaptor_vcgen(const conv_adaptor_vcgen<VertexSource, Generator, Markers>&);
        const conv_adaptor_vcgen<VertexSource, Generator, Markers>&
            operator=(const conv_adaptor_vcgen<VertexSource, Generator, Markers>&);

        VertexSource* m_source;
        Generator m_generator;
        Markers m_markers;
        status status_;
        unsigned m_last_cmd;
        double m_start_x;
        double m_start_y;
    };

    //------------------------------------------------------------------------
    template <class VertexSource, class Generator, class Markers>
    unsigned conv_adaptor_vcgen<VertexSource, Generator, Markers>::vertex(double* x, double* y)
    {
        unsigned cmd = PATH_CMD_STOP;
        bool done = false;
        while (!done) {
            switch (status_) {
                case initial:
                    m_markers.remove_all();
                    m_last_cmd = m_source->vertex(&m_start_x, &m_start_y);
                    status_ = accumulate;

                case accumulate:
                    if (IsStop(m_last_cmd))
                        return PATH_CMD_STOP;

                    m_generator.RemoveAll();
                    m_generator.AddVertex(m_start_x, m_start_y, PATH_CMD_MOVE_TO);
                    m_markers.add_vertex(m_start_x, m_start_y, PATH_CMD_MOVE_TO);

                    for (;;) {
                        cmd = m_source->vertex(x, y);
                        if (IsVertex(cmd)) {
                            m_last_cmd = cmd;
                            if (IsMoveTo(cmd)) {
                                m_start_x = *x;
                                m_start_y = *y;
                                break;
                            }
                            m_generator.AddVertex(*x, *y, cmd);
                            m_markers.add_vertex(*x, *y, PATH_CMD_LINE_TO);
                        } else {
                            if (IsStop(cmd)) {
                                m_last_cmd = PATH_CMD_STOP;
                                break;
                            }
                            if (IsEndPoly(cmd)) {
                                m_generator.AddVertex(*x, *y, cmd);
                                break;
                            }
                        }
                    }
                    m_generator.Rewind(0);
                    status_ = generate;

                case generate:
                    cmd = m_generator.Vertex(x, y);
                    if (IsStop(cmd)) {
                        status_ = accumulate;
                        break;
                    }
                    done = true;
                    break;
            }
        }
        return cmd;
    }

} // namespace OHOS

#endif
