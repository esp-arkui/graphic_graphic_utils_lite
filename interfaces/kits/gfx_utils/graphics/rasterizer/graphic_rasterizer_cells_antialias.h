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
 * @file graphic_rasterizer_cells_antialias.h
 * @brief Defines Grating cell (anti aliasing)
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_RASTERIZERCELLSANTIALIAS_INCLUDED
#define GRAPHIC_RASTERIZERCELLSANTIALIAS_INCLUDED

#include <cstdlib>
#include <cstring>
#include <limits>

#include "gfx_utils/graphics/common/graphic_common_math.h"
#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_array.h"

namespace OHOS {
    // There is no constructor defined for pixel cells,
    // which is to avoid the additional overhead of allocating cell arrays
    struct CellBuildAntiAlias {
        int x;
        int y;
        int cover;
        int area;

        void Initial()
        {
            x = (std::numeric_limits<int>::max)();
            y = (std::numeric_limits<int>::max)();
            cover = 0;
            area = 0;
        }

        void Style(const CellBuildAntiAlias&)
        {}

        int NotEqual(int ex, int ey, const CellBuildAntiAlias&) const
        {
            return ((unsigned)ex - (unsigned)x) | ((unsigned)ey - (unsigned)y);
        }
    };

    template <class Cell>
    class RasterizerCellsAntiAlias {
        struct SortedYLevel {
            unsigned start;
            unsigned num;
        };

        /**
         * @brief Build the offset of 'cell unit', mask mask, cell pool capacity, etc
         * @since 1.0
         * @version 1.0
         */
        enum CellBlockScaleEnum {
            CELL_BLOCK_SHIFT = 12,
            CELL_BLOCK_SIZE = 1 << CELL_BLOCK_SHIFT,
            CELL_BLOCK_MASK = CELL_BLOCK_SIZE - 1,
            CELL_BLOCK_POOL = 256
        };

        enum DxLimitEnum {
            DX_LIMIT = CONSTITUTION << POLY_SUBPIXEL_SHIFT
        };

    public:
        using CellType = Cell;
        using SelfType = RasterizerCellsAntiAlias<Cell>;

        ~RasterizerCellsAntiAlias();

        /**
         * @brief RasterizerCellsAntiAlias Class constructor
         * initialization m_num_blocks,m_max_blocks,m_curr_block etc
         * @since 1.0
         * @version 1.0
         */
        RasterizerCellsAntiAlias(unsigned cell_block_limit = 1024);

        /**
         * Reinitialize settings m_num_blocks,m_max_blocks,m_curr_block etc。
         * @since 1.0
         * @version 1.0
         */
        void Reset();
        void Style(const CellType& style_cell);

        /**
         * @brief According to the incoming 2 coordinate points (both with sub pixels),
         * The process of constructing rasterized cell points is from y to X.
         * @since 1.0
         * @version 1.0
         */
        void LineOperate(int x1, int y1, int x2, int y2);

        /**
         * @brief The rasterization process builds the coordinate range of the entity.
         * @since 1.0
         * @version 1.0
         */
        int MinX() const
        {
            return m_min_x;
        }
        int Miny() const
        {
            return m_min_y;
        }
        int MaxX() const
        {
            return m_max_x;
        }
        int MaxY() const
        {
            return m_max_y;
        }

        /**
         * @brief In the rasterization process, all cells are rasterized according to
         * Sort from left to right and from top to bottom.
         * @since 1.0
         * @version 1.0
         */
        void SortAllCells();

        unsigned GetTotalCells() const
        {
            return m_num_cells;
        }

        /**
         * @brief In the process of rasterization, it is calculated according to the coordinate height of Y
         * Total number of cells.
         * @since 1.0
         * @version 1.0
         */
        unsigned ScanlineNumCells(unsigned yLevel) const
        {
            return m_sorted_y[yLevel - m_min_y].num;
        }

        /**
         * @brief In the process of rasterization, it is calculated according to the coordinate height of Y
         * The first address of the sorted cells array.
         * @since 1.0
         * @version 1.0
         */
        const CellType * const *ScanlineCells(unsigned yLevel) const
        {
            return m_sorted_cells.Data() + m_sorted_y[yLevel - m_min_y].start;
        }

        bool GetSorted() const
        {
            return m_sorted;
        }

    private:
        RasterizerCellsAntiAlias(const SelfType&);
        const SelfType& operator=(const SelfType&);

        /**
         * @brief Set the current cell during rasterization.
         * @since 1.0
         * @version 1.0
         */
        void SetCurrentCell(int x, int y);

        void OutLineLegal(int x1, int y1, int x2, int y2);

        /**
         * @brief Add the current cell during rasterization.
         * @since 1.0
         * @version 1.0
         */
        void AddCurrentCell();

        /**
         * @brief n the rasterization process, the horizontal direction is
         * from x1 to x2 according to the coordinate height value of ey,
         * The filling process of cell cells longitudinally from sub-pixel mask y1 to sub-pixel mask y2.
         * @since 1.0
         * @version 1.0
         */
        void RenderHorizonline(int ey, int x1, int submask_flags_y1, int x2, int submask_flags_y2);

        /**
         * @brief Allocate array space for cells during rasterization.
         * @since 1.0
         * @version 1.0
         */
        void AllocateBlock();

    private:
        unsigned m_num_blocks;
        unsigned m_max_blocks;
        unsigned m_curr_block;
        unsigned m_num_cells;
        unsigned m_cell_block_limit;
        CellType** m_cells;
        CellType* m_curr_cell_ptr;
        GeometryPlainDataVector<CellType*> m_sorted_cells;
        GeometryPlainDataVector<SortedYLevel> m_sorted_y;
        CellType m_curr_cell;
        CellType m_style_cell;
        int m_min_x;
        int m_min_y;
        int m_max_x;
        int m_max_y;
        bool m_sorted;
    };

    class ScanlineHitRegionMeasure {
    public:
        ScanlineHitRegionMeasure(int x)
            : mXCoordinate(x), mHitMeasureFlags(false)
        {}

        void AddCellInContainer(int x, int)
        {
            if (mXCoordinate == x)
                mHitMeasureFlags = true;
        }
        void AddSpanInContainer(int x, int len, int)
        {
            if (mXCoordinate >= x && mXCoordinate < x + len)
                mHitMeasureFlags = true;
        }
        unsigned GetNumberSpans() const
        {
            return 1;
        }
        bool GetHitMeasure() const
        {
            return mHitMeasureFlags;
        }

    private:
        int mXCoordinate;
        bool mHitMeasureFlags;
    };

    template <class Cell>
    RasterizerCellsAntiAlias<Cell>::~RasterizerCellsAntiAlias()
    {
        if (m_num_blocks) {
            CellType** ptr = m_cells + m_num_blocks - 1;
            while (m_num_blocks--) {
                GeometryArrayAllocator<CellType>::Deallocate(*ptr, CELL_BLOCK_SIZE);
                ptr--;
            }
            GeometryArrayAllocator<CellType*>::Deallocate(m_cells, m_max_blocks);
        }
    }

    /**
     * @brief RasterizerCellsAntiAlias Class constructor
     * initialization m_num_blocks,m_max_blocks,m_curr_block Other attributes
     * @since 1.0
     * @version 1.0
     */
    template <class Cell>
    RasterizerCellsAntiAlias<Cell>::RasterizerCellsAntiAlias(unsigned cell_block_limit)
        : m_num_blocks(0),
        m_max_blocks(0),
        m_curr_block(0),
        m_num_cells(0),
        m_cell_block_limit(cell_block_limit),
        m_cells(0),
        m_curr_cell_ptr(0),
        m_sorted_cells(),
        m_sorted_y(),
        m_min_x((std::numeric_limits<int>::max)()),
        m_min_y((std::numeric_limits<int>::max)()),
        m_max_x((std::numeric_limits<int>::min)()),
        m_max_y((std::numeric_limits<int>::min)()),
        m_sorted(false)
    {
        m_style_cell.Initial();
        m_curr_cell.Initial();
    }

    /**
     * Reinitialize settings m_num_blocks,m_max_blocks,m_curr_block and other attributes.
     * @since 1.0
     * @version 1.0
     */
    template <class Cell>
    void RasterizerCellsAntiAlias<Cell>::Reset()
    {
        m_num_cells = 0;
        m_curr_block = 0;
        m_curr_cell.Initial();
        m_style_cell.Initial();
        m_sorted = false;
        m_min_x = (std::numeric_limits<int>::max)();
        m_min_y = (std::numeric_limits<int>::max)();
        m_max_x = (std::numeric_limits<int>::min)();
        m_max_y = (std::numeric_limits<int>::min)();
    }

    /**
     * @brief Add the current cell during rasterization.
     * @since 1.0
     * @version 1.0
     */
    template <class Cell>
    GRAPHIC_GEOMETRY_INLINE void RasterizerCellsAntiAlias<Cell>::AddCurrentCell()
    {
        bool areaCoverFlags = m_curr_cell.area | m_curr_cell.cover;
        if (areaCoverFlags) {
            // Reach CELL_BLOCK_MASK After the number of mask, re allocate memory
            if ((m_num_cells & CELL_BLOCK_MASK) == 0) {
                // Exceeds the memory block size limit. The default is 1024 limit
                if (m_num_blocks >= m_cell_block_limit) {
                    return;
                }
                AllocateBlock();
            }
            *m_curr_cell_ptr++ = m_curr_cell;
            ++m_num_cells;
        }
    }

    /**
     * @brief Set the current cell during rasterization.
     * @since 1.0
     * @version 1.0
     */
    template <class Cell>
    GRAPHIC_GEOMETRY_INLINE void RasterizerCellsAntiAlias<Cell>::SetCurrentCell(int x, int y)
    {
        if (m_curr_cell.NotEqual(x, y, m_style_cell)) {
            AddCurrentCell();
            m_curr_cell.Style(m_style_cell);
            m_curr_cell.x = x;
            m_curr_cell.y = y;
            m_curr_cell.cover = 0;
            m_curr_cell.area = 0;
        }
    }

    template <class Cell>
    GRAPHIC_GEOMETRY_INLINE void RasterizerCellsAntiAlias<Cell>::OutLineLegal(int x1, int y1, int x2, int y2)
    {
        /**
         * outline range
         */
        if (x1 < m_min_x)
            m_min_x = x1;
        if (x1 > m_max_x)
            m_max_x = x1;
        if (y1 < m_min_y)
            m_min_y = y1;
        if (y1 > m_max_y)
            m_max_y = y1;
        if (x2 < m_min_x)
            m_min_x = x2;
        if (x2 > m_max_x)
            m_max_x = x2;
        if (y2 < m_min_y)
            m_min_y = y2;
        if (y2 > m_max_y)
            m_max_y = y2;
    }

    /**
     * @brief In the rasterization process, according to the coordinate height value of ey,
     * x1 to x2 in 1 / 256 pixel horizontally,
     * The filling process of cell cells longitudinally from sub-pixel mask y1 to sub-pixel mask y2.
     * @since 1.0
     * @version 1.0
     */
    template <class Cell>
    GRAPHIC_GEOMETRY_INLINE void RasterizerCellsAntiAlias<Cell>::RenderHorizonline(
        int ey, int x1, int poly_subpixel_mask_y1, int x2, int poly_subpixel_mask_y2)
    {
        /**
         * Take out the mask value of the last 8 bits, namely the color mask,
         * from the points in units of 1 / 256 pixels
         */
        int submask_flags_x1 = x1 & POLY_SUBPIXEL_MASK;
        int submask_flags_x2 = x2 & POLY_SUBPIXEL_MASK;
        /**
         * The coordinates of the first 24 bits are extracted from the points in units of 1 / 256 pixels
         */
        int pixel_x1 = x1 >> POLY_SUBPIXEL_SHIFT;
        int pixel_x2 = x2 >> POLY_SUBPIXEL_SHIFT;

        int delta, deltay_mask, first;
        long long dx;
        int increase, lift_dx_mask, mod_dx_mask, rem_dx_mask;
        /**
         * The color mask of the two points is the same. Add the settings directly and return.
         */
        if (poly_subpixel_mask_y2 == poly_subpixel_mask_y1) {
            SetCurrentCell(pixel_x2, ey);
            return;
        }

        /**
         * The pixel coordinates of the two points are the same and are directly calculated as a cell.
         */
        if (pixel_x1 == pixel_x2) {
            delta = poly_subpixel_mask_y2 - poly_subpixel_mask_y1;
            m_curr_cell.cover += delta;
            m_curr_cell.area += (submask_flags_x1 + submask_flags_x2) * delta;
            return;
        }
        /**
         * hline At the beginning of the process, the cells area adjacent to the same organization is rendered
         */
        first = POLY_SUBPIXEL_SCALE;
        increase = 1;

        /**
         * Convert from submask_flags_x1 to POLY_SUBPIXEL_SCALE to calculate deltax * deltay
         */
        deltay_mask = (POLY_SUBPIXEL_SCALE - submask_flags_x1) * (poly_subpixel_mask_y2 - poly_subpixel_mask_y1);
        dx = (long long)x2 - (long long)x1;

        if (dx < 0) {
            first = 0;
            increase = -1;
            dx = -dx;
            deltay_mask = submask_flags_x1 * (poly_subpixel_mask_y2 - poly_subpixel_mask_y1);
        }

        delta = (int)(deltay_mask / dx);
        mod_dx_mask = (int)(deltay_mask % dx);

        if (mod_dx_mask < 0) {
            mod_dx_mask += dx;
            delta--;
        }
        /**
         * submask_flags_x1+ (0->first)过程
         */
        m_curr_cell.area += (submask_flags_x1 + first) * delta;
        m_curr_cell.cover += delta;

        pixel_x1 += increase;
        SetCurrentCell(pixel_x1, ey);
        poly_subpixel_mask_y1 += delta;

        if (pixel_x1 != pixel_x2) {
            /**
             * delta_subpixel x（ 0 到 POLY_SUBPIXEL_SCALE）  到 ( delta_subpixel_scale_y + delta)
             */
            deltay_mask = POLY_SUBPIXEL_SCALE * (poly_subpixel_mask_y2 - poly_subpixel_mask_y1 + delta);
            rem_dx_mask = (int)(deltay_mask % dx);
            lift_dx_mask = (int)(deltay_mask / dx);

            if (rem_dx_mask < 0) {
                lift_dx_mask--;
                rem_dx_mask += dx;
            }

            mod_dx_mask -= dx;

            while (pixel_x1 != pixel_x2) {
                delta = lift_dx_mask;
                mod_dx_mask += rem_dx_mask;
                if (mod_dx_mask >= 0) {
                    mod_dx_mask -= dx;
                    delta++;
                }

                m_curr_cell.area += POLY_SUBPIXEL_SCALE * delta;
                m_curr_cell.cover += delta;
                poly_subpixel_mask_y1 += delta;
                pixel_x1 += increase;
                SetCurrentCell(pixel_x1, ey);
            }
        }
        delta = poly_subpixel_mask_y2 - poly_subpixel_mask_y1;
        m_curr_cell.cover += delta;
        // From first to POLY_SUBPIXEL_SCALE procedure
        m_curr_cell.area += (submask_flags_x2 + POLY_SUBPIXEL_SCALE - first) * delta;
    }

    template <class Cell>
    GRAPHIC_GEOMETRY_INLINE void RasterizerCellsAntiAlias<Cell>::Style(const CellType& style_cell)
    {
        m_style_cell.style(style_cell);
    }

    /**
     * @brief According to the incoming 2 coordinate points (both with sub pixels),
     * The process of constructing rasterized cell points is from y to X.
     * @since 1.0
     * @version 1.0
     */
    template <class Cell>
    void RasterizerCellsAntiAlias<Cell>::LineOperate(int x1, int y1, int x2, int y2)
    {
        long long dx = (long long)x2 - (long long)x1;
        /**
         * If dx exceeds the limit, a compromise is adopted to calculate the line.
         */
        if (dx >= DX_LIMIT || dx <= -DX_LIMIT) {
            int cx = (int)(((long long)x1 + (long long)x2) >> 1);
            int cy = (int)(((long long)y1 + (long long)y2) >> 1);
            LineOperate(x1, y1, cx, cy);
            LineOperate(cx, cy, x2, y2);
        }
        /**
         * The coordinates of the first 24 bits are extracted from the points in units of 1 / 256 pixels
         */
        long long dy = (long long)y2 - (long long)y1;
        int ex1 = x1 >> POLY_SUBPIXEL_SHIFT;
        int ex2 = x2 >> POLY_SUBPIXEL_SHIFT;
        int ey1 = y1 >> POLY_SUBPIXEL_SHIFT;
        int ey2 = y2 >> POLY_SUBPIXEL_SHIFT;
        /**
         * Take out the mask value of the last 8 bits from
         * the points with 1 / 256 pixel as the unit, that is, the color mask
         */
        int submask_flags_y1 = y1 & POLY_SUBPIXEL_MASK;
        int submask_flags_y2 = y2 & POLY_SUBPIXEL_MASK;

        int x_from, x_to;
        int rem_dy_mask, mod_dy_mask, lift_dy_mask, delta, first, increase;
        long long deltax_mask;

        OutLineLegal(ex1, ey1, ex2, ey2);
        SetCurrentCell(ex1, ey1);

        /**
         * If the Y values of the two points are the same, they will be directly rendered horizontally,
         * The horizontal coordinate spacing is from X1 - > x2 in 1 / 256 pixels,
         * Color mask spacing is from submask_flags_y1 to submask_flags_y2
         */
        if (ey1 == ey2) {
            RenderHorizonline(ey1, x1, submask_flags_y1, x2, submask_flags_y2);
            return;
        }
        /**
         * For the processing of vertical lines, start - > end cells are calculated, and then the line is calculated
         * The above general attribute area - > cover is for each y value,
         * There is only one cell, so it is no longer called RenderHorizonline()
         */
        increase = 1;
        //  Vertical line
        if (dx == 0) {
            /**
             * The coordinates of the first 24 bits are extracted from the points in units of 1 / 256 pixels
             */
            int ex = x1 >> POLY_SUBPIXEL_SHIFT;
            /**
             * Take out the number of decimal points and occupy 2 spaces
             */
            int two_fx = (x1 - (ex << POLY_SUBPIXEL_SHIFT)) << 1;
            int area;
            // 256
            first = POLY_SUBPIXEL_SCALE;
            if (dy < 0) {
                first = 0;
                increase = -1;
            }

            x_from = x1;
            /**
             * From submask_flags_y1 to first process
             * RenderHorizonline(pixel_y1, x_from, submask_flags_y1, x_from, first);
             * The color mask is from submask_flags_y1->first
             */
            delta = first - submask_flags_y1;
            m_curr_cell.cover += delta;
            m_curr_cell.area += two_fx * delta;

            ey1 += increase;
            SetCurrentCell(ex, ey1);
            /**
             * The color mask is from (poly_subpixel_scale - first) -> first
             */
            delta = first + first - POLY_SUBPIXEL_SCALE;
            area = two_fx * delta;
            while (ey1 != ey2) {
                // from poly_subpixel_scale - first to  first
                m_curr_cell.cover = delta;
                m_curr_cell.area = area;
                ey1 += increase;
                SetCurrentCell(ex, ey1);
            }
            /**
             * The color mask is from poly_subpixel_scale - first to  submask_flags_y2
             *
             * RenderHorizonline(pixel_y1, x_from, poly_subpixel_scale - first, x_from, submask_flags_y2);
             */
            delta = submask_flags_y2 - POLY_SUBPIXEL_SCALE + first;
            m_curr_cell.cover += delta;
            m_curr_cell.area += two_fx * delta;
            return;
        }
        // ok, we have to render several hlines
        // dx* mask Difference
        /**
         * The color mask is from the 颜色mask是从submask_flags_y1 to POLY_SUBPIXEL_SCALE Process of scale
         */
        deltax_mask = (POLY_SUBPIXEL_SCALE - submask_flags_y1) * dx;
        first = POLY_SUBPIXEL_SCALE;

        if (dy < 0) {
            deltax_mask = submask_flags_y1 * dx;
            first = 0;
            increase = -1;
            dy = -dy;
        }

        delta = (int)(deltax_mask / dy);
        mod_dy_mask = (int)(deltax_mask % dy);

        if (mod_dy_mask < 0) {
            delta--;
            mod_dy_mask += dy;
        }

        x_from = x1 + delta;
        RenderHorizonline(ey1, x1, submask_flags_y1, x_from, first);

        ey1 += increase;
        SetCurrentCell(x_from >> POLY_SUBPIXEL_SHIFT, ey1);

        if (ey1 != ey2) {
            deltax_mask = POLY_SUBPIXEL_SCALE * dx;
            lift_dy_mask = (int)(deltax_mask / dy);
            rem_dy_mask = (int)(deltax_mask % dy);

            if (rem_dy_mask < 0) {
                lift_dy_mask--;
                rem_dy_mask += dy;
            }
            mod_dy_mask -= dy;

            while (ey1 != ey2) {
                delta = lift_dy_mask;
                mod_dy_mask += rem_dy_mask;
                if (mod_dy_mask >= 0) {
                    mod_dy_mask -= dy;
                    delta++;
                }

                x_to = x_from + delta;
                RenderHorizonline(ey1, x_from, POLY_SUBPIXEL_SCALE - first, x_to, first);
                x_from = x_to;

                ey1 += increase;
                SetCurrentCell(x_from >> POLY_SUBPIXEL_SHIFT, ey1);
            }
        }
        RenderHorizonline(ey1, x_from, POLY_SUBPIXEL_SCALE - first, x2, submask_flags_y2);
    }

    /**
     * @brief Allocate array space for cells during rasterization.
     * @since 1.0
     * @version 1.0
     */
    template <class Cell>
    void RasterizerCellsAntiAlias<Cell>::AllocateBlock()
    {
        if (m_curr_block >= m_num_blocks) {
            if (m_num_blocks >= m_max_blocks) {
                CellType** new_cells =
                    GeometryArrayAllocator<CellType*>::Allocate(m_max_blocks +
                                                                CELL_BLOCK_POOL);

                if (m_cells) {
                    if (memcpy_s(new_cells, m_max_blocks * sizeof(CellType*),
                                 m_cells, m_max_blocks * sizeof(CellType*)) != EOK) {
                    }
                    GeometryArrayAllocator<CellType*>::Deallocate(m_cells, m_max_blocks);
                }
                m_cells = new_cells;
                m_max_blocks += CELL_BLOCK_POOL;
            }

            m_cells[m_num_blocks++] =
                GeometryArrayAllocator<CellType>::Allocate(CELL_BLOCK_SIZE);
        }
        m_curr_cell_ptr = m_cells[m_curr_block++];
    }

    /**
     * @brief The exchange of cells in the rasterization process.
     * @since 1.0
     * @version 1.0
     */
    template <class T>
    GRAPHIC_GEOMETRY_INLINE void SwapCells(T* oneCells, T* twoCells)
    {
        T tempCells = *oneCells;
        *oneCells = *twoCells;
        *twoCells = tempCells;
    }

    /**
     * @brief In the rasterization process, all cells are sorted quickly.
     * @since 1.0
     * @version 1.0
     */
    template <class Cell>
    void QsortCells(Cell** start, unsigned num)
    {
        const int QSORT_THRESHOLD = 9;
        const int stackSize = 80;
        Cell** stack[stackSize];
        Cell*** top;
        Cell** limit;
        Cell** base;

        limit = start + num;
        base = start;
        top = stack;

        while (1) {
            int len = int(limit - base);

            Cell** iIndex;
            Cell** jIndex;
            Cell** pivot;

            if (len > QSORT_THRESHOLD) {
                /**
                 * First exchange base + len / 2 as the pivot
                 */
                pivot = base + len / TWO_TIMES;
                SwapCells(base, pivot);

                iIndex = base + 1;
                jIndex = limit - 1;

                /**
                 * Sorting guarantees the value of * i < = * the value of base < = * the value of j
                 */
                if ((*jIndex)->x < (*iIndex)->x) {
                    SwapCells(iIndex, jIndex);
                }

                if ((*base)->x < (*iIndex)->x) {
                    SwapCells(base, iIndex);
                }

                if ((*jIndex)->x < (*base)->x) {
                    SwapCells(base, jIndex);
                }

                while (1) {
                    int x = (*base)->x;
                    do {
                        iIndex++;
                    } while ((*iIndex)->x < x);
                    do {
                        jIndex--;
                    } while (x < (*jIndex)->x);

                    if (iIndex > jIndex) {
                        break;
                    }
                    SwapCells(iIndex, jIndex);
                }

                SwapCells(base, jIndex);
                /**
                 * push Pressed in the largest sub array sub array
                 */
                if (jIndex - base > limit - iIndex) {
                    top[0] = base;
                    top[1] = jIndex;
                    base = iIndex;
                } else {
                    top[0] = iIndex;
                    top[1] = limit;
                    limit = jIndex;
                }
                top += TWO_STEP;
            } else {
                /**
                 * When the sub-array becomes smaller, insert sort is performed using
                 */
                jIndex = base;
                iIndex = jIndex + 1;

                for (; iIndex < limit; iIndex++) {
                    for (; jIndex[1]->x < (*jIndex)->x; jIndex--) {
                        SwapCells(jIndex + 1, jIndex);
                        if (jIndex == base) {
                            break;
                        }
                    }
                    jIndex = iIndex;
                }

                if (top > stack) {
                    top -= TWO_STEP;
                    base = top[0];
                    limit = top[1];
                } else {
                    break;
                }
            }
        }
    }

    /**
     * @brief In the rasterization process, all cells are rasterized according to
     * Sort from left to right and from top to bottom.
     * @since 1.0
     * @version 1.0
     */
    template <class Cell>
    void RasterizerCellsAntiAlias<Cell>::SortAllCells()
    {
        if (m_sorted)
            return; // Perform sort only the first time.

        AddCurrentCell();
        m_curr_cell.x = (std::numeric_limits<int>::max)();
        m_curr_cell.y = (std::numeric_limits<int>::max)();
        m_curr_cell.cover = 0;
        m_curr_cell.area = 0;

        if (m_num_cells == 0) {
            return;
        }

        // Allocate the array of cell pointers
        m_sorted_cells.Allocate(m_num_cells, CELLS_SIZE);

        // Allocate and zero the Y array
        m_sorted_y.Allocate(m_max_y - m_min_y + 1, CELLS_SIZE);
        m_sorted_y.CleanData();

        // Create the Y-histogram (count the numbers of cells for each Y)
        CellType** block_ptr = m_cells;
        CellType* cell_ptr;
        unsigned nb = m_num_cells;
        unsigned i;
        while (nb) {
            cell_ptr = *block_ptr++;
            i = (nb > CELL_BLOCK_SIZE) ? unsigned(CELL_BLOCK_SIZE) : nb;
            nb -= i;
            while (i--) {
                m_sorted_y[cell_ptr->y - m_min_y].start++;
                ++cell_ptr;
            }
        }

        // Convert the Y-histogram into the array of starting indexes
        unsigned start = 0;
        for (i = 0; i < m_sorted_y.Size(); i++) {
            unsigned v = m_sorted_y[i].start;
            m_sorted_y[i].start = start;
            start += v;
        }

        // Fill the cell pointer array sorted by Y
        block_ptr = m_cells;
        nb = m_num_cells;
        while (nb) {
            cell_ptr = *block_ptr++;
            i = (nb > CELL_BLOCK_SIZE) ? unsigned(CELL_BLOCK_SIZE) : nb;
            nb -= i;
            while (i--) {
                SortedYLevel& curr_y = m_sorted_y[cell_ptr->y - m_min_y];
                m_sorted_cells[curr_y.start + curr_y.num] = cell_ptr;
                ++curr_y.num;
                ++cell_ptr;
            }
        }

        // Finally arrange the X-arrays
        for (i = 0; i < m_sorted_y.Size(); i++) {
            const SortedYLevel& curr_y = m_sorted_y[i];
            if (curr_y.num) {
                QsortCells(m_sorted_cells.Data() + curr_y.start, curr_y.num);
            }
        }
        m_sorted = true;
    }
} // namespace OHOS
#endif
