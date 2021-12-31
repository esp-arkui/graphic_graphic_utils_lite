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

#include "gfx_utils/color.h"
#include "gfx_utils/graphics/common/graphic_common_basics.h"
#include "gfx_utils/graphics/transform/graphic_transform_affine.h"
#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_dda_line.h"

namespace OHOS {
    /**
     * 渐变的颜色插入器
     */
    template <class ColorT>
    struct ColorInterpolator {
#if GRAPHIC_GEOMETYR_ENABLE_GRADIENT_FILLSTROKECOLOR
    public:
        typedef ColorT color_type;

        ColorInterpolator(const color_type& color1,
                          const color_type& color2,
                          unsigned distance) :
            colorStart(color1),
            colorEnd(color2), len(distance), place(0)
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
        color_type GetColor() const
        {
            return colorStart.Gradient(colorEnd, double(place) / len);
        }

    private:
        color_type colorStart;
        color_type colorEnd;
        unsigned len;
        unsigned place;
#endif
    };

    /**
     * 线性的扫描线插入器
     */
    template <class Transformer = TransAffine, unsigned SUBPIXELSHIFT = 8>
    class SpanInterpolatorLinear {
    public:
        typedef Transformer trans_type;

        enum SubpixelScale {
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
            Begin(x, y, len);
        }
        const trans_type& GetTransformer() const
        {
            return *transType;
        }
        void SetTransformer(trans_type& trans)
        {
            transType = &trans;
        }

        /*
         * 重新更新设置dda2LineInterpolatorX与dda2LineInterpolatorY属性
         */
        void Begin(double x, double y, unsigned len)
        {
            double tx;
            double ty;

            tx = x;
            ty = y;

            transType->Transform(&tx, &ty);
            int x1 = Iround(tx * SUBPIXEL_SCALE);
            int y1 = Iround(ty * SUBPIXEL_SCALE);

            tx = x + len;
            ty = y;
            transType->Transform(&tx, &ty);
            int x2 = Iround(tx * SUBPIXEL_SCALE);
            int y2 = Iround(ty * SUBPIXEL_SCALE);

            dda2LineInterpolatorX = DdaTwoLineInterpolator(x1, x2, len);
            dda2LineInterpolatorY = DdaTwoLineInterpolator(y1, y2, len);
        }

        /*
         * 重新更新设置dda2LineInterpolatorX与dda2LineInterpolatorY属性
         */
        void Resynchronize(double xe, double ye, unsigned len)
        {
            transType->transform(&xe, &ye);
            dda2LineInterpolatorX = DdaTwoLineInterpolator(
                dda2LineInterpolatorX.GetCoordinate(), Iround(xe * SUBPIXEL_SCALE), len);
            dda2LineInterpolatorY = DdaTwoLineInterpolator(
                dda2LineInterpolatorY.GetCoordinate(), Iround(ye * SUBPIXEL_SCALE), len);
        }

        /**
         * @brief 重载++运算符
         */
        void operator++()
        {
            ++dda2LineInterpolatorX;
            ++dda2LineInterpolatorY;
        }

        void Coordinates(int* x, int* y) const
        {
            *x = dda2LineInterpolatorX.GetCoordinate();
            *y = dda2LineInterpolatorY.GetCoordinate();
        }

    private:
        trans_type* transType;
        DdaTwoLineInterpolator dda2LineInterpolatorX;
        DdaTwoLineInterpolator dda2LineInterpolatorY;
    };
} // namespace OHOS

#endif
