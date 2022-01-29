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

#include "gfx_utils/diagram/common/graphic_common_math.h"
#include "gfx_utils/diagram/vertexprimitive/graphic_geometry_plaindata_vector.h"

namespace OHOS {
#define ALLOCATE_BLOCK \
    if (numBlocks_ >= maxBlocks_) { \
        CellType** newCells = \
            GeometryArrayAllocator<CellType*>::Allocate(maxBlocks_ + \
                                                        CELL_BLOCK_POOL); \
        if (cells_) { \
            if (memcpy_s(newCells, maxBlocks_ * sizeof(CellType*), \
                         cells_, maxBlocks_ * sizeof(CellType*)) != EOK) { \
            } \
            GeometryArrayAllocator<CellType*>::Deallocate(cells_, maxBlocks_); \
        } \
        cells_ = newCells; \
        maxBlocks_ += CELL_BLOCK_POOL; \
    } \
    cells_[numBlocks_++] = \
        GeometryArrayAllocator<CellType>::Allocate(CELL_BLOCK_SIZE)

#define RENDER_HORIZON_LINE \
    first = POLY_SUBPIXEL_SCALE; \
    increase = 1; \
    /**  Convert from submaskFlagsX1 to POLY_SUBPIXEL_SCALE to calculate deltax * deltay */ \
    deltayMask = (POLY_SUBPIXEL_SCALE - submaskFlagsX1) * (polySubpixelMaskY2 - polySubpixelMaskY1); \
    dx = (long long)x2 - (long long)x1; \
    if (dx < 0) { \
        first = 0; \
        increase = -1; \
        dx = -dx; \
        deltayMask = submaskFlagsX1 * (polySubpixelMaskY2 - polySubpixelMaskY1); \
    } \
    delta = (int)(deltayMask / dx); \
    modDxMask = (int)(deltayMask % dx); \
    if (modDxMask < 0) { \
        modDxMask += dx; \
        delta--; \
    } \
    /* submaskFlagsX1+ (0->first)过程 */ \
    currCell_.area += (submaskFlagsX1 + first) * delta; \
    currCell_.cover += delta; \
    pixelX1 += increase; \
    SetCurrentCell(pixelX1, ey); \
    polySubpixelMaskY1 += delta; \
    if (pixelX1 != pixelX2) { \
        /* delta_subpixel x（ 0 到 POLY_SUBPIXEL_SCALE）  到 ( delta_subpixel_scale_y + delta) */ \
        deltayMask = POLY_SUBPIXEL_SCALE * (polySubpixelMaskY2 - polySubpixelMaskY1 + delta); \
        remDxMask = (int)(deltayMask % dx); \
        liftDxMask = (int)(deltayMask / dx); \
        if (remDxMask < 0) { \
            liftDxMask--; \
            remDxMask += dx; \
        } \
        modDxMask -= dx; \
        while (pixelX1 != pixelX2) { \
            delta = liftDxMask; \
            modDxMask += remDxMask; \
            if (modDxMask >= 0) { \
                modDxMask -= dx; \
                delta++; \
            } \
            currCell_.area += POLY_SUBPIXEL_SCALE * delta; \
            currCell_.cover += delta; \
            polySubpixelMaskY1 += delta; \
            pixelX1 += increase; \
            SetCurrentCell(pixelX1, ey); \
        } \
    } \
    delta = polySubpixelMaskY2 - polySubpixelMaskY1; \
    currCell_.cover += delta; \
    /* From first to POLY_SUBPIXEL_SCALE procedure */ \
    currCell_.area += (submaskFlagsX2 + POLY_SUBPIXEL_SCALE - first) * delta

#define LINE_OPERATE_VERTICAL_LINE \
    int ex = x1 >> POLY_SUBPIXEL_SHIFT; \
    /* Take out the number of decimal points and occupy 2 spaces */ \
    int twoFx = (x1 - (ex << POLY_SUBPIXEL_SHIFT)) << 1; \
    int area; \
    /* 256 */  \
    first = POLY_SUBPIXEL_SCALE; \
    if (dy < 0) { \
        first = 0; \
        increase = -1; \
    } \
    xFrom = x1; \
    /* From submaskFlagsY1 to first process */ \
    /* The color mask is from submaskFlagsY1->first */ \
    delta = first - submaskFlagsY1; \
    currCell_.cover += delta; \
    currCell_.area += twoFx * delta; \
    ey1 += increase; \
    SetCurrentCell(ex, ey1); \
    /* The color mask is from (poly_subpixel_scale - first) -> first */ \
    delta = first + first - POLY_SUBPIXEL_SCALE; \
    area = twoFx * delta; \
    while (ey1 != ey2) { \
        /* from poly_subpixel_scale - first to  first */ \
        currCell_.cover = delta; \
        currCell_.area = area; \
        ey1 += increase; \
        SetCurrentCell(ex, ey1); \
    } \
    /* The color mask is from poly_subpixel_scale - first to  submaskFlagsY2 */ \
    delta = submaskFlagsY2 - POLY_SUBPIXEL_SCALE + first; \
    currCell_.cover += delta; \
    currCell_.area += twoFx * delta

#define LINE_OPERATE_RENDER_SEVERAL_HLINES \
    deltaxMask = (POLY_SUBPIXEL_SCALE - submaskFlagsY1) * dx; \
    first = POLY_SUBPIXEL_SCALE; \
    if (dy < 0) { \
        deltaxMask = submaskFlagsY1 * dx; \
        first = 0; \
        increase = -1; \
        dy = -dy; \
    } \
    delta = (int)(deltaxMask / dy); \
    modDyMask = (int)(deltaxMask % dy); \
    if (modDyMask < 0) { \
        delta--; \
        modDyMask += dy; \
    } \
    xFrom = x1 + delta; \
    RenderHorizonline(ey1, x1, submaskFlagsY1, xFrom, first); \
    ey1 += increase; \
    SetCurrentCell(xFrom >> POLY_SUBPIXEL_SHIFT, ey1); \
    if (ey1 != ey2) { \
        deltaxMask = POLY_SUBPIXEL_SCALE * dx; \
        liftDyMask = (int)(deltaxMask / dy); \
        remDyMask = (int)(deltaxMask % dy); \
        if (remDyMask < 0) { \
            liftDyMask--; \
            remDyMask += dy; \
        } \
        modDyMask -= dy; \
        while (ey1 != ey2) { \
            delta = liftDyMask; \
            modDyMask += remDyMask; \
            if (modDyMask >= 0) { \
                modDyMask -= dy; \
                delta++; \
            } \
            xTo = xFrom + delta; \
            RenderHorizonline(ey1, xFrom, POLY_SUBPIXEL_SCALE - first, xTo, first); \
            xFrom = xTo; \
            ey1 += increase; \
            SetCurrentCell(xFrom >> POLY_SUBPIXEL_SHIFT, ey1); \
        } \
    } \
    RenderHorizonline(ey1, xFrom, POLY_SUBPIXEL_SCALE - first, x2, submaskFlagsY2)

#define QSORT_CELLS_SET \
    SwapCells(base, jIndex); \
    if (jIndex - base > limit - iIndex) { \
        top[0] = base; \
        top[1] = jIndex; \
        base = iIndex; \
    } else { \
        top[0] = iIndex; \
        top[1] = limit; \
        limit = jIndex; \
    } \
    top += TWO_STEP

#define SORT_ALL_CELLS_SETVALUE \
    blockPtr = cells_; \
    nb = numCells_; \
    while (nb) { \
        cellPtr = *blockPtr++; \
        i = (nb > CELL_BLOCK_SIZE) ? unsigned(CELL_BLOCK_SIZE) : nb; \
        nb -= i; \
        while (i--) { \
            SortedYLevel& currY = sortedY_[cellPtr->y - minY_]; \
            sortedCells_[currY.start + currY.num] = cellPtr; \
            ++currY.num; \
            ++cellPtr; \
        } \
    }


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
     * initialization numBlocks_,maxBlocks_,currBlock_ etc
     * @since 1.0
     * @version 1.0
     */
    RasterizerCellsAntiAlias(unsigned cellBlockLimit = 1024);

    /**
     * Reinitialize settings numBlocks_,maxBlocks_,currBlock_ etc。
     * @since 1.0
     * @version 1.0
     */
    void Reset();
    void Style(const CellType& styleCell);

    /**
     * @brief According to the incoming 2 coordinate points (both with sub pixels),
     * The process of constructing rasterized cell points is from y to X.
     * @since 1.0
     * @version 1.0
     */
    void LineOperate(int x1, int y1, int x2, int y2);

    void LineOperateVerticalLine();

    /**
     * @brief The rasterization process builds the coordinate range of the entity.
     * @since 1.0
     * @version 1.0
     */
    int MinX() const
    {
        return minX_;
    }
    int Miny() const
    {
        return minY_;
    }
    int MaxX() const
    {
        return maxX_;
    }
    int MaxY() const
    {
        return maxY_;
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
        return numCells_;
    }

    /**
     * @brief In the process of rasterization, it is calculated according to the coordinate height of Y
     * Total number of cells.
     * @since 1.0
     * @version 1.0
     */
    unsigned ScanlineNumCells(unsigned yLevel) const
    {
        return sortedY_[yLevel - minY_].num;
    }

    /**
     * @brief In the process of rasterization, it is calculated according to the coordinate height of Y
     * The first address of the sorted cells array.
     * @since 1.0
     * @version 1.0
     */
    const CellType * const *ScanlineCells(unsigned yLevel) const
    {
        return sortedCells_.Data() + sortedY_[yLevel - minY_].start;
    }

    bool GetSorted() const
    {
        return sorted_;
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
    void RenderHorizonline(int ey, int x1, int submaskFlagsY1, int x2, int submaskFlagsY2);

    /**
     * @brief Allocate array space for cells during rasterization.
     * @since 1.0
     * @version 1.0
     */
    void AllocateBlock();

private:
    unsigned numBlocks_;
    unsigned maxBlocks_;
    unsigned currBlock_;
    unsigned numCells_;
    unsigned cellBlockLimit_;
    CellType** cells_;
    CellType* currCellPtr_;
    GraphicGeometryPlaindataVector<CellType*> sortedCells_;
    GraphicGeometryPlaindataVector<SortedYLevel> sortedY_;
    CellType currCell_;
    CellType styleCell_;
    int minX_;
    int minY_;
    int maxX_;
    int maxY_;
    bool sorted_;
};

class ScanlineHitRegionMeasure {
public:
    ScanlineHitRegionMeasure(int x)
        : xCoordinate_(x), hitMeasureFlags_(false)
    {}

    void AddCellInContainer(int x, int)
    {
        if (xCoordinate_ == x) {
            hitMeasureFlags_ = true;
        }
    }
    void AddSpanInContainer(int x, int len, int)
    {
        if (xCoordinate_ >= x && xCoordinate_ < x + len) {
            hitMeasureFlags_ = true;
        }
    }
    unsigned GetNumberSpans() const
    {
        return 1;
    }
    bool GetHitMeasure() const
    {
        return hitMeasureFlags_;
    }

private:
    int xCoordinate_;
    bool hitMeasureFlags_;
};

template <class Cell>
RasterizerCellsAntiAlias<Cell>::~RasterizerCellsAntiAlias()
{
    if (numBlocks_) {
        CellType** ptr = cells_ + numBlocks_ - 1;
        while (numBlocks_--) {
            GeometryArrayAllocator<CellType>::Deallocate(*ptr, CELL_BLOCK_SIZE);
            ptr--;
        }
        GeometryArrayAllocator<CellType*>::Deallocate(cells_, maxBlocks_);
    }
}

/**
 * @brief RasterizerCellsAntiAlias Class constructor
 * initialization numBlocks_,maxBlocks_,currBlock_ Other attributes
 * @since 1.0
 * @version 1.0
 */
template <class Cell>
RasterizerCellsAntiAlias<Cell>::RasterizerCellsAntiAlias(unsigned cellBlockLimit)
    : numBlocks_(0),
    maxBlocks_(0),
    currBlock_(0),
    numCells_(0),
    cellBlockLimit_(cellBlockLimit),
    cells_(0),
    currCellPtr_(0),
    sortedCells_(),
    sortedY_(),
    minX_((std::numeric_limits<int>::max)()),
    minY_((std::numeric_limits<int>::max)()),
    maxX_((std::numeric_limits<int>::min)()),
    maxY_((std::numeric_limits<int>::min)()),
    sorted_(false)
{
    styleCell_.Initial();
    currCell_.Initial();
}

/**
 * Reinitialize settings numBlocks_,maxBlocks_,currBlock_ and other attributes.
 * @since 1.0
 * @version 1.0
 */
template <class Cell>
void RasterizerCellsAntiAlias<Cell>::Reset()
{
    numCells_ = 0;
    currBlock_ = 0;
    currCell_.Initial();
    styleCell_.Initial();
    sorted_ = false;
    minX_ = (std::numeric_limits<int>::max)();
    minY_ = (std::numeric_limits<int>::max)();
    maxX_ = (std::numeric_limits<int>::min)();
    maxY_ = (std::numeric_limits<int>::min)();
}

/**
 * @brief Add the current cell during rasterization.
 * @since 1.0
 * @version 1.0
 */
template <class Cell>
void RasterizerCellsAntiAlias<Cell>::AddCurrentCell()
{
    bool areaCoverFlags = currCell_.area | currCell_.cover;
    if (areaCoverFlags) {
        // Reach CELL_BLOCK_MASK After the number of mask, re allocate memory
        if ((numCells_ & CELL_BLOCK_MASK) == 0) {
            // Exceeds the memory block size limit. The default is 1024 limit
            if (numBlocks_ >= cellBlockLimit_) {
                return;
            }
            AllocateBlock();
        }
        *currCellPtr_++ = currCell_;
        ++numCells_;
    }
}

/**
 * @brief Set the current cell during rasterization.
 * @since 1.0
 * @version 1.0
 */
template <class Cell>
inline void RasterizerCellsAntiAlias<Cell>::SetCurrentCell(int x, int y)
{
    if (currCell_.NotEqual(x, y, styleCell_)) {
        AddCurrentCell();
        currCell_.Style(styleCell_);
        currCell_.x = x;
        currCell_.y = y;
        currCell_.cover = 0;
        currCell_.area = 0;
    }
}

template <class Cell>
void RasterizerCellsAntiAlias<Cell>::OutLineLegal(int x1, int y1, int x2, int y2)
{
    /**
     * outline range
     */
    if (x1 < minX_) {
        minX_ = x1;
    }
    if (x1 > maxX_) {
        maxX_ = x1;
    }
    if (y1 < minY_) {
        minY_ = y1;
    }
    if (y1 > maxY_) {
        maxY_ = y1;
    }
    if (x2 < minX_) {
        minX_ = x2;
    }
    if (x2 > maxX_) {
        maxX_ = x2;
    }
    if (y2 < minY_) {
        minY_ = y2;
    }
    if (y2 > maxY_) {
        maxY_ = y2;
    }
}

/**
 * @brief In the rasterization process, according to the coordinate height value of ey,
 * x1 to x2 in 1 / 256 pixel horizontally,
 * The filling process of cell cells longitudinally from sub-pixel mask y1 to sub-pixel mask y2.
 * @since 1.0
 * @version 1.0
 */
template <class Cell>
void RasterizerCellsAntiAlias<Cell>::RenderHorizonline(
    int ey, int x1, int polySubpixelMaskY1, int x2, int polySubpixelMaskY2)
{
    /**
     * Take out the mask value of the last 8 bits, namely the color mask,
     * from the points in units of 1 / 256 pixels
     */
    int submaskFlagsX1 = x1 & POLY_SUBPIXEL_MASK;
    int submaskFlagsX2 = x2 & POLY_SUBPIXEL_MASK;
    /**
     * The coordinates of the first 24 bits are extracted from the points in units of 1 / 256 pixels
     */
    int pixelX1 = x1 >> POLY_SUBPIXEL_SHIFT;
    int pixelX2 = x2 >> POLY_SUBPIXEL_SHIFT;

    int delta, deltayMask, first;
    long long dx;
    int increase, liftDxMask, modDxMask, remDxMask;
    /**
     * The color mask of the two points is the same. Add the settings directly and return.
     */
    if (polySubpixelMaskY2 == polySubpixelMaskY1) {
        SetCurrentCell(pixelX2, ey);
        return;
    }

    // The pixel coordinates of the two points are the same and are directly calculated as a cell.
    if (pixelX1 == pixelX2) {
        delta = polySubpixelMaskY2 - polySubpixelMaskY1;
        currCell_.cover += delta;
        currCell_.area += (submaskFlagsX1 + submaskFlagsX2) * delta;
        return;
    }
    // hline At the beginning of the process, the cells area adjacent to the same organization is rendered
    RENDER_HORIZON_LINE;
}

template <class Cell>
inline void RasterizerCellsAntiAlias<Cell>::Style(const CellType& styleCell)
{
    styleCell_.style(styleCell);
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
    int submaskFlagsY1 = y1 & POLY_SUBPIXEL_MASK;
    int submaskFlagsY2 = y2 & POLY_SUBPIXEL_MASK;

    int xFrom, xTo;
    int remDyMask, modDyMask, liftDyMask, delta, first, increase;
    long long deltaxMask;

    OutLineLegal(ex1, ey1, ex2, ey2);
    SetCurrentCell(ex1, ey1);

    /**
     * If the Y values of the two points are the same, they will be directly rendered horizontally,
     * The horizontal coordinate spacing is from X1 - > x2 in 1 / 256 pixels,
     * Color mask spacing is from submaskFlagsY1 to submaskFlagsY2
     */
    if (ey1 == ey2) {
        RenderHorizonline(ey1, x1, submaskFlagsY1, x2, submaskFlagsY2);
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
        LINE_OPERATE_VERTICAL_LINE;
        return;
    }
    // ok, we have to render several hlines
    // dx* mask Difference
    /**
     * The color mask is from the 颜色mask是从submaskFlagsY1 to POLY_SUBPIXEL_SCALE Process of scale
     */
    LINE_OPERATE_RENDER_SEVERAL_HLINES;
}

/**
 * @brief Allocate array space for cells during rasterization.
 * @since 1.0
 * @version 1.0
 */
template <class Cell>
void RasterizerCellsAntiAlias<Cell>::AllocateBlock()
{
    if (currBlock_ >= numBlocks_) {
        ALLOCATE_BLOCK;
    }
    currCellPtr_ = cells_[currBlock_++];
}

/**
 * @brief The exchange of cells in the rasterization process.
 * @since 1.0
 * @version 1.0
 */
template <class T>
inline void SwapCells(T* oneCells, T* twoCells)
{
    T tempCells = *oneCells;
    *oneCells = *twoCells;
    *twoCells = tempCells;
}

template <class Cell>
void QsortCellsSweep(Cell*** base, Cell*** iIndex, Cell*** jIndex)
{
    /**
     * Sorting guarantees the value of * i < = * the value of base < = * the value of j
     */
    if ((**jIndex)->x < (**iIndex)->x) {
        SwapCells(*iIndex, *jIndex);
    }

    if ((**base)->x < (**iIndex)->x) {
        SwapCells(*base, *iIndex);
    }

    if ((**jIndex)->x < (**base)->x) {
        SwapCells(*base, *jIndex);
    }

    while (1) {
        int x = (**base)->x;
        do {
            (*iIndex)++;
        } while ((**iIndex)->x < x);
        do {
            (*jIndex)--;
        } while (x < (**jIndex)->x);

        if ((*iIndex) > (*jIndex)) {
            break;
        }
        SwapCells(*iIndex, *jIndex);
    }
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

            QsortCellsSweep(&base, &iIndex, &jIndex);

            QSORT_CELLS_SET;
        } else {
            /**
             * When the sub-array becomes smaller, insert sort is performed using
             */
            jIndex = base;
            iIndex = jIndex + 1;
            QsortCellsFor(&iIndex, &jIndex, &limit, &base);
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

template <class Cell>
void QsortCellsFor(Cell*** iIndex, Cell*** jIndex, Cell*** limit, Cell*** base)
{
    for (; *iIndex < *limit; (*iIndex)++) {
        for (; (*jIndex)[1]->x < (**jIndex)->x; (*jIndex)--) {
            SwapCells((*jIndex) + 1, *jIndex);
            if ((*jIndex) == (*base)) {
                break;
            }
        }
        *jIndex = *iIndex;
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
    if (sorted_) {
        return; // Perform sort only the first time.
    }

    AddCurrentCell();
    currCell_.x = (std::numeric_limits<int>::max)();
    currCell_.y = (std::numeric_limits<int>::max)();
    currCell_.cover = 0;
    currCell_.area = 0;
    if (numCells_ == 0) {
        return;
    }

    // Allocate the array of cell pointers
    sortedCells_.Allocate(numCells_, CELLS_SIZE);

    // Allocate and zero the Y array
    sortedY_.Allocate(maxY_ - minY_ + 1, CELLS_SIZE);
    sortedY_.CleanData();

    // Create the Y-histogram (count the numbers of cells for each Y)
    CellType** blockPtr = cells_;
    CellType* cellPtr = nullptr;
    unsigned nb = numCells_;
    unsigned i = 0;
    while (nb) {
        cellPtr = *blockPtr++;
        i = (nb > CELL_BLOCK_SIZE) ? unsigned(CELL_BLOCK_SIZE) : nb;
        nb -= i;
        while (i--) {
            sortedY_[cellPtr->y - minY_].start++;
            ++cellPtr;
        }
    }

    // Convert the Y-histogram into the array of starting indexes
    unsigned start = 0;
    for (i = 0; i < sortedY_.Size(); i++) {
        unsigned v = sortedY_[i].start;
        sortedY_[i].start = start;
        start += v;
    }

    // Fill the cell pointer array sorted by Y
    SORT_ALL_CELLS_SETVALUE

    // Finally arrange the X-arrays
    for (i = 0; i < sortedY_.Size(); i++) {
        const SortedYLevel& currY = sortedY_[i];
        if (currY.num) {
            QsortCells(sortedCells_.Data() + currY.start, currY.num);
        }
    }
    sorted_ = true;
}
} // namespace OHOS
#endif
