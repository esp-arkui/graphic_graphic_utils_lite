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
 * @file graphic_rasterizer_scanline_clip.h
 * @brief Defines 光栅栏剪裁
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_RASTERIZER_SL_CLIP_INCLUDED
#define GRAPHIC_RASTERIZER_SL_CLIP_INCLUDED

#include "gfx_utils/graphics/common/graphic_common_clip_operate.h"
#include "gfx_utils/graphic_math.h"
namespace OHOS {
    /**
     * 该PolyMaxCoordEnum枚举类型
     * 定义了多边形最大坐标的值.
     * @since 1.0
     * @version 1.0
     */
    enum PolyMaxCoordEnum {
        POLY_MAX_COORD = (1 << 30) - 1
    };

    /**
     * @struct RasterDepictInt
     * @brief 该RasterDepictInt结构体在对于目标范围内的
     * 坐标进行裁剪的时候，对于类似坐标间距的3个值或者
     * 上采样以及下采样等操作做处理。
     * @since 1.0
     * @version 1.0
     */
    struct RasterDepictInt {
        typedef int coord_type;

        /**
         * @brief 该函数主要是对于输入的坐标值
         * 做上采样的处理。
         * @since 1.0
         * @version 1.0
         */
        static int UpScale(double vUpscale)
        {
            return MATH_ROUND32(vUpscale * POLY_SUBPIXEL_SCALE);
        }

        /**
         * @brief 该函数主要是对于输入的坐标值
         * 做下采样的处理。
         * @since 1.0
         * @version 1.0
         */
        static int DownScale(int vDownscale)
        {
            return vDownscale;
        }

        static GRAPHIC_GEOMETRY_INLINE int MultDiv(double deltaA, double deltaB, double dealtaC)
        {
            return MATH_ROUND32(deltaA * deltaB / dealtaC);
        }
        static int GetXCoordinateValue(int xValue)
        {
            return xValue;
        }
        static int GetYCoordinateValue(int yValue)
        {
            return yValue;
        }
    };

    /**
     * @template<Depict> class RasterizerScanlineClip
     * @brief Defines 光栅化阶段，交换扫描线处理时，对于
     * 坐标的裁剪和加工的处理过程.
     * @since 1.0
     * @version 1.0
     */
    template <class Depict>
    class RasterizerScanlineClip {
    public:
        using depict_type = Depict;
        using coord_type = typename Depict::coord_type;
        using rect_type = RectBase<coord_type>;
        /**
         * @brief RasterizerScanlineClip 类的构造函数。.
         * 初始化裁剪范围，裁剪标志等。
         * @since 1.0
         * @version 1.0
         */
        RasterizerScanlineClip()
            : m_clip_box(0, 0, 0, 0),
            m_x1(0),
            m_y1(0),
            m_clipping_flags(0),
            m_clipping(false)
        {}

        void ResetClipping()
        {
            m_clipping = false;
        }

        /**
         * @brief 设置裁剪范围。
         * @since 1.0
         * @version 1.0
         */
        void ClipBox(coord_type x1, coord_type y1, coord_type x2, coord_type y2)
        {
            m_clip_box = rect_type(x1, y1, x2, y2);
            m_clip_box.Normalize();
            m_clipping = true;
        }

        /**
         * @brief 在RASTERIZER 过程中，增加设置起始点，并且设置
         * m_clipping_flags的标志。
         * @since 1.0
         * @version 1.0
         */
        void MoveTo(coord_type x1, coord_type y1)
        {
            m_x1 = x1;
            m_y1 = y1;
            if (m_clipping) {
                m_clipping_flags = ClippingFlags(x1, y1, m_clip_box);
            }
        }
        /**
         * @brief 在RASTERIZER 过程中，增加设置采样点，并且设置
         * 采样点设置相关的cover与area的属性等。
         *         |        |
         *   0110  |  0010  | 0011
         *         |        |
         *  -------+--------+-------- clip_box.y2
         *         |        |
         *   0100  |  0000  | 0001
         *         |        |
         *  -------+--------+-------- clip_box.y1
         *         |        |
         *   1100  |  1000  | 1001
         *         |        |
         *   clip_box.x1  clip_box.x2
         * @since 1.0
         * @version 1.0
         */
        template <class Rasterizer>
        void LineTo(Rasterizer& ras, coord_type x2, coord_type y2);

    private:
        /**
         * @brief 在RASTERIZER 过程中,根据上次的裁剪范围判断标志
         * 以及本次的裁剪范围判断标志，进行实际的采样点的添加以及
         * 相关的属性设置等。
         * @since 1.0
         * @version 1.0
         */
        template <class Rasterizer>
        GRAPHIC_GEOMETRY_INLINE void LineClipY(Rasterizer& ras,
                                               coord_type x1, coord_type y1,
                                               coord_type x2, coord_type y2,
                                               unsigned clipFlagsOne, unsigned clipFlagsTwo) const;
        rect_type m_clip_box;
        coord_type m_x1;
        coord_type m_y1;
        unsigned m_clipping_flags;
        bool m_clipping;
    };

    class RasterizerScanlineNoClip {
    public:
        typedef RasterDepictInt conv_type;
        typedef int coord_type;

        RasterizerScanlineNoClip() : m_x1(0), m_y1(0)
        {}

        void ResetClipping()
        {}
        void ClipBox(coord_type, coord_type, coord_type, coord_type)
        {}
        void MoveTo(coord_type x1, coord_type y1)
        {
            m_x1 = x1;
            m_y1 = y1;
        }

        template <class Rasterizer>
        void LineTo(Rasterizer& ras, coord_type x2, coord_type y2)
        {
            ras.line(m_x1, m_y1, x2, y2);
            m_x1 = x2;
            m_y1 = y2;
        }

    private:
        int m_x1, m_y1;
    };

    /**
     * @brief 在RASTERIZER 过程中,根据上次的裁剪范围判断标志
     * 以及本次的裁剪范围判断标志，进行实际的采样点的添加以及
     * 相关的属性设置等。
     * @since 1.0
     * @version 1.0
     */
    template <class Depict>
    template <class Rasterizer>
    void RasterizerScanlineClip<Depict>::LineClipY(
            Rasterizer& ras, coord_type x1, coord_type y1, coord_type x2,
            coord_type y2, unsigned clipFlagsOne, unsigned clipFlagsTwo) const
    {
        clipFlagsOne &= DIRECTLY_BELOW;
        clipFlagsTwo &= DIRECTLY_BELOW;
        if ((clipFlagsOne | clipFlagsTwo) == 0) {
            /**
             * 表明坐标x1,y1,x2,y2 全部在范围内,line 操作之
             */
            ras.LineOperate(RasterDepictInt::GetXCoordinateValue(x1),
                            RasterDepictInt::GetYCoordinateValue(y1),
                            RasterDepictInt::GetXCoordinateValue(x2),
                            RasterDepictInt::GetYCoordinateValue(y2));
        } else {
            if (clipFlagsOne == clipFlagsTwo) {
                /**
                 * 表明坐标x1,y1,x2,y2 全部在范围外,不操作
                 */
                return;
            }
            coord_type tx1 = x1;
            coord_type ty1 = y1;
            coord_type tx2 = x2;
            coord_type ty2 = y2;
            /**
             * 表明坐标y1 < clip.y1
             */
            if (clipFlagsOne & 0x08) {
                tx1 = x1 + Depict::MultDiv(m_clip_box.y1 - y1, x2 - x1, y2 - y1);
                ty1 = m_clip_box.y1;
            }

            /**
             * 表明坐标y1 > clip.y2
             */
            if (clipFlagsOne & 0x02) {
                tx1 = x1 + Depict::MultDiv(m_clip_box.y2 - y1, x2 - x1, y2 - y1);
                ty1 = m_clip_box.y2;
            }
            /**
             * 表明坐标y1 > clip.y2
             */
            if (clipFlagsTwo & 0x08) {
                tx2 = x1 + Depict::MultDiv(m_clip_box.y1 - y1, x2 - x1, y2 - y1);
                ty2 = m_clip_box.y1;
            }
            /**
             * 表明坐标y2 > clip.y2
             */
            if (clipFlagsTwo & 0x02) {
                tx2 = x1 + Depict::MultDiv(m_clip_box.y2 - y1, x2 - x1, y2 - y1);
                ty2 = m_clip_box.y2;
            }
            ras.LineOperate(RasterDepictInt::GetXCoordinateValue(tx1), RasterDepictInt::GetYCoordinateValue(ty1),
                            RasterDepictInt::GetXCoordinateValue(tx2), RasterDepictInt::GetYCoordinateValue(ty2));
        }
    }

    /**
     * @brief 在RASTERIZER 过程中，增加设置采样点，并且设置
     * 采样点设置相关的cover与area的属性等。
     *         |        |
     *   0110  |  0010  | 0011
     *         |        |
     *  -------+--------+-------- clip_box.y2
     *         |        |
     *   0100  |  0000  | 0001
     *         |        |
     *  -------+--------+-------- clip_box.y1
     *         |        |
     *   1100  |  1000  | 1001
     *         |        |
     *   clip_box.x1  clip_box.x2
     * @since 1.0
     * @version 1.0
     */
    template <class Depict>
    template <class Rasterizer>
    void RasterizerScanlineClip<Depict>::LineTo(Rasterizer& rasterLine, coord_type x2, coord_type y2)
    {
        if (m_clipping) {
            unsigned cFlagsLineToPoint = ClippingFlags(x2, y2, m_clip_box);
            if ((m_clipping_flags & 0x0A) == (cFlagsLineToPoint & 0x0A) && (m_clipping_flags & 0x0A) != 0) {
                /**
                 * 表明moveto与lineto重合
                 */
                m_x1 = x2;
                m_y1 = y2;
                m_clipping_flags = cFlagsLineToPoint;
                return;
            }

            coord_type x1 = m_x1;
            coord_type y1 = m_y1;
            unsigned clipFlagsMoveToPoint = m_clipping_flags;
            coord_type yPilotOne, yPilotTwo;
            unsigned yClipFlagsOne, yClipFlagsTwo;

            switch (((clipFlagsMoveToPoint & 0x05) << 1) | (cFlagsLineToPoint & 0x05)) {
                /**
                 * 表明 x1, y1, x2, y2 全在clip区域内
                 */
                case 0x00:
                    LineClipY(rasterLine, x1, y1, x2, y2, clipFlagsMoveToPoint, cFlagsLineToPoint);
                    break;
                /**
                 * 表明 x2 > clip.x2
                 */
                case 0x01:
                    yPilotOne = y1 + Depict::MultDiv(m_clip_box.x2 - x1, y2 - y1, x2 - x1);
                    yClipFlagsOne = ClippingFlagsY(yPilotOne, m_clip_box);
                    LineClipY(rasterLine, x1, y1, m_clip_box.x2, yPilotOne,
                              clipFlagsMoveToPoint, yClipFlagsOne);
                    LineClipY(rasterLine, m_clip_box.x2, yPilotOne,
                              m_clip_box.x2, y2, yClipFlagsOne, cFlagsLineToPoint);
                    break;
                /**
                 * 表明 x1 > clip.x2
                 */
                case 0x02:
                    yPilotOne = y1 + Depict::MultDiv(m_clip_box.x2 - x1, y2 - y1, x2 - x1);
                    yClipFlagsOne = ClippingFlagsY(yPilotOne, m_clip_box);
                    LineClipY(rasterLine, m_clip_box.x2, y1, m_clip_box.x2,
                              yPilotOne, clipFlagsMoveToPoint, yClipFlagsOne);
                    LineClipY(rasterLine, m_clip_box.x2, yPilotOne, x2, y2,
                              yClipFlagsOne, cFlagsLineToPoint);
                    break;
                /**
                 * 表明 x1 > clip.x2 && x2 > clip.x2
                 */
                case 0x03:
                    LineClipY(rasterLine, m_clip_box.x2, y1, m_clip_box.x2, y2,
                              clipFlagsMoveToPoint, cFlagsLineToPoint);
                    break;
                /**
                 * 表明 x2 < clip.x1
                 */
                case 0x04:
                    yPilotOne = y1 + Depict::MultDiv(m_clip_box.x1 - x1, y2 - y1, x2 - x1);
                    yClipFlagsOne = ClippingFlagsY(yPilotOne, m_clip_box);
                    LineClipY(rasterLine, x1, y1, m_clip_box.x1, yPilotOne,
                              clipFlagsMoveToPoint, yClipFlagsOne);
                    LineClipY(rasterLine, m_clip_box.x1, yPilotOne, m_clip_box.x1, y2,
                              yClipFlagsOne, cFlagsLineToPoint);
                    break;
                /**
                 * 表明 x1 > clip.x2 && x2 < clip.x1
                 */
                case 0x06:
                    yPilotOne = y1 + Depict::MultDiv(m_clip_box.x2 - x1, y2 - y1, x2 - x1);
                    yPilotTwo = y1 + Depict::MultDiv(m_clip_box.x1 - x1, y2 - y1, x2 - x1);
                    yClipFlagsOne = ClippingFlagsY(yPilotOne, m_clip_box);
                    yClipFlagsTwo = ClippingFlagsY(yPilotTwo, m_clip_box);
                    LineClipY(rasterLine, m_clip_box.x2, y1, m_clip_box.x2, yPilotOne,
                              clipFlagsMoveToPoint, yClipFlagsOne);
                    LineClipY(rasterLine, m_clip_box.x2, yPilotOne, m_clip_box.x1, yPilotTwo,
                              yClipFlagsOne, yClipFlagsTwo);
                    LineClipY(rasterLine, m_clip_box.x1, yPilotTwo, m_clip_box.x1, y2,
                              yClipFlagsTwo, cFlagsLineToPoint);
                    break;

                /**
                 * 表明 x1 < clip.x1
                 */
                case 0x08:
                    yPilotOne = y1 + Depict::MultDiv(m_clip_box.x1 - x1, y2 - y1, x2 - x1);
                    yClipFlagsOne = ClippingFlagsY(yPilotOne, m_clip_box);
                    LineClipY(rasterLine, m_clip_box.x1, y1, m_clip_box.x1, yPilotOne,
                              clipFlagsMoveToPoint, yClipFlagsOne);
                    LineClipY(rasterLine, m_clip_box.x1, yPilotOne, x2, y2,
                              yClipFlagsOne, cFlagsLineToPoint);
                    break;
                /**
                 * 表明 x1 < clip.x1 && x2 > clip.x2
                 */
                case 0x09:
                    yPilotOne = y1 + Depict::MultDiv(m_clip_box.x1 - x1, y2 - y1, x2 - x1);
                    yPilotTwo = y1 + Depict::MultDiv(m_clip_box.x2 - x1, y2 - y1, x2 - x1);
                    yClipFlagsOne = ClippingFlagsY(yPilotOne, m_clip_box);
                    yClipFlagsTwo = ClippingFlagsY(yPilotTwo, m_clip_box);
                    LineClipY(rasterLine, m_clip_box.x1, y1, m_clip_box.x1, yPilotOne,
                              clipFlagsMoveToPoint, yClipFlagsOne);
                    LineClipY(rasterLine, m_clip_box.x1, yPilotOne, m_clip_box.x2, yPilotTwo,
                              yClipFlagsOne, yClipFlagsTwo);
                    LineClipY(rasterLine, m_clip_box.x2, yPilotTwo, m_clip_box.x2, y2,
                              yClipFlagsTwo, cFlagsLineToPoint);
                    break;
                /**
                 * 表明 x1 < clip.x1 && x2 < clip.x1
                 */
                case 0x0c:
                    LineClipY(rasterLine, m_clip_box.x1, y1, m_clip_box.x1, y2,
                              clipFlagsMoveToPoint, cFlagsLineToPoint);
                    break;
            }
            m_clipping_flags = cFlagsLineToPoint;
        } else {
            rasterLine.LineOperate(RasterDepictInt::GetXCoordinateValue(m_x1),
                                   RasterDepictInt::GetYCoordinateValue(m_y1),
                                   RasterDepictInt::GetXCoordinateValue(x2),
                                   RasterDepictInt::GetYCoordinateValue(y2));
        }
        m_x1 = x2;
        m_y1 = y2;
    }
} // namespace OHOS
#endif
