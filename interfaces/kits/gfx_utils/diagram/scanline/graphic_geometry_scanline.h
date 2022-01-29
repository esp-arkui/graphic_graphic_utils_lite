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
 * @file graphic_geometry_scanline_u.h
 *
 * @brief When converting information from rasterizer to scanline renderer,
 * The scanline container is used. A scanline consists of many horizontal, disjoint spans.
 * These spans are sorted by the x-axis
 * This means that sorting is not provided and must be handled when span is added to scanline
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_SCANLINE_U_INCLUDED
#define GRAPHIC_GEOMETRY_SCANLINE_U_INCLUDED

#include <cstring>

#include "gfx_utils/diagram/vertexprimitive/graphic_geometry_plaindata_array.h"

namespace OHOS {
/**
 * @class ScanlineUnPackedContainer
 * @brief 16 bit unpacked scan line container class - packed scan line container,
 * Containers can contain de aliasing information Unpacked scanline always
 * saves overlay values for all pixels（coverage value）,
 * Include those pixels that all cover the interior of the polygon
 * @since 1.0
 * @version 1.0
 */
class   GraphicGeometryScanline {
public:
    /**
     * Pixel coverage type
     */
    struct SpanBlock {
        int16_t x;
        int16_t spanLength;
        uint8_t* covers;
    };
    using Iterator = SpanBlock* ;
    using ConstIterator = const SpanBlock* ;
    GraphicGeometryScanline() : minScaneLineXCoord_(0), lastScaneLineXCoord_(0x7FFFFFF0), curSpanBlock_(0) {}
    /**
     * @class ScanlineUnPackedContainer
     * @brief 16 bit unpacked scan line container class - packed scan line container,
     * Containers can contain de aliasing information Packed scanline and unpacked scanline
     * The difference between scanline and unpacked scanline is that
     * unpacked scanline always saves coverage values for all pixels,
     * Include those pixels that are all covered inside the polygon.
     * Packed scanlines will merge pixels with the same coverage value into solid span
     * @since 1.0
     * @version 1.0
     */
    void Reset(int minX, int maxX)
    {
        const int liftNumber = 2;
        unsigned maxLen = maxX - minX + liftNumber;
        if (maxLen > arraySpans_.Size()) {
            arraySpans_.Resize(maxLen);
            arrayCovers_.Resize(maxLen);
        }
        lastScaneLineXCoord_ = 0x7FFFFFF0;
        minScaneLineXCoord_ = minX;
        curSpanBlock_ = &arraySpans_[0];
    }

    /**
     * According to the position of X and the cover color, the coverage is the area of the extended call
     * or add a cell
     */
    void AddCell(int x, unsigned cover)
    {
        x -= minScaneLineXCoord_;
        arrayCovers_[x] = (uint8_t)cover;
        if (x == lastScaneLineXCoord_ + 1) {
            curSpanBlock_->spanLength++;
        } else {
            curSpanBlock_++;
            curSpanBlock_->x = (int16_t)(x + minScaneLineXCoord_);
            curSpanBlock_->spanLength = 1;
            curSpanBlock_->covers = &arrayCovers_[x];
        }
        lastScaneLineXCoord_ = x;
    }

    /**
     * According to the position of X, the span length of len and
     * the cover color coverage are the areas where the call is extended
     * or add a cell
     */
    void AddCells(int x, unsigned cellLength, const uint8_t* covers)
    {
        x -= minScaneLineXCoord_;
        memcpy_s(&arrayCovers_[x], cellLength * sizeof(uint8_t), covers, cellLength * sizeof(uint8_t));
        if (x == lastScaneLineXCoord_ + 1) {
            curSpanBlock_->spanLength += (int16_t)cellLength;
        } else {
            curSpanBlock_++;
            curSpanBlock_->x = (int16_t)(x + minScaneLineXCoord_);
            curSpanBlock_->spanLength = (int16_t)cellLength;
            curSpanBlock_->covers = &arrayCovers_[x];
        }
        lastScaneLineXCoord_ = x + cellLength - 1;
    }

    /**
     * According to the position of X, the span length of len and
     * the cover color coverage are the areas where the call is extended
     * Or add a span cell array.
     * Note here that pixels with the same coverage value will be merged into solid span.
     */
    void AddSpan(int x, unsigned spanLength, unsigned cover)
    {
        x -= minScaneLineXCoord_;
        memset_s(&arrayCovers_[x], spanLength, cover, spanLength);
        if (x == lastScaneLineXCoord_ + 1) {
            curSpanBlock_->spanLength += (int16_t)spanLength;
        } else {
            curSpanBlock_++;
            curSpanBlock_->x = (int16_t)(x + minScaneLineXCoord_);
            curSpanBlock_->spanLength = (int16_t)spanLength;
            curSpanBlock_->covers = &arrayCovers_[x];
        }
        lastScaneLineXCoord_ = x + spanLength - 1;
    }

    /**
     * End operation
     */
    void Finalize(int y)
    {
        scaneLineYCoord_ = y;
    }
    void ResetSpans()
    {
        lastScaneLineXCoord_ = 0x7FFFFFF0;
        curSpanBlock_ = &arraySpans_[0];
    }

    int GetYLevel() const
    {
        return scaneLineYCoord_;
    }
    unsigned NumSpans() const
    {
        return unsigned(curSpanBlock_ - &arraySpans_[0]);
    }
    ConstIterator Begin() const
    {
        return &arraySpans_[1];
    }
    Iterator Begin()
    {
        return &arraySpans_[1];
    }

private:
    GraphicGeometryScanline(const GraphicGeometryScanline&);
    const GraphicGeometryScanline& operator=(const GraphicGeometryScanline&);

private:
    int minScaneLineXCoord_;
    int lastScaneLineXCoord_;
    int scaneLineYCoord_;
    GraphicGeometryPlainDataArray<uint8_t> arrayCovers_;
    GraphicGeometryPlainDataArray<SpanBlock> arraySpans_;
    SpanBlock* curSpanBlock_;
};
} // namespace OHOS

#endif
