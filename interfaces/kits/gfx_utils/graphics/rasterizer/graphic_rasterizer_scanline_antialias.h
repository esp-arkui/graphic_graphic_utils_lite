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
* @brief Defines 光栅化以及扫描线过程阶段处理.
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
    * @brief 多边形光栅化用于高质量的填充多边形的渲染,
    * 这个类的int坐标包括了24.8的格式，24位用于坐标int部分,
    * 8位用于子像素的shift偏移部分 ，再用move_to(x, y) / line_to(x, y)
    * 构建多边形时，构建扫描线光栅化处理.
    * @since 1.0
    * @version 1.0
    */
    template <class Clip = RasterizerScanlineClipInt>
    class RasterizerScanlineAntiAlias {
        /**
        * @brief 构建扫描线状态枚举
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
        * 构建光栅化扫描线抗锯齿构造函数
        * @brief 主要包括细胞单元块的分配额度，裁剪器
        * 填充规则，自动闭合，起始位置等。
        * @since 1.0
        * @version 1.0
        */
        RasterizerScanlineAntiAlias(unsigned cell_block_limit = (1 << (AA_SHIFT + 2))) :
            m_outline(cell_block_limit),
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
        RasterizerScanlineAntiAlias(const GammaF& gamma_function, unsigned cell_block_limit) :
            m_outline(cell_block_limit),
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
        * @brief 重新设置构建轮廓线单元细胞数组，
        * 重新设置扫描线状态值等。
        * @since 1.0
        * @version 1.0
        */
        void Reset();

        /**
        * @brief 重新设置裁剪器的裁剪范围以及裁剪标志。
        * @since 1.0
        * @version 1.0
        */
        void ResetClipping();
        void ClipBox(double x1, double y1, double x2, double y2);

        /**
        * @brief 设置填充规则，FILL_NON_ZERO（是
        * 假设是用来判断哪些区域属于路径内( 计算结果非0，即为路径内 )。
        * 在路径包围的区域中，随便找一点，向外发射一条射线，
        * 和所有围绕它的边相交，
        * 然后开启一个计数器，从0计数，
        * 如果这个射线遇到顺时针围绕，那么+1，
        * 如果遇到逆时针围绕，那么-1，
        * 如果最终值非0，则这块区域在路径内）,
        * FILL_EVEN_ODD(* 在路径包围的区域中，随便找一点，向外发射一条射线，
        * 和所有围绕它的边相交，
        * 查看相交线的个数，如果为奇数，就填充，如果是偶数，就不填充。)
        * @since 1.0
        * @version 1.0
        */
        void FillingRule(FillingRuleEnum filling_rule);
        void AutoClose(bool flag)
        {
            m_auto_close = flag;
        }

        /**
        * @brief 设置Gamma函数为了计算alpha值，抗锯齿服务的
        * Gamma函数的索引是覆盖率，每个覆盖率都按照Gamma函数比如
        * gamma_power：使用每个覆盖率*设置值的平方，作为颜色值，可实现抗锯齿值的整体放大或缩小。
        * gamma_linear ：设置起止范围，对颜色值进行线性变换，小于起点则为0，大于起点则为1，
        * 范围内则是(cover - m_start) / (m_end - m_start)。
        * gamma_threshold：设置阈值，小于阈值为0，大于则为1. 设置阈值后，将无抗锯齿效果。
        * gamma_multiply：将颜色值乘以设置的倍数。。
        * @since 1.0
        * @version 1.0
        */
        template <typename GammaF>
        void GammaFunction(const GammaF& gamma_function)
        {
            int coverIndex;
            for (coverIndex = 0; coverIndex < AA_SCALE; coverIndex++) {
                m_gamma[coverIndex] = Uround(gamma_function(double(coverIndex) / AA_MASK) * AA_MASK);
            }
        }

        unsigned ApplyGammaFunction(unsigned cover) const
        {
            return m_gamma[cover];
        }

        /**
        * @brief 按照1/256 像素单元的设置图元的起点位置。
        * @since 1.0
        * @version 1.0
        */
        void MoveTo(int x, int y);
        /**
        * @brief 按照1/256 像素单元的设置图元的移动位置。
        * @since 1.0
        * @version 1.0
        */
        void LineTo(int x, int y);
        /**
        * @brief 按照1像素单元的设置图元的起点位置。
        * @since 1.0
        * @version 1.0
        */
        void MoveToByDouble(double x, double y);
        /**
        * @brief 按照像素单元的设置图元的移动位置。
        * @since 1.0
        * @version 1.0
        */
        void LineToByDouble(double x, double y);

        /**
        * @brief 闭合多边形处理。
        * @since 1.0
        * @version 1.0
        */
        void ClosePolygon();
        void AddVertex(double x, double y, unsigned cmd);

        /**
        * @brief 利用裁剪器按照1/256像素单元裁剪出一条边且构建轮廓线。
        * @since 1.0
        * @version 1.0
        */
        void EdgeMake(int x1, int y1, int x2, int y2);
        /**
        * @brief 利用裁剪器按照1像素单元裁剪出一条边且构建轮廓线。
        * @since 1.0
        * @version 1.0
        */
        void EdgeMakeUsingDouble(double x1, double y1, double x2, double y2);

        /**
        * @brief 从顶点源中获取顶点信息坐标，且按照扫描过程
        * 设置添加cells单元数组的过程。
        * @since 1.0
        * @version 1.0
        */
        template <typename VertexSource>
        void AddPath(VertexSource& vs, unsigned path_id = 0)
        {
            double x;
            double y;

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
        * @brief 轮廓线的范围边界值。
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
        * @brief 对于轮廓线中的cells单元数组按照从左向右，自上而下排序。
        * @since 1.0
        * @version 1.0
        */
        void Sort();
        bool RewindScanlines();

        /**
        * @brief 按照某y值高度导航到某条扫描线起始位置。
        * @since 1.0
        * @version 1.0
        */
        bool NavigateScanline(int y);

        /**
        * @brief 将area cover转为gamma cover值计算alpha。
        * @since 1.0
        * @version 1.0
        */
        unsigned CalculateAlpha(int area) const;
        /**
        * @brief 从Rasterizer阶段获取到某y值的扫描线
        * 且迭代当前扫描线的cells单元数组，从中获得area->cover，
        * 利用2者计算delta area 作为area cover 转换成gamma cover
        * 成功得到颜色信息，然后利用gamma函数推算出颜色的alpha信息
        * 填充到新的scanline中，拥有后续的render。
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

    private:
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
    * @brief 从Rasterizer阶段获取到某y值的扫描线
    * 且迭代当前扫描线的cells单元数组，从中获得area->cover，
    * 利用2者计算delta area 作为area cover 转换成gamma cover
    * 成功得到颜色信息，然后利用gamma函数推算出颜色的alpha信息
    * 填充到新的scanline中，拥有后续的render。
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
            const CellBuildAntiAlias* const* cells = m_outline.ScanlineCells(m_scan_y);
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
                    // 由 area 到  (cover << (POLY_SUBPIXEL_SHIFT + 1)) 的跨度间隔
                    // cover 可以理解为是 area 为 1的delta mask
                    alpha = CalculateAlpha((cover << (POLY_SUBPIXEL_SHIFT + 1)) - area);
                    if (alpha) {
                        sl.AddCell(x, alpha);
                    }
                    x++;
                }

                if (num_cells && cur_cell->x > x) {
                    // 此时 area为0 ，也就是说是 0 到 cover << (POLY_SUBPIXEL_SHIFT + 1)
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
    * @brief 将area cover转为gamma cover值计算alpha。
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
    void RasterizerScanlineAntiAlias<Clip>::ClipBox(double x1, double y1,
                                                    double x2, double y2)
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
    void RasterizerScanlineAntiAlias<Clip>::MoveToByDouble(double x, double y)
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
    void RasterizerScanlineAntiAlias<Clip>::LineToByDouble(double x, double y)
    {
        m_clipper.LineTo(m_outline,
                         depict_type::UpScale(x),
                         depict_type::UpScale(y));
        m_status = STATUS_LINE_TO;
    }

    template <class Clip>
    void RasterizerScanlineAntiAlias<Clip>::AddVertex(double x, double y, unsigned cmd)
    {
        if (IsMoveTo(cmd)) {
            MoveToByDouble(x, y);
        } else if (IsVertex(cmd)) {
            LineToByDouble(x, y);
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
    void RasterizerScanlineAntiAlias<Clip>::EdgeMakeUsingDouble(double x1, double y1,
                                                                double x2, double y2)
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
