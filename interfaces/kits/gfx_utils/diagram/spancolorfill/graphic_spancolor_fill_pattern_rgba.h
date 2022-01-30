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
#ifndef GRAPHIC_SPAN_PATTERN_RGBA_INCLUDED
#define GRAPHIC_SPAN_PATTERN_RGBA_INCLUDED

#include <gfx_utils/image_info.h>
#include "gfx_utils/color.h"

/**
 * @file span_pattern_rgba.h
 * @brief Defines Scan line of pattern
 * @since 1.0
 * @version 1.0
 */

namespace OHOS {
#define PATTERN_REPEAT_X \
    x = x % patternImagewidth_; \
    if (y>=patternImageheigth_) { \
        ChangeColor(span, NoColor()); \
    } else { \
        y = y % patternImageheigth_; \
        if (x >= patternImagewidth_ || y >= patternImageheigth_) { \
            ChangeColor(span, NoColor()); \
        } else { \
            ChangeColor(span, patternImage_[patternImagewidth_ * y + x]); \
        } \
    }

#define GENERATE_REPEAT_Y \
    x = x % patternImagewidth_; \
    if (x >= patternImagewidth_ || \
       y >= patternImageheigth_) { \
        ChangeColor(span, NoColor()); \
    } else { \
        ChangeColor(span, patternImage_[patternImagewidth_ * y + x]); \
    }

template <class ColorT>
class GraphicSpancolorFillPatternRgba {
#if GRAPHIC_ENABLE_PATTERN_FILL_FLAG
public:
    typedef ColorT Color;
    GraphicSpancolorFillPatternRgba() {}

    GraphicSpancolorFillPatternRgba(const ImageInfo* image,
                                    PatternRepeatMode patternRepeat,
                                    double startX, double startY)
        : patternRepeat_(patternRepeat)
    {
        if (image->header.colorMode==ARGB8888) {
            patternImage_ = reinterpret_cast<Color32*>(const_cast<uint8_t*>(image->data));
            patternImageheigth_ = image->header.height;
            patternImagewidth_ = image->header.width;
            patternStartX_ = startX;
            patternStartY_ = startY;
        }
    }

    void Attach(const ImageInfo* image, PatternRepeatMode patternRepeat, double startX, double startY)
    {
        patternRepeat_ = patternRepeat;
        if (image->header.colorMode == ARGB8888) {
            patternImage_ = reinterpret_cast<Color32*>(const_cast<uint8_t*>(image->data));
            patternImageheigth_ = image->header.height;
            patternImagewidth_ = image->header.width;
            patternStartX_ = startX;
            patternStartY_ = startY;
        }
    }

    /**
     * @brief black
     * @return black
     */
    ColorType NoColor() const
    {
        ColorType fillColor;
        fillColor.red = 0;
        fillColor.green = 0;
        fillColor.blue = 0;
        fillColor.alpha = MAX_COLOR_NUM;
        return fillColor;
    }

    /**
     * @brief Prepare Ready for render_scanlines_aa in
     */
    void Prepare() {}

    void Generate(Color* span, int x, int y, unsigned len)
    {
        y = y - patternStartY_;
        x = x - patternStartX_;
        for (; len; --len, span++, x++) {
            if (patternRepeat_ == NO_REPEAT) {
                if (x >= patternImagewidth_ ||
                        y >= patternImageheigth_) {
                    ChangeColor(span, NoColor());
                } else {
                    ChangeColor(span, patternImage_[patternImagewidth_ * y + x]);
                }
            }

            if (patternRepeat_ == REPEAT) {
                x = x % patternImagewidth_;
                y = y % patternImageheigth_;
                if (x >= patternImagewidth_ || y >= patternImageheigth_) {
                    ChangeColor(span, NoColor());
                } else {
                    ChangeColor(span, patternImage_[patternImagewidth_ * y + x]);
                }
            }

            if (patternRepeat_ == REPEAT_X) {
                PATTERN_REPEAT_X
            }

            if (patternRepeat_ == REPEAT_Y) {
                y = y % patternImageheigth_;
                if (x >= patternImageheigth_) {
                    ChangeColor(span, NoColor());
                } else {
                    GENERATE_REPEAT_Y
                }
            }
        }
    }
private:
    PatternRepeatMode patternRepeat_;
    const Color32* patternImage_;
    uint16_t patternImageheigth_;
    uint16_t patternImagewidth_;
    double patternStartX_;
    double patternStartY_;

    void ChangeColor(Color* color, ColorType colorType)
    {
        color->redValue = colorType.red;
        color->greenValue = colorType.green;
        color->blueValue = colorType.blue;
        color->alphaValue = colorType.alpha;
    }
#endif
};
} // namespace OHOS

#endif
