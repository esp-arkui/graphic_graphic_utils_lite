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
#define SWEEP_SCANLINE_CALCULATE_ALPHA \
    alpha = CalculateAlpha((cover << (POLY_SUBPIXEL_SHIFT + 1)) - area); \
    if (alpha) { \
        sl.AddCell(x, alpha); \
    } \
    x++

#define SWEEP_SCANLINE_CALCULATE_ALPHA_SPAN \
    alpha = CalculateAlpha(cover << (POLY_SUBPIXEL_SHIFT + 1)); \
    if (alpha) { \
        sl.AddSpan(x, curCell->x - x, alpha); \
    }

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
class GraphicRasterizerScanlineAntialias {
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
    using ClipType = Clip;
    using DepictType = typename Clip::DepictType;

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
    GraphicRasterizerScanlineAntialias(unsigned cell_block_limit = (1 << (AA_SHIFT + 2)))
        : outline_(cell_block_limit),
        clipper_(),
        fillingRule_(FILL_NON_ZERO),
        autoClose_(true),
        startX_(0),
        startY_(0),
        status_(STATUS_INITIAL)
    {
        for (int coverIndex = 0; coverIndex < AA_SCALE; coverIndex++) {
            gammar_[coverIndex] = coverIndex;
        }
    }

    template <typename GammaF>
    GraphicRasterizerScanlineAntialias(const GammaF& gamma_function, unsigned cell_block_limit)
        : outline_(cell_block_limit),
        clipper_(outline_),
        fillingRule_(FILL_NON_ZERO),
        autoClose_(true),
        startX_(0),
        startY_(0),
        status_(STATUS_INITIAL)
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
     * @brief Set fill rule, Set fill rule，FILL_NON_ZERO
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
    void FillingRule(FillingRuleEnum fillingRule);
    void AutoClose(bool flag)
    {
        autoClose_ = flag;
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
        for (int coverIndex = 0; coverIndex < AA_SCALE; coverIndex++) {
            gammar_[coverIndex] = MATH_ROUND32(gamma_function(float(coverIndex) / AA_MASK) * AA_MASK);
        }
    }

    unsigned ApplyGammaFunction(unsigned cover) const
    {
        return gammar_[cover];
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
    void AddPath(VertexSource& vs, unsigned pathId = 0)
    {
        float x;
        float y;

        unsigned cmd;
        vs.Rewind(pathId);
        if (outline_.GetSorted()) {
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
        return outline_.MinX();
    }
    int MinY() const
    {
        return outline_.Miny();
    }
    int MaxX() const
    {
        return outline_.MaxX();
    }
    int MaxY() const
    {
        return outline_.MaxY();
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
    GraphicRasterizerScanlineAntialias(const GraphicRasterizerScanlineAntialias<Clip>&);
    const GraphicRasterizerScanlineAntialias<Clip>& operator=(const GraphicRasterizerScanlineAntialias<Clip>&);

    RasterizerCellsAntiAlias<CellBuildAntiAlias> outline_;
    ClipType clipper_;
    int gammar_[AA_SCALE];
    FillingRuleEnum fillingRule_;
    bool autoClose_;
    int32_t startX_;
    int32_t startY_;
    unsigned status_;
    int scanY_;
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
bool GraphicRasterizerScanlineAntialias<Clip>::SweepScanline(Scanline& sl)
{
    while (1) {
        if (scanY_ > outline_.MaxY()) {
            return false;
        }
        sl.ResetSpans();
        unsigned numCells = outline_.ScanlineNumCells(scanY_);
        const CellBuildAntiAlias * const *cells = outline_.ScanlineCells(scanY_);
        int cover = 0;

        while (numCells) {
            const CellBuildAntiAlias* curCell = *cells;
            int x = curCell->x;
            int area = curCell->area;
            unsigned alpha;

            cover += curCell->cover;
            // accumulate all cells with the same X
            while (--numCells && (curCell = *++cells) && (curCell->x == x)) {
                area += curCell->area;
                cover += curCell->cover;
            }
            if (area) {
                // Span interval from area to  (cover << (POLY_SUBPIXEL_SHIFT + 1))
                // Cover can be understood as a delta mask with an area of 1
                SWEEP_SCANLINE_CALCULATE_ALPHA;
            }
            if (numCells && curCell->x > x) {
                // At this time, area is 0, that is, 0 to cover << (POLY_SUBPIXEL_SHIFT + 1)
                SWEEP_SCANLINE_CALCULATE_ALPHA_SPAN
            }
        }

        if (sl.NumSpans()) {
            break;
        }
        ++scanY_;
    }

    sl.Finalize(scanY_);
    ++scanY_;
    return true;
}

/**
 * @brief Convert area cover to gamma cover value to calculate alpha.
 * @since 1.0
 * @version 1.0
 */
template <class Clip>
unsigned GraphicRasterizerScanlineAntialias<Clip>::CalculateAlpha(int area) const
{
    int cover = area >> (POLY_SUBPIXEL_SHIFT * 2 + 1 - AA_SHIFT);

    if (cover < 0) {
        cover = -cover;
    }
    if (fillingRule_ == FILL_EVEN_ODD) {
        cover &= AA_MASK2;
        if (cover > AA_SCALE) {
            cover = AA_SCALE2 - cover;
        }
    }
    if (cover > AA_MASK) {
        cover = AA_MASK;
    }
    return gammar_[cover];
}

template <class Clip>
void GraphicRasterizerScanlineAntialias<Clip>::Reset()
{
    outline_.Reset();
    status_ = STATUS_INITIAL;
}

template <class Clip>
void GraphicRasterizerScanlineAntialias<Clip>::FillingRule(FillingRuleEnum fillingRule)
{
    fillingRule_ = fillingRule;
}

template <class Clip>
void GraphicRasterizerScanlineAntialias<Clip>::ClipBox(float x1, float y1,
                                                       float x2, float y2)
{
    Reset();
    clipper_.ClipBox(DepictType::UpScale(x1), DepictType::UpScale(y1),
                     DepictType::UpScale(x2), DepictType::UpScale(y2));
}

template <class Clip>
void GraphicRasterizerScanlineAntialias<Clip>::ResetClipping()
{
    Reset();
    clipper_.reset_clipping();
}

template <class Clip>
void GraphicRasterizerScanlineAntialias<Clip>::ClosePolygon()
{
    if (status_ == STATUS_LINE_TO) {
        clipper_.LineTo(outline_, startX_, startY_);
        status_ = STATUS_CLOSED;
    }
}

template <class Clip>
void GraphicRasterizerScanlineAntialias<Clip>::MoveTo(int x, int y)
{
    if (outline_.GetSorted()) {
        Reset();
    }
    if (autoClose_) {
        ClosePolygon();
    }
    clipper_.move_to(startX_ = DepictType::downscale(x),
                     startY_ = DepictType::downscale(y));
    status_ = STATUS_MOVE_TO;
}

template <class Clip>
void GraphicRasterizerScanlineAntialias<Clip>::LineTo(int x, int y)
{
    clipper_.line_to(outline_, DepictType::downscale(x), DepictType::downscale(y));
    status_ = STATUS_LINE_TO;
}

template <class Clip>
void GraphicRasterizerScanlineAntialias<Clip>::MoveToByfloat(float x, float y)
{
    if (outline_.GetSorted()) {
        Reset();
    }
    if (autoClose_) {
        ClosePolygon();
    }
    clipper_.MoveTo(startX_ = DepictType::UpScale(x),
                    startY_ = DepictType::UpScale(y));
    status_ = STATUS_MOVE_TO;
}

template <class Clip>
void GraphicRasterizerScanlineAntialias<Clip>::LineToByfloat(float x, float y)
{
    clipper_.LineTo(outline_, DepictType::UpScale(x), DepictType::UpScale(y));
    status_ = STATUS_LINE_TO;
}

template <class Clip>
void GraphicRasterizerScanlineAntialias<Clip>::AddVertex(float x, float y, unsigned cmd)
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
void GraphicRasterizerScanlineAntialias<Clip>::EdgeMake(int x1, int y1, int x2, int y2)
{
    if (outline_.GetSorted()) {
        Reset();
    }
    clipper_.move_to(DepictType::downscale(x1), DepictType::downscale(y1));
    clipper_.line_to(outline_,
                     DepictType::downscale(x2),
                     DepictType::downscale(y2));
    status_ = STATUS_MOVE_TO;
}

template <class Clip>
void GraphicRasterizerScanlineAntialias<Clip>::EdgeMakeUsingfloat(float x1, float y1, float x2, float y2)
{
    if (outline_.GetSorted()) {
        Reset();
    }
    clipper_.move_to(DepictType::UpScale(x1), DepictType::UpScale(y1));
    clipper_.line_to(outline_,
                     DepictType::UpScale(x2),
                     DepictType::UpScale(y2));
    status_ = STATUS_MOVE_TO;
}

template <class Clip>
void GraphicRasterizerScanlineAntialias<Clip>::Sort()
{
    if (autoClose_) {
        ClosePolygon();
    }
    outline_.SortAllCells();
}

template <class Clip>
inline bool GraphicRasterizerScanlineAntialias<Clip>::RewindScanlines()
{
    if (autoClose_) {
        ClosePolygon();
    }
    outline_.SortAllCells();
    if (outline_.GetTotalCells() == 0) {
        return false;
    }
    scanY_ = outline_.Miny();
    return true;
}

template <class Clip>
bool GraphicRasterizerScanlineAntialias<Clip>::NavigateScanline(int y)
{
    if (autoClose_) {
        ClosePolygon();
    }
    outline_.SortAllCells();
    if (outline_.GetTotalCells() == 0 ||
        y < outline_.Miny() ||
        y > outline_.MaxY()) {
        return false;
    }
    scanY_ = y;
    return true;
}

template <class Clip>
bool GraphicRasterizerScanlineAntialias<Clip>::HitTest(int tx, int ty)
{
    if (!NavigateScanline(ty)) {
        return false;
    }
    ScanlineHitRegionMeasure sl(tx);
    SweepScanline(sl);
    return sl.GetHitMeasure();
}
} // namespace OHOS
#endif
