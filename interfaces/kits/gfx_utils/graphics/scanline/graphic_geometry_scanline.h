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

#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_array.h"

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
    class ScanlineUnPackedContainer {
    public:
        typedef ScanlineUnPackedContainer self_type;
        /**
         * Pixel coverage type
         */
        using cover_type = uint8_t;
        using coord_type = int16_t;
        struct SpanBlock {
            coord_type x;
            coord_type spanLength;
            cover_type* covers;
        };
        using Iterator = SpanBlock* ;
        using ConstIterator = const SpanBlock* ;
        ScanlineUnPackedContainer()
            : minScaneLineXCoord(0),
            lastScaneLineXCoord(0x7FFFFFF0),
            curSpanBlock(0)
        {}
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
        void Reset(int min_x, int max_x)
        {
            const int liftNumber = 2;
            unsigned max_len = max_x - min_x + liftNumber;
            if (max_len > arraySpans.Size()) {
                arraySpans.Resize(max_len);
                arrayCovers.Resize(max_len);
            }
            lastScaneLineXCoord = 0x7FFFFFF0;
            minScaneLineXCoord = min_x;
            curSpanBlock = &arraySpans[0];
        }

        /**
         * According to the position of X and the cover color, the coverage is the area of the extended call
         * or add a cell
         */
        void AddCell(int x, unsigned cover)
        {
            x -= minScaneLineXCoord;
            arrayCovers[x] = (uint8_t)cover;
            if (x == lastScaneLineXCoord + 1) {
                curSpanBlock->spanLength++;
            } else {
                curSpanBlock++;
                curSpanBlock->x = (coord_type)(x + minScaneLineXCoord);
                curSpanBlock->spanLength = 1;
                curSpanBlock->covers = &arrayCovers[x];
            }
            lastScaneLineXCoord = x;
        }

        /**
         * According to the position of X, the span length of len and
         * the cover color coverage are the areas where the call is extended
         * or add a cell
         */
        void AddCells(int x, unsigned cellLength, const uint8_t* covers)
        {
            x -= minScaneLineXCoord;
            memcpy_s(&arrayCovers[x], cellLength * sizeof(uint8_t), covers, cellLength * sizeof(uint8_t));
            if (x == lastScaneLineXCoord + 1) {
                curSpanBlock->spanLength += (coord_type)cellLength;
            } else {
                curSpanBlock++;
                curSpanBlock->x = (coord_type)(x + minScaneLineXCoord);
                curSpanBlock->spanLength = (coord_type)cellLength;
                curSpanBlock->covers = &arrayCovers[x];
            }
            lastScaneLineXCoord = x + cellLength - 1;
        }

        /**
         * According to the position of X, the span length of len and
         * the cover color coverage are the areas where the call is extended
         * Or add a span cell array.
         * Note here that pixels with the same coverage value will be merged into solid span.
         */
        void AddSpan(int x, unsigned spanLength, unsigned cover)
        {
            x -= minScaneLineXCoord;
            memset_s(&arrayCovers[x], spanLength, cover, spanLength);
            if (x == lastScaneLineXCoord + 1) {
                curSpanBlock->spanLength += (coord_type)spanLength;
            } else {
                curSpanBlock++;
                curSpanBlock->x = (coord_type)(x + minScaneLineXCoord);
                curSpanBlock->spanLength = (coord_type)spanLength;
                curSpanBlock->covers = &arrayCovers[x];
            }
            lastScaneLineXCoord = x + spanLength - 1;
        }

        /**
         * End operation
         */
        void Finalize(int y)
        {
            scaneLineYCoord = y;
        }
        void ResetSpans()
        {
            lastScaneLineXCoord = 0x7FFFFFF0;
            curSpanBlock = &arraySpans[0];
        }

        int GetYLevel() const
        {
            return scaneLineYCoord;
        }
        unsigned NumSpans() const
        {
            return unsigned(curSpanBlock - &arraySpans[0]);
        }
        ConstIterator Begin() const
        {
            return &arraySpans[1];
        }
        Iterator Begin()
        {
            return &arraySpans[1];
        }

    private:
        ScanlineUnPackedContainer(const self_type&);
        const self_type& operator=(const self_type&);

    private:
        int minScaneLineXCoord;
        int lastScaneLineXCoord;
        int scaneLineYCoord;
        GeometryPlainDataArray<uint8_t> arrayCovers;
        GeometryPlainDataArray<SpanBlock> arraySpans;
        SpanBlock* curSpanBlock;
    };
} // namespace OHOS

#endif
