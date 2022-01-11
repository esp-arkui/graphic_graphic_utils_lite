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
 * @file span_pattern_rgba.h
 * @brief Defines Scan line of pattern
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_SPAN_PATTERN_RGBA_INCLUDED
#define GRAPHIC_SPAN_PATTERN_RGBA_INCLUDED

namespace OHOS {
    template <class Source>
    class SpanPatternFillRgba {
#if GRAPHIC_GEOMETYR_ENABLE_PATTERN_FILLSTROKECOLOR
    public:
        typedef Source PatternSource;
        typedef typename PatternSource::color_type colorType;
        typedef typename PatternSource::order_type orderType;
        SpanPatternFillRgba()
        {
        }
        SpanPatternFillRgba(PatternSource& source,
                            unsigned offset_x, unsigned offset_y)
            : patternSource(&source),
            offsetX(offset_x), offsetY(offset_y)
        {
        }
        /**
         * @brief Prepare Ready for render_scanlines_aa in
         */
        void Prepare()
        {
        }
        /**
         * @brief Generate From m_src takes out rgba and assigns it to rgba in span
         * @param span First address of scan line to be filled
         * @param x coordinate-x
         * @param y coordinate-y
         * @param len Scan line length
         */
        void Generate(colorType* span, int x, int y, unsigned len)
        {
            // z coordinate plus x, y offset
            x += offsetX;
            y += offsetY;
            // Extract the corresponding pixel from the corresponding image_accessors template
            const uint8_t* color = (const uint8_t*)patternSource->Span(x, y, len);
            for (; len; --len, color = (const uint8_t*)patternSource->NextX(), ++span) {
                if (color) {
                    // From source_type takes out the corresponding pixel.
                    span->redValue = color[orderType::RED];
                    span->greenValue = color[orderType::GREEN];
                    span->blueValue = color[orderType::BLUE];
                    span->alphaValue = color[orderType::ALPHA];
                } else {
                    // The default color is black and opaque
                    span->redValue = 0;
                    span->greenValue = 0;
                    span->blueValue = 0;
                    span->alphaValue = 255; // 0-transparent255-Opaque
                }
            }
        }

    private:
        PatternSource* patternSource;
        unsigned offsetX;
        unsigned offsetY;
#endif
    };
} // namespace OHOS

#endif
