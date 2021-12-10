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
* @brief Defines pattern的扫描线
* @since 1.0
* @version 1.0
*/

#ifndef GRAPHIC_SPAN_PATTERN_RGBA_INCLUDED
#define GRAPHIC_SPAN_PATTERN_RGBA_INCLUDED

namespace OHOS {
    template <class Source>
    class SpanPatternRgba {
    public:
        typedef Source PatternSource;
        typedef typename PatternSource::color_type colorType;
        typedef typename PatternSource::order_type orderType;
        typedef typename colorType::value_type valueType;
        SpanPatternRgba()
        {
        }
        SpanPatternRgba(PatternSource& source,
                        unsigned offset_x, unsigned offset_y) :
            patternSource(&source),
            offsetX(offset_x),
            offsetY(offset_y)
        {
        }
        /**
         * @brief Prepare 预备用给render_scanlines_aa中的
         */
        void Prepare()
        {
        }
        /**
         * @brief Generate 从m_src取出rgba赋予span中的rgba
         * @param span 需要填色的扫描线首地址
         * @param x 坐标-x
         * @param y 坐标-y
         * @param len 扫描线长度
         */
        void Generate(colorType* span, int x, int y, unsigned len)
        {
            //z坐标加上x，y的偏移量
            x += offsetX;
            y += offsetY;
            //从对应的image_accessors模板中取出对应像素
            const valueType* color = (const valueType*)patternSource->Span(x, y, len);
            for (; len; --len, color = (const valueType*)patternSource->NextX(), ++span) {
                if (color) {
                    //从source_type取出相应像素。
                    span->r = color[orderType::R];
                    span->g = color[orderType::G];
                    span->b = color[orderType::B];
                    span->a = color[orderType::A];
                } else {
                    //默认的颜色黑色不透明
                    span->r = 0;
                    span->g = 0;
                    span->b = 0;
                    span->a = 255; //0-透明255-不透明
                }
            }
        }

    private:
        PatternSource* patternSource;
        unsigned offsetX;
        unsigned offsetY;
    };
} // namespace OHOS

#endif
