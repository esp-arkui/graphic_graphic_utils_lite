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
 * @brief Defines Scan line distributor and converter
 * @since 1.0
 * @version 1.0
 */

#ifndef SPAN_ALLOCATOR_CONVERTER_INCLUDED
#define SPAN_ALLOCATOR_CONVERTER_INCLUDED

#include "gfx_utils/diagram/vertexprimitive/graphic_geometry_plaindata_array.h"

namespace OHOS {
template <class ColorT>
class SpanFillColorAllocator {
public:
    /**
     * @brief Resize Redistribution calculation m_Span length
     * @param span_len Scan line length
     * @return First address of spans
     */
    inline ColorT* Resize(unsigned span_len)
    {
        if (span_len > spans_.Size()) {
            spans_.Resize(((span_len + MAX_COLOR_NUM) >> BYTE_LENGTH) << BYTE_LENGTH);
        }
        return &spans_[0];
    }

    inline ColorT* GetPtr()
    {
        if (spans_.Size() < 0) {
            spans_.Resize(INDEX_EIGHT);
        }
        return &spans_[0];
    }
private:
    GraphicGeometryPlainDataArray<ColorT> spans_;
};

template <class ColorT>
class SpanSoildColor {
public:
    SpanSoildColor(ColorT color)
    {
        color_ = color;
    }
    void Prepare() {}
    void Generate(ColorT* span, int, int, unsigned len)
    {
        for (; len; --len) {
            *span++ = color_;
        }
    }
private:
    ColorT color_;
};

template <class SpanGenerator1, class SpanGenerator2, class ColorT>
class SpanBlendColor {
public:
    using SpanGen1 = SpanGenerator1;
    using SpanGen2 = SpanGenerator2;
    SpanBlendColor(SpanGen1& spanGenerator1, SpanGen2& spanGenerator2)
        : spanGenerator1_(spanGenerator1),
          spanGenerator2_(spanGenerator2)
    {
    }
    void Prepare()
    {
        spanGenerator1_.Prepare();
        spanGenerator2_.Prepare();
    }
    void Generate(ColorT* span, int x, int y, unsigned len)
    {
        using SpanAllocator = OHOS::SpanFillColorAllocator<ColorT>;

        SpanAllocator allocator1;
        SpanAllocator allocator2;

        ColorT* colors1 = allocator1.Resize(len);
        ColorT* colors2 = allocator2.Resize(len);

        spanGenerator1_.Generate(colors1, x, y, len);
        spanGenerator2_.Generate(colors2, x, y, len);

        for (; len; --len, ++span, colors1++, colors2++) {
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
