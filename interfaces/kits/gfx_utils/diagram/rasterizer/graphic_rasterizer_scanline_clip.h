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
 * @brief Defines Light fence clipping
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_RASTERIZER_SL_CLIP_INCLUDED
#define GRAPHIC_RASTERIZER_SL_CLIP_INCLUDED

#include "gfx_utils/diagram/common/graphic_common_clip_operate.h"
#include "gfx_utils/graphic_math.h"
namespace OHOS {
#define RASTER_SCANLINE_CLIP_LINETO_0x01 \
    yPilotOne = y1 + Depict::MultDiv(clipBox_.x2 - x1, y2 - y1, x2 - x1); \
    yClipFlagsOne = ClippingFlagsY(yPilotOne, clipBox_); \
    LineClipY(rasterLine, x1, y1, clipBox_.x2, yPilotOne, \
              clipFlagsMoveToPoint, yClipFlagsOne); \
    LineClipY(rasterLine, clipBox_.x2, yPilotOne, \
              clipBox_.x2, y2, yClipFlagsOne, cFlagsLineToPoint)

#define RASTER_SCANLINE_CLIP_LINETO_0x02 \
    yPilotOne = y1 + Depict::MultDiv(clipBox_.x2 - x1, y2 - y1, x2 - x1); \
    yClipFlagsOne = ClippingFlagsY(yPilotOne, clipBox_); \
    LineClipY(rasterLine, clipBox_.x2, y1, clipBox_.x2, \
              yPilotOne, clipFlagsMoveToPoint, yClipFlagsOne); \
    LineClipY(rasterLine, clipBox_.x2, yPilotOne, x2, y2, \
              yClipFlagsOne, cFlagsLineToPoint)

#define RASTER_SCANLINE_CLIP_LINETO_0x04 \
    yPilotOne = y1 + Depict::MultDiv(clipBox_.x1 - x1, y2 - y1, x2 - x1); \
    yClipFlagsOne = ClippingFlagsY(yPilotOne, clipBox_); \
    LineClipY(rasterLine, x1, y1, clipBox_.x1, yPilotOne, \
              clipFlagsMoveToPoint, yClipFlagsOne); \
    LineClipY(rasterLine, clipBox_.x1, yPilotOne, clipBox_.x1, y2, \
              yClipFlagsOne, cFlagsLineToPoint)

#define RASTER_SCANLINE_CLIP_LINETO_0x06 \
    yPilotOne = y1 + Depict::MultDiv(clipBox_.x2 - x1, y2 - y1, x2 - x1); \
    yPilotTwo = y1 + Depict::MultDiv(clipBox_.x1 - x1, y2 - y1, x2 - x1); \
    yClipFlagsOne = ClippingFlagsY(yPilotOne, clipBox_); \
    yClipFlagsTwo = ClippingFlagsY(yPilotTwo, clipBox_); \
    LineClipY(rasterLine, clipBox_.x2, y1, clipBox_.x2, yPilotOne, \
              clipFlagsMoveToPoint, yClipFlagsOne); \
    LineClipY(rasterLine, clipBox_.x2, yPilotOne, clipBox_.x1, yPilotTwo, \
              yClipFlagsOne, yClipFlagsTwo); \
    LineClipY(rasterLine, clipBox_.x1, yPilotTwo, clipBox_.x1, y2, \
              yClipFlagsTwo, cFlagsLineToPoint)

#define RASTER_SCANLINE_CLIP_LINETO_0x08 \
    yPilotOne = y1 + Depict::MultDiv(clipBox_.x1 - x1, y2 - y1, x2 - x1); \
    yClipFlagsOne = ClippingFlagsY(yPilotOne, clipBox_); \
    LineClipY(rasterLine, clipBox_.x1, y1, clipBox_.x1, yPilotOne, \
              clipFlagsMoveToPoint, yClipFlagsOne); \
    LineClipY(rasterLine, clipBox_.x1, yPilotOne, x2, y2, \
              yClipFlagsOne, cFlagsLineToPoint)

#define RASTER_SCANLINE_CLIP_LINETO_0x09 \
    yPilotOne = y1 + Depict::MultDiv(clipBox_.x1 - x1, y2 - y1, x2 - x1); \
    yPilotTwo = y1 + Depict::MultDiv(clipBox_.x2 - x1, y2 - y1, x2 - x1); \
    yClipFlagsOne = ClippingFlagsY(yPilotOne, clipBox_); \
    yClipFlagsTwo = ClippingFlagsY(yPilotTwo, clipBox_); \
    LineClipY(rasterLine, clipBox_.x1, y1, clipBox_.x1, yPilotOne, \
              clipFlagsMoveToPoint, yClipFlagsOne); \
    LineClipY(rasterLine, clipBox_.x1, yPilotOne, clipBox_.x2, yPilotTwo, \
              yClipFlagsOne, yClipFlagsTwo); \
    LineClipY(rasterLine, clipBox_.x2, yPilotTwo, clipBox_.x2, y2, \
              yClipFlagsTwo, cFlagsLineToPoint)

#define LINE_OPERATE \
    rasterLine.LineOperate(RasterDepictInt::GetXCoordinateValue(x1_), \
                           RasterDepictInt::GetYCoordinateValue(y1_), \
                           RasterDepictInt::GetXCoordinateValue(x2), \
                           RasterDepictInt::GetYCoordinateValue(y2))
#define LINE_TO \
    int32_t x1 = x1_; \
    int32_t y1 = y1_; \
    unsigned clipFlagsMoveToPoint = clippingFlags_; \
    int32_t yPilotOne; \
    int32_t yPilotTwo; \
    unsigned yClipFlagsOne = 0; \
    unsigned yClipFlagsTwo = 0

/**
 * The PolyMaxCoordEnum enumeration type
 * Defines the value of the maximum coordinate of the polygon
 * @since 1.0
 * @version 1.0
 */
enum PolyMaxCoordEnum {
    POLY_MAX_COORD = (1 << 30) - 1
};

/**
 * @struct RasterDepictInt
 * @brief The RasterDepictInt structure is within the target range
 * When clipping coordinates, for 3 values similar to coordinate spacing, or
 * Upper sampling and lower sampling are processed.
 * @since 1.0
 * @version 1.0
 */
struct RasterDepictInt {
    /**
     * @brief This function is mainly for the input coordinate values
     * Do sampling processing.
     * @since 1.0
     * @version 1.0
     */
    static int UpScale(float vUpscale)
    {
        return MATH_ROUND32(vUpscale * POLY_SUBPIXEL_SCALE);
    }

    /**
     * @brief This function is mainly for the input coordinate values
     * Do the next sampling.
     * @since 1.0
     * @version 1.0.
     */
    static int DownScale(int vDownscale)
    {
        return vDownscale;
    }

    static inline int MultDiv(float deltaA, float deltaB, float dealtaC)
    {
        if (dealtaC == 0) {
            return 0;
        }
        if (dealtaC != 0.0f) {
            return MATH_ROUND32(deltaA * deltaB / dealtaC);
        }
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
 * @brief Defines In the rasterization stage, when exchanging scan line processing, for
 * Coordinate cutting and processing
 * @since 1.0
 * @version 1.0
 */
template <class Depict>
class RasterizerScanlineClip {
public:
    using DepictType = Depict;
    using RectType = RectBase<int32_t>;
    /**
     * @brief RasterizerScanlineClip Class constructor
     * Initialize clipping range, clipping flag, etc.
     * @since 1.0
     * @version 1.0
     */
    RasterizerScanlineClip()
        : clipBox_(0, 0, 0, 0),
        x1_(0),
        y1_(0),
        clippingFlags_(0),
        clipping_(false)
    {}

    void ResetClipping()
    {
        clipping_ = false;
    }

    /**
     * @brief Sets the clipping range.
     * @since 1.0
     * @version 1.0
     */
    void ClipBox(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
    {
        clipBox_ = RectType(x1, y1, x2, y2);
        clipBox_.Normalize();
        clipping_ = true;
    }

    /**
     * @brief In the RASTERIZER process, the starting point of setting is added,
     * And set the flag of clippingFlags_
     * @since 1.0
     * @version 1.0
     */
    void MoveTo(int32_t x1, int32_t y1)
    {
        x1_ = x1;
        y1_ = y1;
        if (clipping_) {
            clippingFlags_ = ClippingFlags(x1, y1, clipBox_);
        }
    }
    /**
     * @brief In the RASTERIZER process, add the set sampling point
     * And set the sampling point, set the related cover and area attributes, etc
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
    void LineTo(Rasterizer& ras, int32_t x2, int32_t y2);

private:
    /**
     * @brief In the RASTERIZER process,Judge the mark according to the last clipping range
     * And the cutting range judgment flag this time,
     * add the actual sampling points and set relevant attributes
     * @since 1.0
     * @version 1.0
     */
    template <class Rasterizer>
    inline void LineClipY(Rasterizer& ras,
                          int32_t x1, int32_t y1,
                          int32_t x2, int32_t y2,
                          unsigned clipFlagsOne, unsigned clipFlagsTwo) const;
    RectType clipBox_;
    int32_t x1_;
    int32_t y1_;
    unsigned clippingFlags_;
    bool clipping_;
};

class RasterizerScanlineNoClip {
public:
    typedef RasterDepictInt conv_type;
    typedef int int32_t;

    RasterizerScanlineNoClip() : x1_(0), y1_(0)
    {}

    void ResetClipping()
    {}
    void ClipBox(int32_t, int32_t, int32_t, int32_t)
    {}
    void MoveTo(int32_t x1, int32_t y1)
    {
        x1_ = x1;
        y1_ = y1;
    }

    template <class Rasterizer>
    void LineTo(Rasterizer& ras, int32_t x2, int32_t y2)
    {
        ras.line(x1_, y1_, x2, y2);
        x1_ = x2;
        y1_ = y2;
    }

private:
    int x1_, y1_;
};

/**
 * @brief In the RASTERIZER process,Judge the mark according to the last clipping range
 * And the cutting range judgment flag this time,
 * add the actual sampling points and set relevant attributes
 * @since 1.0
 * @version 1.0
 */
template <class Depict>
template <class Rasterizer>
void RasterizerScanlineClip<Depict>::LineClipY(Rasterizer& ras,
                                               int32_t x1, int32_t y1,
                                               int32_t x2, int32_t y2,
                                               unsigned clipFlagsOne, unsigned clipFlagsTwo) const
{
    clipFlagsOne &= DIRECTLY_BELOW;
    clipFlagsTwo &= DIRECTLY_BELOW;
    if ((clipFlagsOne | clipFlagsTwo) == 0) {
        /**
         * It indicates that the coordinates x1, y1, x2 and y2 are all within the range, and the line operates
         */
        ras.LineOperate(RasterDepictInt::GetXCoordinateValue(x1),
                        RasterDepictInt::GetYCoordinateValue(y1),
                        RasterDepictInt::GetXCoordinateValue(x2),
                        RasterDepictInt::GetYCoordinateValue(y2));
    } else {
        if (clipFlagsOne == clipFlagsTwo) {
            /**
             * It indicates that the coordinates x1, y1, x2 and y2 are all outside the range and do not operate
             */
            return;
        }
        int32_t tx1 = x1;
        int32_t ty1 = y1;
        int32_t tx2 = x2;
        int32_t ty2 = y2;
        /**
         * Indicates that the coordinate y1 < clip.y1
         */
        if (clipFlagsOne & 0x08) {
            tx1 = x1 + Depict::MultDiv(clipBox_.y1 - y1, x2 - x1, y2 - y1);
            ty1 = clipBox_.y1;
        }

        /**
         * Indicates that the coordinate y1 > clip.y2
         */
        if (clipFlagsOne & 0x02) {
            tx1 = x1 + Depict::MultDiv(clipBox_.y2 - y1, x2 - x1, y2 - y1);
            ty1 = clipBox_.y2;
        }
        /**
         * Indicates that the coordinate y1 > clip.y2
         */
        if (clipFlagsTwo & 0x08) {
            tx2 = x1 + Depict::MultDiv(clipBox_.y1 - y1, x2 - x1, y2 - y1);
            ty2 = clipBox_.y1;
        }
        /**
         * Indicates that the coordinate y2 > clip.y2
         */
        if (clipFlagsTwo & 0x02) {
            tx2 = x1 + Depict::MultDiv(clipBox_.y2 - y1, x2 - x1, y2 - y1);
            ty2 = clipBox_.y2;
        }
        ras.LineOperate(RasterDepictInt::GetXCoordinateValue(tx1), RasterDepictInt::GetYCoordinateValue(ty1),
                        RasterDepictInt::GetXCoordinateValue(tx2), RasterDepictInt::GetYCoordinateValue(ty2));
    }
}

/**
 * @brief In the RASTERIZER process, add the set sampling point,
 * And set the sampling point, set the related cover and area attributes, etc.
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
void RasterizerScanlineClip<Depict>::LineTo(Rasterizer& rasterLine, int32_t x2, int32_t y2)
{
    if (clipping_) {
        unsigned cFlagsLineToPoint = ClippingFlags(x2, y2, clipBox_);
        if ((clippingFlags_ & 0x0A) == (cFlagsLineToPoint & 0x0A) && (clippingFlags_ & 0x0A) != 0) {
            x1_ = x2;
            y1_ = y2;
            clippingFlags_ = cFlagsLineToPoint;
            return;
        }
        LINE_TO;

        switch (((clipFlagsMoveToPoint & 0x05) << 1) | (cFlagsLineToPoint & 0x05)) {
            /**
             * It indicates that x1, y1, x2,and y2 are all in the clip area
             */
            case 0x00:
                LineClipY(rasterLine, x1, y1, x2, y2, clipFlagsMoveToPoint, cFlagsLineToPoint);
                break;
            /**
             * indicate x2 > clip.x2
             */
            case 0x01:
                RASTER_SCANLINE_CLIP_LINETO_0x01;
                break;
            /**
             * indicate x1 > clip.x2
             */
            case 0x02:
                RASTER_SCANLINE_CLIP_LINETO_0x02;
                break;
            /**
             * indicate x1 > clip.x2 && x2 > clip.x2
             */
            case 0x03:
                LineClipY(rasterLine, clipBox_.x2, y1, clipBox_.x2, y2,
                          clipFlagsMoveToPoint, cFlagsLineToPoint);
                break;
            /**
             * indicate x2 < clip.x1
             */
            case 0x04:
                RASTER_SCANLINE_CLIP_LINETO_0x04;
                break;
            /**
             * indicate x1 > clip.x2 && x2 < clip.x1
             */
            case 0x06:
                RASTER_SCANLINE_CLIP_LINETO_0x06;
                break;

            /**
             * indicate x1 < clip.x1
             */
            case 0x08:
                RASTER_SCANLINE_CLIP_LINETO_0x08;
                break;
            /**
             * indicate x1 < clip.x1 && x2 > clip.x2
             */
            case 0x09:
                RASTER_SCANLINE_CLIP_LINETO_0x09;
                break;
            /**
             * indicate x1 < clip.x1 && x2 < clip.x1
             */
            case 0x0c:
                LineClipY(rasterLine, clipBox_.x1, y1, clipBox_.x1, y2,
                          clipFlagsMoveToPoint, cFlagsLineToPoint);
                break;
        }
        clippingFlags_ = cFlagsLineToPoint;
    } else {
        LINE_OPERATE;
    }
    x1_ = x2;
    y1_ = y2;
}
} // namespace OHOS
#endif
