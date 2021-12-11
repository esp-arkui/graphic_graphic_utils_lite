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
* @file agg_rasterizer_scanline_aa.h
* @brief Defines 光栅扫描线（防走样）
* @since 1.0
* @version 1.0
*/

//----------------------------------------------------------------------------
#ifndef GRAPHIC_RASTERIZER_SCANLINE_AA_INCLUDED
#define GRAPHIC_RASTERIZER_SCANLINE_AA_INCLUDED

#include "agg_rasterizer_cells_aa.h"
#include "agg_rasterizer_sl_clip.h"

namespace OHOS {
    //------------------------------------------------------------------------
    template <class Clip = rasterizer_sl_clip_int>
    class rasterizer_scanline_aa {
        enum status
        {
            status_initial,
            status_move_to,
            status_line_to,
            status_closed
        };

    public:
        typedef Clip clip_type;
        typedef typename Clip::conv_type conv_type;
        typedef typename Clip::coord_type coord_type;

        enum aa_scale_e
        {
            aa_shift = 8,
            aa_scale = 1 << aa_shift,
            aa_mask = aa_scale - 1,
            aa_scale2 = aa_scale * 2,
            aa_mask2 = aa_scale2 - 1
        };

        //--------------------------------------------------------------------
        rasterizer_scanline_aa(unsigned cell_block_limit = 1024) :
            m_outline(cell_block_limit),
            m_clipper(),
            m_filling_rule(FILL_NON_ZERO),
            m_auto_close(true),
            m_start_x(0),
            m_start_y(0),
            status_(status_initial)
        {
            int i;
            for (i = 0; i < aa_scale; i++)
                m_gamma[i] = i;
        }

        //--------------------------------------------------------------------
        template <class GammaF>
        rasterizer_scanline_aa(const GammaF& gamma_function, unsigned cell_block_limit) :
            m_outline(cell_block_limit),
            m_clipper(m_outline),
            m_filling_rule(FILL_NON_ZERO),
            m_auto_close(true),
            m_start_x(0),
            m_start_y(0),
            status_(status_initial)
        {
            gamma(gamma_function);
        }

        //--------------------------------------------------------------------
        void reset();
        void reset_clipping();
        void clip_box(double x1, double y1, double x2, double y2);
        void filling_rule(FillingRuleEnum filling_rule);
        void auto_close(bool flag)
        {
            m_auto_close = flag;
        }

        //--------------------------------------------------------------------
        template <class GammaF>
        void gamma(const GammaF& gamma_function)
        {
            int i;
            for (i = 0; i < aa_scale; i++) {
                m_gamma[i] = Uround(gamma_function(double(i) / aa_mask) * aa_mask);
            }
        }

        //--------------------------------------------------------------------
        unsigned apply_gamma(unsigned cover) const
        {
            return m_gamma[cover];
        }

        //--------------------------------------------------------------------
        void move_to(int x, int y);
        void line_to(int x, int y);
        void move_to_d(double x, double y);
        void line_to_d(double x, double y);
        void close_polygon();
        void add_vertex(double x, double y, unsigned cmd);

        void edge(int x1, int y1, int x2, int y2);
        void edge_d(double x1, double y1, double x2, double y2);

        //-------------------------------------------------------------------
        template <class VertexSource>
        void add_path(VertexSource& vs, unsigned path_id = 0)
        {
            double x;
            double y;

            unsigned cmd;
            vs.rewind(path_id);
            if (m_outline.sorted())
                reset();
            while (!IsStop(cmd = vs.vertex(&x, &y))) {
                add_vertex(x, y, cmd);
            }
        }

        //--------------------------------------------------------------------
        int min_x() const
        {
            return m_outline.min_x();
        }
        int min_y() const
        {
            return m_outline.min_y();
        }
        int max_x() const
        {
            return m_outline.max_x();
        }
        int max_y() const
        {
            return m_outline.max_y();
        }

        //--------------------------------------------------------------------
        void sort();
        bool rewind_scanlines();
        bool navigate_scanline(int y);

        //--------------------------------------------------------------------
        GRAPHIC_GEOMETRY_INLINE unsigned calculate_alpha(int area) const
        {
            int cover = area >> (POLY_SUBPIXEL_SHIFT * 2 + 1 - aa_shift);

            if (cover < 0)
                cover = -cover;
            if (m_filling_rule == FILL_EVEN_ODD) {
                cover &= aa_mask2;
                if (cover > aa_scale) {
                    cover = aa_scale2 - cover;
                }
            }
            if (cover > aa_mask)
                cover = aa_mask;
            return m_gamma[cover];
        }

        //--------------------------------------------------------------------
        template <class Scanline>
        bool sweep_scanline(Scanline& sl)
        {
            for (;;) {
                if (m_scan_y > m_outline.max_y()) {
                    return false;
                }
                sl.reset_spans();
                unsigned num_cells = m_outline.scanline_num_cells(m_scan_y);
                const cell_aa* const* cells = m_outline.scanline_cells(m_scan_y);
                int cover = 0;

                while (num_cells) {
                    const cell_aa* cur_cell = *cells;
                    int x = cur_cell->x;
                    int area = cur_cell->area;
                    unsigned alpha;

                    cover += cur_cell->cover;

                    //accumulate all cells with the same X
                    while (--num_cells) {
                        cur_cell = *++cells;
                        if (cur_cell->x != x)
                            break;
                        area += cur_cell->area;
                        cover += cur_cell->cover;
                    }

                    if (area) {
                        alpha = calculate_alpha((cover << (POLY_SUBPIXEL_SHIFT + 1)) - area);
                        if (alpha) {
                            sl.add_cell(x, alpha);
                        }
                        x++;
                    }

                    if (num_cells && cur_cell->x > x) {
                        alpha = calculate_alpha(cover << (POLY_SUBPIXEL_SHIFT + 1));
                        if (alpha) {
                            sl.add_span(x, cur_cell->x - x, alpha);
                        }
                    }
                }

                if (sl.num_spans()) {
                    break;
                }
                ++m_scan_y;
            }

            sl.finalize(m_scan_y);
            ++m_scan_y;
            return true;
        }

        //--------------------------------------------------------------------
        bool hit_test(int tx, int ty);

    private:
        //--------------------------------------------------------------------
        // Disable copying
        rasterizer_scanline_aa(const rasterizer_scanline_aa<Clip>&);
        const rasterizer_scanline_aa<Clip>&
            operator=(const rasterizer_scanline_aa<Clip>&);

    private:
        rasterizer_cells_aa<cell_aa> m_outline;
        clip_type m_clipper;
        int m_gamma[aa_scale];
        FillingRuleEnum m_filling_rule;
        bool m_auto_close;
        coord_type m_start_x;
        coord_type m_start_y;
        unsigned status_;
        int m_scan_y;
    };

    //------------------------------------------------------------------------
    template <class Clip>
    void rasterizer_scanline_aa<Clip>::reset()
    {
        m_outline.reset();
        status_ = status_initial;
    }

    //------------------------------------------------------------------------
    template <class Clip>
    void rasterizer_scanline_aa<Clip>::filling_rule(FillingRuleEnum filling_rule)
    {
        m_filling_rule = filling_rule;
    }

    //------------------------------------------------------------------------
    template <class Clip>
    void rasterizer_scanline_aa<Clip>::clip_box(double x1, double y1,
                                                double x2, double y2)
    {
        reset();
        m_clipper.clip_box(conv_type::upscale(x1), conv_type::upscale(y1),
                           conv_type::upscale(x2), conv_type::upscale(y2));
    }

    //------------------------------------------------------------------------
    template <class Clip>
    void rasterizer_scanline_aa<Clip>::reset_clipping()
    {
        reset();
        m_clipper.reset_clipping();
    }

    //------------------------------------------------------------------------
    template <class Clip>
    void rasterizer_scanline_aa<Clip>::close_polygon()
    {
        if (status_ == status_line_to) {
            m_clipper.line_to(m_outline, m_start_x, m_start_y);
            status_ = status_closed;
        }
    }

    //------------------------------------------------------------------------
    template <class Clip>
    void rasterizer_scanline_aa<Clip>::move_to(int x, int y)
    {
        if (m_outline.sorted())
            reset();
        if (m_auto_close)
            close_polygon();
        m_clipper.move_to(m_start_x = conv_type::downscale(x),
                          m_start_y = conv_type::downscale(y));
        status_ = status_move_to;
    }

    //------------------------------------------------------------------------
    template <class Clip>
    void rasterizer_scanline_aa<Clip>::line_to(int x, int y)
    {
        m_clipper.line_to(m_outline,
                          conv_type::downscale(x),
                          conv_type::downscale(y));
        status_ = status_line_to;
    }

    //------------------------------------------------------------------------
    template <class Clip>
    void rasterizer_scanline_aa<Clip>::move_to_d(double x, double y)
    {
        if (m_outline.sorted())
            reset();
        if (m_auto_close)
            close_polygon();
        m_clipper.move_to(m_start_x = conv_type::upscale(x),
                          m_start_y = conv_type::upscale(y));
        status_ = status_move_to;
    }

    //------------------------------------------------------------------------
    template <class Clip>
    void rasterizer_scanline_aa<Clip>::line_to_d(double x, double y)
    {
        m_clipper.line_to(m_outline,
                          conv_type::upscale(x),
                          conv_type::upscale(y));
        status_ = status_line_to;
    }

    //------------------------------------------------------------------------
    template <class Clip>
    void rasterizer_scanline_aa<Clip>::add_vertex(double x, double y, unsigned cmd)
    {
        if (IsMoveTo(cmd)) {
            move_to_d(x, y);
        } else if (IsVertex(cmd)) {
            line_to_d(x, y);
        } else if (IsClose(cmd)) {
            close_polygon();
        }
    }

    //------------------------------------------------------------------------
    template <class Clip>
    void rasterizer_scanline_aa<Clip>::edge(int x1, int y1, int x2, int y2)
    {
        if (m_outline.sorted())
            reset();
        m_clipper.move_to(conv_type::downscale(x1), conv_type::downscale(y1));
        m_clipper.line_to(m_outline,
                          conv_type::downscale(x2),
                          conv_type::downscale(y2));
        status_ = status_move_to;
    }

    //------------------------------------------------------------------------
    template <class Clip>
    void rasterizer_scanline_aa<Clip>::edge_d(double x1, double y1,
                                              double x2, double y2)
    {
        if (m_outline.sorted())
            reset();
        m_clipper.move_to(conv_type::upscale(x1), conv_type::upscale(y1));
        m_clipper.line_to(m_outline,
                          conv_type::upscale(x2),
                          conv_type::upscale(y2));
        status_ = status_move_to;
    }

    //------------------------------------------------------------------------
    template <class Clip>
    void rasterizer_scanline_aa<Clip>::sort()
    {
        if (m_auto_close)
            close_polygon();
        m_outline.sort_cells();
    }

    //------------------------------------------------------------------------
    template <class Clip>
    GRAPHIC_GEOMETRY_INLINE bool rasterizer_scanline_aa<Clip>::rewind_scanlines()
    {
        if (m_auto_close)
            close_polygon();
        m_outline.sort_cells();
        if (m_outline.total_cells() == 0) {
            return false;
        }
        m_scan_y = m_outline.min_y();
        return true;
    }

    //------------------------------------------------------------------------
    template <class Clip>
    GRAPHIC_GEOMETRY_INLINE bool rasterizer_scanline_aa<Clip>::navigate_scanline(int y)
    {
        if (m_auto_close)
            close_polygon();
        m_outline.sort_cells();
        if (m_outline.total_cells() == 0 ||
            y < m_outline.min_y() ||
            y > m_outline.max_y()) {
            return false;
        }
        m_scan_y = y;
        return true;
    }

    //------------------------------------------------------------------------
    template <class Clip>
    bool rasterizer_scanline_aa<Clip>::hit_test(int tx, int ty)
    {
        if (!navigate_scanline(ty))
            return false;
        scanline_hit_test sl(tx);
        sweep_scanline(sl);
        return sl.hit();
    }

} // namespace OHOS

#endif
