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
* @file span_interpolator.h
* @brief Defines 扫描线插入器
* @since 1.0
* @version 1.0
*/

#ifndef GRAPHIC_SPAN_INTERPOLATOR_LINEAR_INCLUDED
#define GRAPHIC_SPAN_INTERPOLATOR_LINEAR_INCLUDED

#include "gfx_utils/graphics/graphic_color/agg_color_rgba.h"
#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "gfx_utils/graphics/graphic_geometry/agg_dda_line.h"
#include "gfx_utils/graphics/graphic_transform/agg_trans_affine.h"

namespace OHOS {
    /**
     *渐变的颜色插入器
     */
    template <class ColorT>
    struct ColorInterpolator {
    public:
        typedef ColorT color_type;

        ColorInterpolator(const color_type& color1,
                          const color_type& color2,
                          unsigned distance) :
            colorStart(color1),
            colorEnd(color2),
            len(distance),
            place(0)
        {
        }

        /**
         * @brief 重写++
         */
        void operator++()
        {
            ++place;
        }

        /**
         * @brief 返回colorStart渐变到colorEnd过程中处于count的颜色
         * @return
         */
        color_type color() const
        {
            return colorStart.gradient(colorEnd, double(place) / len);
        }

    private:
        color_type colorStart;
        color_type colorEnd;
        unsigned len;
        unsigned place;
    };

    /**
     *线性的扫描线插入器
     */
    template <class Transformer = TransAffine, unsigned SUBPIXELSHIFT = 8>
    class SpanInterpolatorLinear {
    public:
        typedef Transformer trans_type;

        enum SubpixelScale
        {
            SUBPIXEL_SHIFT = SUBPIXELSHIFT,
            SUBPIXEL_SCALE = 1 << SUBPIXEL_SHIFT
        };
        SpanInterpolatorLinear()
        {
        }
        SpanInterpolatorLinear(trans_type& trans) :
            transType(&trans)
        {
        }
        SpanInterpolatorLinear(trans_type& trans,
                               double x, double y, unsigned len) :
            transType(&trans)
        {
            begin(x, y, len);
        }
        const trans_type& transformer() const
        {
            return *transType;
        }
        void transformer(trans_type& trans)
        {
            transType = &trans;
        }
        void begin(double x, double y, unsigned len)
        {
            double tx;
            double ty;

            tx = x;
            ty = y;

            transType->Transform(&tx, &ty);
            int x1 = iround(tx * SUBPIXEL_SCALE);
            int y1 = iround(ty * SUBPIXEL_SCALE);

            tx = x + len;
            ty = y;
            transType->Transform(&tx, &ty);
            int x2 = iround(tx * SUBPIXEL_SCALE);
            int y2 = iround(ty * SUBPIXEL_SCALE);

            dda2LineInterpolatorX = dda2_line_interpolator(x1, x2, len);
            dda2LineInterpolatorY = dda2_line_interpolator(y1, y2, len);
        }

        /**
         * @brief 重写++
         */
        void operator++()
        {
            ++dda2LineInterpolatorX;
            ++dda2LineInterpolatorY;
        }

        void coordinates(int* x, int* y) const
        {
            *x = dda2LineInterpolatorX.y();
            *y = dda2LineInterpolatorY.y();
        }

    private:
        trans_type* transType;
        dda2_line_interpolator dda2LineInterpolatorX;
        dda2_line_interpolator dda2LineInterpolatorY;
    };
} // namespace OHOS

#endif
