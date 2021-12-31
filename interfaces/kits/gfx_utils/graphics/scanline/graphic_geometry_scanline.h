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
* @brief 在从 rasterizer（光栅化器） 到 scanline renderer 之间的信息转换时，
* 用到了 scanline 容器。一个 scanline 由很多的水平的、不相交的 span 组成。
* 这些 span 按 x 轴排序。这就意味着并没有提供排序这个操作，必须在将 span 添加到
* scanline 时就把顺序处理好。.
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
    * @brief 16位的Unpacked scanline container class - 包装过的 scanline 容器,
    * 容器可以包含去锯齿信息.unpacked scanline 总是会为所有像素保存覆盖值（coverage value），
    * 包括那些全部覆盖在多边形内部的像素。
    * @since 1.0
    * @version 1.0
    */
    class ScanlineUnPackedContainer {
    public:
        typedef ScanlineUnPackedContainer self_type;
        /*
         * 像素覆盖率类型
         */
        using cover_type = int8u;
        using coord_type = int16;
        struct SpanBlock {
            coord_type x;
            coord_type spanLength;
            cover_type* covers;
        };
        using Iterator = SpanBlock*;
        using ConstIterator = const SpanBlock*;
        ScanlineUnPackedContainer() :
            minScaneLineXCoord(0),
            lastScaneLineXCoord(0x7FFFFFF0),
            curSpanBlock(0)
        {}
        /**
            * @class ScanlineUnPackedContainer
            * @brief 16位的Unpacked scanline container class - 包装过的 scanline 容器,
            * 容器可以包含去锯齿信息.包装的(packed）scanline 与未包装的（unpacked）
            * scanline 的区别在于，unpacked scanline 总是会为所有像素保存覆盖值（coverage value），
            * 包括那些全部覆盖在多边形内部的像素。而 packed scanlines 则会把具有相同
            * coverage value 的像素合并成 solid span。
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

        /* 根据x的位置 以及cover 颜色覆盖率是扩展call的区域
         * 还是添加cell单元 。
         */
        void AddCell(int x, unsigned cover)
        {
            x -= minScaneLineXCoord;
            arrayCovers[x] = (CoverType)cover;
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

        /* 根据x的位置 len的span length 以及cover 颜色覆盖率是扩展call的区域
        * 还是添加cells单元数组 。
        */
        void AddCells(int x, unsigned cellLength, const CoverType* covers)
        {
            x -= minScaneLineXCoord;
            memcpy_s(&arrayCovers[x], cellLength * sizeof(CoverType), covers, cellLength * sizeof(CoverType));
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

        /* 根据x的位置 len的span length 以及cover 颜色覆盖率是扩展call的区域
        * 还是添加span单元数组 。这里注意会把具有相同
        * coverage value 的像素合并成 solid span。
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

        /*
        * 结束操作
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
        GeometryPlainDataArray<CoverType> arrayCovers;
        GeometryPlainDataArray<SpanBlock> arraySpans;
        SpanBlock* curSpanBlock;
    };
} // namespace OHOS

#endif
