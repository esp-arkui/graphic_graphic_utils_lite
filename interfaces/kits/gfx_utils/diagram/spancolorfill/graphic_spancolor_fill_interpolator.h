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
 * @brief Defines Scan line interpolator
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_SPAN_INTERPOLATOR_LINEAR_INCLUDED
#define GRAPHIC_SPAN_INTERPOLATOR_LINEAR_INCLUDED

#include "gfx_utils/color.h"
#include "gfx_utils/diagram/common/graphic_common_basics.h"
#include "gfx_utils/trans_affine.h"
#include "gfx_utils/diagram/vertexprimitive/graphic_geometry_dda_line.h"
#include "gfx_utils/graphic_math.h"
namespace OHOS {
/**
 * Gradient color interpolator
 */
template <class ColorT>
struct ColorInterpolator {
#if GRAPHIC_ENABLE_GRADIENT_FILL_FLAG
public:
    using ColorType = ColorT;

    ColorInterpolator(const ColorType& color1, const ColorType& color2, unsigned distance)
        : colorStart_(color1), colorEnd_(color2), len_(distance), place_(0) {}

    /**
     * @brief overwrite ++
     */
    void operator++()
    {
        ++place_;
    }

    /**
     * @brief Returns the color at count during the transition from colorstart to colorend
     * @return
     */
    ColorType GetColor() const
    {
        return colorStart_.Gradient(colorEnd_, float(place_) / len_);
    }

private:
    ColorType colorStart_;
    ColorType colorEnd_;
    unsigned len_;
    unsigned place_;
#endif
};

/**
 * Linear scan line inserter
 */
template <class Transformer = TransAffine, unsigned SUBPIXELSHIFT = 8>
class GraphicSpancolorFillInterpolator {
public:
    using TransType = Transformer;

    enum SubpixelScale {
        SUBPIXEL_SHIFT = SUBPIXELSHIFT,
        SUBPIXEL_SCALE = 1 << SUBPIXEL_SHIFT
    };
    GraphicSpancolorFillInterpolator() {}
    GraphicSpancolorFillInterpolator(TransType& trans) : transType_(&trans) {}
    GraphicSpancolorFillInterpolator(TransType& trans,
                           float x, float y, unsigned len) : transType_(&trans)
    {
        Begin(x, y, len);
    }

    const TransType& GetTransformer() const
    {
        return *transType_;
    }
    void SetTransformer(TransType& trans)
    {
        transType_ = &trans;
    }

    /*
     * Update and set dda2lineinterpolatorx and dda2lineinterpolatory properties again
     */
    void Begin(float x, float y, unsigned len)
    {
        float tx;
        float ty;

        tx = x;
        ty = y;

        transType_->Transform(&tx, &ty);
        int x1 = MATH_ROUND32(tx * SUBPIXEL_SCALE);
        int y1 = MATH_ROUND32(ty * SUBPIXEL_SCALE);

        tx = x + len;
        ty = y;
        transType_->Transform(&tx, &ty);
        int x2 = MATH_ROUND32(tx * SUBPIXEL_SCALE);
        int y2 = MATH_ROUND32(ty * SUBPIXEL_SCALE);

        dda2LineInterpolatorX_ = GraphicGeometryDdaLine(x1, x2, len);
        dda2LineInterpolatorY_ = GraphicGeometryDdaLine(y1, y2, len);
    }

    /**
     * Update and set dda2lineinterpolatorx and dda2lineinterpolatory properties again
     */
    void Resynchronize(float xe, float ye, unsigned len)
    {
        transType_->transform(&xe, &ye);
        dda2LineInterpolatorX_ = GraphicGeometryDdaLine(
            dda2LineInterpolatorX_.GetCoordinate(), MATH_ROUND32(xe * SUBPIXEL_SCALE), len);
        dda2LineInterpolatorY_ = GraphicGeometryDdaLine(
            dda2LineInterpolatorY_.GetCoordinate(), MATH_ROUND32(ye * SUBPIXEL_SCALE), len);
    }

    /**
     * @brief Overloading + + operators
     */
    void operator++()
    {
        ++dda2LineInterpolatorX_;
        ++dda2LineInterpolatorY_;
    }

    void Coordinates(int* x, int* y) const
    {
        *x = dda2LineInterpolatorX_.GetCoordinate();
        *y = dda2LineInterpolatorY_.GetCoordinate();
    }

private:
    TransType* transType_;
    GraphicGeometryDdaLine dda2LineInterpolatorX_;
    GraphicGeometryDdaLine dda2LineInterpolatorY_;
};
} // namespace OHOS
#endif
