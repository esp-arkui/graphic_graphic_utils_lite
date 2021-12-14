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

#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_array.h"

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
        using CoverType = int8u;
        using CoordType = int16;
        struct SpanBlock {
            CoordType x;
            CoordType spanLength;
            CoverType* covers;
        };

        using Iterator = SpanBlock*;
        using ConstIterator = const SpanBlock*;

        ScanlineUnPackedContainer()
            : minX_(0),
              lastX_(0x7FFFFFF0),
              curSpan_(0)
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
            if (max_len > spans_.Size()) {
                spans_.Resize(max_len);
                covers_.Resize(max_len);
            }
            lastX_ = 0x7FFFFFF0;
            minX_ = min_x;
            curSpan_ = &spans_[0];
        }

        /* 根据x的位置 以及cover 颜色覆盖率是扩展call的区域
         * 还是添加cell单元 。
         */
        void AddCell(int x, unsigned cover)
        {
            x -= minX_;
            covers_[x] = (CoverType)cover;
            if (x == lastX_ + 1) {
                curSpan_->spanLength++;
            } else {
                curSpan_++;
                curSpan_->x = (CoordType)(x + minX_);
                curSpan_->spanLength = 1;
                curSpan_->covers = &covers_[x];
            }
            lastX_ = x;
        }

        /* 根据x的位置 len的span length 以及cover 颜色覆盖率是扩展call的区域
        * 还是添加cells单元数组 。
        */
        void AddCells(int x, unsigned cellLength, const CoverType* covers)
        {
            x -= minX_;
            std::memcpy(&covers_[x], covers, cellLength * sizeof(CoverType));
            if (x == lastX_ + 1) {
                curSpan_->spanLength += (CoordType)cellLength;
            } else {
                curSpan_++;
                curSpan_->x = (CoordType)(x + minX_);
                curSpan_->spanLength = (CoordType)cellLength;
                curSpan_->covers = &covers_[x];
            }
            lastX_ = x + cellLength - 1;
        }

        /* 根据x的位置 len的span length 以及cover 颜色覆盖率是扩展call的区域
        * 还是添加span单元数组 。这里注意会把具有相同
        * coverage value 的像素合并成 solid span。
        */
        void AddSpan(int x, unsigned spanLength, unsigned cover)
        {
            x -= minX_;
            std::memset(&covers_[x], cover, spanLength);
            if (x == lastX_ + 1) {
                curSpan_->spanLength += (CoordType)spanLength;
            } else {
                curSpan_++;
                curSpan_->x = (CoordType)(x + minX_);
                curSpan_->spanLength = (CoordType)spanLength;
                curSpan_->covers = &covers_[x];
            }
            lastX_ = x + spanLength - 1;
        }

        /*
        * 结束操作
        */
        void Finalize(int y)
        {
            y_ = y;
        }
        void ResetSpans()
        {
            lastX_ = 0x7FFFFFF0;
            curSpan_ = &spans_[0];
        }

        int GetYLevel() const
        {
            return y_;
        }
        unsigned NumSpans() const
        {
            return unsigned(curSpan_ - &spans_[0]);
        }
        ConstIterator Begin() const
        {
            return &spans_[1];
        }
        Iterator Begin()
        {
            return &spans_[1];
        }

    private:
        ScanlineUnPackedContainer(const self_type&);
        const self_type& operator=(const self_type&);

    private:
        int minX_;
        int lastX_;
        int y_;
        PodArray<CoverType> covers_;
        PodArray<SpanBlock> spans_;
        SpanBlock* curSpan_;
    };
} // namespace OHOS

#endif
