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
 * @addtogroup GraphicGeometry
 * @{
 *
 * @brief Defines RoundedRect.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file graphic_geometry_bounding_rect.h
 *
 * @brief Defines ‘≤Ω«æÿ–Œ¿‡.
 *
 * @since 1.0
 * @version 1.0
 */
#ifndef GRAPHIC_GEOMETRY_ROUNDED_RECT_INCLUDED
#define GRAPHIC_GEOMETRY_ROUNDED_RECT_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "agg_arc.h"

namespace OHOS{
class RoundedRect : public HeapBase{
public:
    RoundedRect(double x1, double y1, double x2, double y2, double r);

    RoundedRect() {}

    void Radius(double rx, double ry);

    void Radius(double rxBottom, double ryBottom, double rxTop, double ryTop);

    void Radius(double rx1, double ry1, double rx2, double ry2, 
                double rx3, double ry3, double rx4, double ry4);

    void ApproximationScale(double s)
    {
        arc_.ApproximationScale(s);
    }

    void Rect(double x1, double y1, double x2, double y2);

    void Radius(double r);

    void NormalizeRadius();

    void Rewind(unsigned);

    double ApproximationScale() const
    {
        return arc_.ApproximationScale();
    }

    unsigned Vertex(double* x, double* y);

private:
    unsigned status_;
    Arc arc_;
    double x1_;
    double y1_;
    double x2_;
    double y2_;
    double rx1_;
    double ry1_;
    double rx2_;
    double ry2_;
    double rx3_;
    double ry3_;
    double rx4_;
    double ry4_;

};

}

#endif

