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
 * @file span_allocator_converter.h
 * @brief Defines 扫描线分配器和转换器
 * @since 1.0
 * @version 1.0
 */

#ifndef SPAN_ALLOCATOR_CONVERTER_INCLUDED
#define SPAN_ALLOCATOR_CONVERTER_INCLUDED

#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_array.h"

namespace OHOS {
    template <class ColorT>
    class SpanFillColorAllocator {
    public:
        typedef ColorT color_type;
        /**
         * @brief Resize 重新分配计算m_span长度
         * @param span_len 扫描线长度
         * @return spans的首地址
         */
        GRAPHIC_GEOMETRY_INLINE color_type* Resize(unsigned span_len)
        {
            if (span_len > spans.Size()) {
                spans.Resize(((span_len + MAX_COLOR_NUM) >> BYTE_LENGTH) << BYTE_LENGTH);
            }
            return &spans[0];
        }

    private:
        GeometryPlainDataArray<color_type> spans;
    };

    template <class ColorT>
    class SpanSoildColor {
    public:
        typedef ColorT color_type;
        SpanSoildColor(ColorT color)
        {
            color_ = color;
        }
        void Prepare()
        {
        }
        void Generate(color_type* span, int, int, unsigned len)
        {
            for (; len; --len) {
                *span++ = color_;
            }
        }
    private:
        color_type color_;
    };

    template <class SpanGenerator1,class SpanGenerator2,class ColorT>
    class SpanBlendColor {
    public:
        typedef SpanGenerator1 SpanGen1;
        typedef SpanGenerator2 SpanGen2;
        typedef ColorT color_type;
        SpanBlendColor(SpanGen1& spanGenerator1,SpanGen2& spanGenerator2)
            : spanGenerator1_(spanGenerator1),
              spanGenerator2_(spanGenerator2)
        {
        }
        void Prepare()
        {
            spanGenerator1_.Prepare();
            spanGenerator2_.Prepare();
        }
        void Generate(color_type* span, int x, int y, unsigned len)
        {
            typedef OHOS::SpanFillColorAllocator<color_type> SpanAllocator;

            SpanAllocator allocator1;
            SpanAllocator allocator2;

            color_type* colors1 = allocator1.Resize(len);
            color_type* colors2 = allocator2.Resize(len);

            spanGenerator1_.Generate(colors1,x,y,len);
            spanGenerator2_.Generate(colors2,x,y,len);

            for (; len; --len,++span,colors1++,colors2++) {
                span->redValue = (colors1->redValue+colors2->redValue) >= MAX_COLOR_NUM ?
                            MAX_COLOR_NUM:(colors1->redValue+colors2->redValue);
                span->greenValue = (colors1->greenValue+colors2->greenValue) >= MAX_COLOR_NUM ?
                            MAX_COLOR_NUM:(colors1->greenValue+colors2->greenValue);
                span->blueValue = (colors1->blueValue+colors2->blueValue) >= MAX_COLOR_NUM ?
                            MAX_COLOR_NUM:(colors1->blueValue+colors2->blueValue);
                span->alphaValue = (colors1->alphaValue+colors2->alphaValue) >= MAX_COLOR_NUM ?
                            MAX_COLOR_NUM:(colors1->alphaValue+colors2->alphaValue);
            }
        }
    private:
        SpanGen1 spanGenerator1_;
        SpanGen2 spanGenerator2_;
    };
} // namespace OHOS
#endif
