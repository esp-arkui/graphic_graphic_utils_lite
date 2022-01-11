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
 * @file graphic_geometry_rasterizer_scanline_antialias.h
 * @brief Defines Rasterization and scanline process stage processing
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_RASTERIZER_SCANLINE_ANTIALIAS_INCLUDED
#define GRAPHIC_GEOMETRY_RASTERIZER_SCANLINE_ANTIALIAS_INCLUDED

#include "graphic_rasterizer_cells_antialias.h"
#include "graphic_rasterizer_scanline_clip.h"

namespace OHOS {
    using RasterizerScanlineClipInt = RasterizerScanlineClip<RasterDepictInt>;
    /**
     * @template<Clip = RasterizerScanlineClipInt> typename RasterizerScanlineAntiAlias
     * @brief Polygon rasterization is used for high-quality rendering of filled polygons,
     * The int coordinates of this class include the format of 24.8,
     * and 24 bits are used for the int part of the coordinates,
     * 8 bits are used for the shift offset part of the sub-pixel, and then move_ to(x, y) / line_ to(x, y)
     * When constructing polygons, the scanline is rasterized
     * @since 1.0
     * @version 1.0
     */
    template <class Clip = RasterizerScanlineClipInt>
    class RasterizerScanlineAntiAlias {
        /**
         * @brief Build scanline status enumeration
         * @since 1.0
         * @version 1.0
         */
        enum RasterizerStatusEnum {
            STATUS_INITIAL,
            STATUS_MOVE_TO,
            STATUS_LINE_TO,
            STATUS_CLOSED
        };

    public:
        typedef Clip clip_type;
        typedef typename Clip::depict_type depict_type;
        typedef typename Clip::coord_type coord_type;

        enum AntialiasScaleEnum {
            AA_SHIFT = 8,
            AA_SCALE = 1 << AA_SHIFT,
            AA_MASK = AA_SCALE - 1,
            AA_SCALE2 = AA_SCALE * 2,
            AA_MASK2 = AA_SCALE2 - 1
        };

        /**
         * Construction of rasterized scanline antialiasing constructor
         * @brief It mainly includes the allocation quota of cell block and cutter
         * Filling rules, automatic closing, starting position, etc.
         * @since 1.0
         * @version 1.0
         */
        RasterizerScanlineAntiAlias(unsigned cell_block_limit = (1 << (AA_SHIFT + 2)))
            : m_outline(cell_block_limit),
            m_clipper(),
            m_filling_rule(FILL_NON_ZERO),
            m_auto_close(true),
            m_start_x(0),
            m_start_y(0),
            m_status(STATUS_INITIAL)
        {
            int coverIndex;
            for (coverIndex = 0; coverIndex < AA_SCALE; coverIndex++) {
                m_gamma[coverIndex] = coverIndex;
            }
        }

        template <typename GammaF>
        RasterizerScanlineAntiAlias(const GammaF& gamma_function, unsigned cell_block_limit)
            : m_outline(cell_block_limit),
            m_clipper(m_outline),
            m_filling_rule(FILL_NON_ZERO),
            m_auto_close(true),
            m_start_x(0),
            m_start_y(0),
            m_status(STATUS_INITIAL)
        {
            gamma(gamma_function);
        }

        /**
         * @brief Reset the cell array for building contour lines,
         * Reset the scan line status value, etc.
         * @since 1.0
         * @version 1.0
         */
        void Reset();

        /**
         * @brief Reset the clipping range and clipping flag of the clipper.
         * @since 1.0
         * @version 1.0
         */
        void ResetClipping();
        void ClipBox(float x1, float y1, float x2, float y2);

        /**
         * @brief Set fill rule, 设置填充规则，FILL_NON_ZERO
         * The assumption is used to determine which areas belong to the path
         * (if the calculation result is not 0, it is within the path).
         * In the area surrounded by the path, find any point and emit a ray outward,
         * Intersecting all the edges around it,
         * Then turn on a counter and count from 0,
         * If this ray encounters a clockwise circle, then + 1,
         * If you encounter a counterclockwise surround, - 1,
         * If the final value is not 0, this area is within the path),
         * FILL_EVEN_ODD (* find a random point in the area surrounded by the path and emit a ray,
         * Intersecting all the edges around it,
         * Check the number of intersecting lines. If it is odd, it will be filled. If it is even,
         * it will not be filled.)
         * @since 1.0
         * @version 1.0
         */
        void FillingRule(FillingRuleEnum filling_rule);
        void AutoClose(bool flag)
        {
            m_auto_close = flag;
        }

        /**
         * @brief Set the gamma function to calculate the alpha value of the antialiasing service
         * The index of the gamma function is coverage, and each coverage is based on the gamma function, such as
         * gamma_power：Using the square of each coverage * setting value as the color value,
         * you can zoom in or out of the anti aliasing value as a whole.
         * gamma_linear ：Set the start and end range, and linearly transform the color value.
         * If it is less than the starting point, it will be 0,
         * and if it is greater than the starting point, it will be 1,
         * Within the scope (cover - m_start) / (m_end - m_start)。
         * gamma_threshold：Set the threshold value. If it is less than the threshold value, it will be 0.
         * If it is greater than the threshold value,
         * it will be 1 When the threshold is set, there is no antialiasing effect.
         * gamma_multiply：Multiplies the color value by the set multiple
         * @since 1.0
         * @version 1.0
         */
        template <typename GammaF>
        void GammaFunction(const GammaF& gamma_function)
        {
            int coverIndex;
            for (coverIndex = 0; coverIndex < AA_SCALE; coverIndex++) {
                m_gamma[coverIndex] = MATH_ROUND32(gamma_function(float(coverIndex) / AA_MASK) * AA_MASK);
            }
        }

        unsigned ApplyGammaFunction(unsigned cover) const
        {
            return m_gamma[cover];
        }

        /**
         * @brief Set the starting position of the element according to the of 1 / 256 pixel unit.
         * @since 1.0
         * @version 1.0
         */
        void MoveTo(int x, int y);
        /**
         * @brief The moving position of the element is set according to the 1 / 256 pixel unit.
         * @since 1.0
         * @version 1.0
         */
        void LineTo(int x, int y);
        /**
         * @brief Set the starting position of the element according to the of 1 pixel unit.
         * @since 1.0
         * @version 1.0
         */
        void MoveToByfloat(float x, float y);
        /**
         * @brief Set the moving position of the element according to the pixel unit.
         * @since 1.0
         * @version 1.0
         */
        void LineToByfloat(float x, float y);

        /**
         * @brief Closed polygon processing.
         * @since 1.0
         * @version 1.0
         */
        void ClosePolygon();
        void AddVertex(float x, float y, unsigned cmd);

        /**
         * @brief Use the clipper to cut an edge according to 1 / 256 pixel unit and construct a contour line.
         * @since 1.0
         * @version 1.0
         */
        void EdgeMake(int x1, int y1, int x2, int y2);
        /**
         * @brief Use the clipper to cut an edge according to 1 pixel unit and construct a contour line.
         * @since 1.0
         * @version 1.0
         */
        void EdgeMakeUsingfloat(float x1, float y1, float x2, float y2);

        /**
         * @brief Obtain the vertex information coordinates from the vertex source and follow the scanning process
         * Sets the procedure for adding an array of cells.
         * @since 1.0
         * @version 1.0
         */
        template <typename VertexSource>
        void AddPath(VertexSource& vs, unsigned path_id = 0)
        {
            float x;
            float y;

            unsigned cmd;
            vs.Rewind(path_id);
            if (m_outline.GetSorted()) {
                Reset();
            }
            while (!IsStop(cmd = vs.Vertex(&x, &y))) {
                AddVertex(x, y, cmd);
            }
        }

        /**
         * @brief The range boundary value of the contour line.
         * @since 1.0
         * @version 1.0
         */
        int MinX() const
        {
            return m_outline.MinX();
        }
        int MinY() const
        {
            return m_outline.Miny();
        }
        int MaxX() const
        {
            return m_outline.MaxX();
        }
        int MaxY() const
        {
            return m_outline.MaxY();
        }

        /**
         * @brief The cells in the contour line are sorted from left to right and from top to bottom.
         * @since 1.0
         * @version 1.0
         */
        void Sort();
        bool RewindScanlines();

        /**
         * @brief Navigate to the starting position of a scan line according to the height of a y value.
         * @since 1.0
         * @version 1.0
         */
        bool NavigateScanline(int y);

        /**
         * @brief Convert area cover to gamma cover value to calculate alpha.
         * @since 1.0
         * @version 1.0
         */
        unsigned CalculateAlpha(int area) const;
        /**
         * @brief Scan line that gets a y value from rasterizer stage
         * And iterate the cell array of the current scan line to obtain area->cover，
         * Use both to calculate delta area as area cover and convert it into gamma cover
         * The color information is obtained successfully,
         * and then the alpha information of color is calculated by gamma function
         * Fill in the new scanline and have subsequent render.
         * @since 1.0
         * @version 1.0
         */
        template <typename Scanline>
        bool SweepScanline(Scanline& sl);
        bool HitTest(int tx, int ty);

    private:
        // Disable copying
        RasterizerScanlineAntiAlias(const RasterizerScanlineAntiAlias<Clip>&);
        const RasterizerScanlineAntiAlias<Clip>& operator=(const RasterizerScanlineAntiAlias<Clip>&);

        RasterizerCellsAntiAlias<CellBuildAntiAlias> m_outline;
        clip_type m_clipper;
        int m_gamma[AA_SCALE];
        FillingRuleEnum m_filling_rule;
        bool m_auto_close;
        coord_type m_start_x;
        coord_type m_start_y;
        unsigned m_status;
        int m_scan_y;
    };

    /**
     * @brief Scan line that gets a y value from rasterizer stage
     * And iterate the cell array of the current scan line to obtain area->cover，
     * Use both to calculate delta area as area cover and convert it into gamma cover
     * the color information is obtained successfully,
     * and then the alpha information of color is calculated by gamma function
     * Fill in the new scanline and have subsequent render.
     * @since 1.0
     * @version 1.0
     */
    template <class Clip>
    template <class Scanline>
    bool RasterizerScanlineAntiAlias<Clip>::SweepScanline(Scanline& sl)
    {
        while (1) {
            if (m_scan_y > m_outline.MaxY()) {
                return false;
            }
            sl.ResetSpans();
            unsigned num_cells = m_outline.ScanlineNumCells(m_scan_y);
            const CellBuildAntiAlias* const *cells = m_outline.ScanlineCells(m_scan_y);
            int cover = 0;

            while (num_cells) {
                const CellBuildAntiAlias* cur_cell = *cells;
                int x = cur_cell->x;
                int area = cur_cell->area;
                unsigned alpha;

                cover += cur_cell->cover;
                // accumulate all cells with the same X
                while (--num_cells) {
                    cur_cell = *++cells;
                    if (cur_cell->x != x) {
                        break;
                    }
                    area += cur_cell->area;
                    cover += cur_cell->cover;
                }

                if (area) {
                    // Span interval from area to  (cover << (POLY_SUBPIXEL_SHIFT + 1))
                    // Cover can be understood as a delta mask with an area of 1
                    alpha = CalculateAlpha((cover << (POLY_SUBPIXEL_SHIFT + 1)) - area);
                    if (alpha) {
                        sl.AddCell(x, alpha);
                    }
                    x++;
                }

                if (num_cells && cur_cell->x > x) {
                    // At this time, area is 0, that is, 0 to cover << (POLY_SUBPIXEL_SHIFT + 1)
                    alpha = CalculateAlpha(cover << (POLY_SUBPIXEL_SHIFT + 1));
                    if (alpha) {
                        sl.AddSpan(x, cur_cell->x - x, alpha);
                    }
                }
            }

            if (sl.NumSpans())
                break;
            ++m_scan_y;
        }

        sl.Finalize(m_scan_y);
        ++m_scan_y;
        return true;
    }

    /**
     * @brief Convert area cover to gamma cover value to calculate alpha.
     * @since 1.0
     * @version 1.0
     */
    template <class Clip>
    GRAPHIC_GEOMETRY_INLINE unsigned RasterizerScanlineAntiAlias<Clip>::CalculateAlpha(int area) const
    {
        int cover = area >> (POLY_SUBPIXEL_SHIFT * 2 + 1 - AA_SHIFT);

        if (cover < 0)
            cover = -cover;
        if (m_filling_rule == FILL_EVEN_ODD) {
            cover &= AA_MASK2;
            if (cover > AA_SCALE) {
                cover = AA_SCALE2 - cover;
            }
        }
        if (cover > AA_MASK)
            cover = AA_MASK;
        return m_gamma[cover];
    }

    template <class Clip>
    void RasterizerScanlineAntiAlias<Clip>::Reset()
    {
        m_outline.Reset();
        m_status = STATUS_INITIAL;
    }

    template <class Clip>
    void RasterizerScanlineAntiAlias<Clip>::FillingRule(FillingRuleEnum filling_rule)
    {
        m_filling_rule = filling_rule;
    }

    template <class Clip>
    void RasterizerScanlineAntiAlias<Clip>::ClipBox(float x1, float y1,
                                                    float x2, float y2)
    {
        Reset();
        m_clipper.ClipBox(depict_type::UpScale(x1), depict_type::UpScale(y1),
                          depict_type::UpScale(x2), depict_type::UpScale(y2));
    }

    template <class Clip>
    void RasterizerScanlineAntiAlias<Clip>::ResetClipping()
    {
        Reset();
        m_clipper.reset_clipping();
    }

    template <class Clip>
    void RasterizerScanlineAntiAlias<Clip>::ClosePolygon()
    {
        if (m_status == STATUS_LINE_TO) {
            m_clipper.LineTo(m_outline, m_start_x, m_start_y);
            m_status = STATUS_CLOSED;
        }
    }

    template <class Clip>
    void RasterizerScanlineAntiAlias<Clip>::MoveTo(int x, int y)
    {
        if (m_outline.GetSorted())
            Reset();
        if (m_auto_close)
            ClosePolygon();
        m_clipper.move_to(m_start_x = depict_type::downscale(x),
                          m_start_y = depict_type::downscale(y));
        m_status = STATUS_MOVE_TO;
    }

    template <class Clip>
    void RasterizerScanlineAntiAlias<Clip>::LineTo(int x, int y)
    {
        m_clipper.line_to(m_outline,
                          depict_type::downscale(x),
                          depict_type::downscale(y));
        m_status = STATUS_LINE_TO;
    }

    template <class Clip>
    void RasterizerScanlineAntiAlias<Clip>::MoveToByfloat(float x, float y)
    {
        if (m_outline.GetSorted())
            Reset();
        if (m_auto_close)
            ClosePolygon();
        m_clipper.MoveTo(m_start_x = depict_type::UpScale(x),
                         m_start_y = depict_type::UpScale(y));
        m_status = STATUS_MOVE_TO;
    }

    template <class Clip>
    void RasterizerScanlineAntiAlias<Clip>::LineToByfloat(float x, float y)
    {
        m_clipper.LineTo(m_outline,
                         depict_type::UpScale(x),
                         depict_type::UpScale(y));
        m_status = STATUS_LINE_TO;
    }

    template <class Clip>
    void RasterizerScanlineAntiAlias<Clip>::AddVertex(float x, float y, unsigned cmd)
    {
        if (IsMoveTo(cmd)) {
            MoveToByfloat(x, y);
        } else if (IsVertex(cmd)) {
            LineToByfloat(x, y);
        } else if (IsClose(cmd)) {
            ClosePolygon();
        }
    }

    template <class Clip>
    void RasterizerScanlineAntiAlias<Clip>::EdgeMake(int x1, int y1, int x2, int y2)
    {
        if (m_outline.GetSorted())
            Reset();
        m_clipper.move_to(depict_type::downscale(x1), depict_type::downscale(y1));
        m_clipper.line_to(m_outline,
                          depict_type::downscale(x2),
                          depict_type::downscale(y2));
        m_status = STATUS_MOVE_TO;
    }

    template <class Clip>
    void RasterizerScanlineAntiAlias<Clip>::EdgeMakeUsingfloat(float x1, float y1, float x2, float y2)
    {
        if (m_outline.GetSorted())
            Reset();
        m_clipper.move_to(depict_type::UpScale(x1), depict_type::UpScale(y1));
        m_clipper.line_to(m_outline,
                          depict_type::UpScale(x2),
                          depict_type::UpScale(y2));
        m_status = STATUS_MOVE_TO;
    }

    template <class Clip>
    void RasterizerScanlineAntiAlias<Clip>::Sort()
    {
        if (m_auto_close)
            ClosePolygon();
        m_outline.SortAllCells();
    }

    template <class Clip>
    GRAPHIC_GEOMETRY_INLINE bool RasterizerScanlineAntiAlias<Clip>::RewindScanlines()
    {
        if (m_auto_close)
            ClosePolygon();
        m_outline.SortAllCells();
        if (m_outline.GetTotalCells() == 0) {
            return false;
        }
        m_scan_y = m_outline.Miny();
        return true;
    }

    template <class Clip>
    GRAPHIC_GEOMETRY_INLINE bool RasterizerScanlineAntiAlias<Clip>::NavigateScanline(int y)
    {
        if (m_auto_close)
            ClosePolygon();
        m_outline.SortAllCells();
        if (m_outline.GetTotalCells() == 0 ||
            y < m_outline.Miny() ||
            y > m_outline.MaxY()) {
            return false;
        }
        m_scan_y = y;
        return true;
    }

    template <class Clip>
    bool RasterizerScanlineAntiAlias<Clip>::HitTest(int tx, int ty)
    {
        if (!NavigateScanline(ty))
            return false;
        ScanlineHitRegionMeasure sl(tx);
        SweepScanline(sl);
        return sl.GetHitMeasure();
    }
} // namespace OHOS
#endif
