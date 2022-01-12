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

#ifndef GRAPHIC_GEOMETRY_ARC_INCLUDED
#define GRAPHIC_GEOMETRY_ARC_INCLUDED

#include "gfx_utils/diagram/common/graphic_common_basics.h"
#include "gfx_utils/heap_base.h"
namespace OHOS {
/**
 * @file graphic_geometry_arc.h
 *
 * @brief Defines Arc class.
 *
 * @since 1.0
 * @version 1.0
 */
class GraphicGeometryArc : public HeapBase {
#if GRAPHIC_ENABLE_ARC_FLAG
public:
    GraphicGeometryArc() : initialized_(false), expansionRatio_(1.0f) {}

    /**
     * @brief Constructor arc.
     * @param centerX,centerY Arc Center.
     * @param rx Ellipse Arc Transverse Axis Radius.
     * @param ry Ellipse Arc Vertical Axis Radius.
     * @param angle1,angle2 Starting angle.
     * @param isClockwise Is the arc clockwise.
     * @since 1.0
     * @version 1.0
     */
    GraphicGeometryArc(float centerX, float centerY,
        float rx, float ry,
        float angle1, float angle2,
        bool isClockwise = true);
    /**
     * @brief Set as Initial Step.
     *
     * @since 1.0
     * @version 1.0
     */
    void Rewind(unsigned);

    /**
     * @brief Adjust approximation level correctly.
     * @param scale Is the ratio between viewport coordinates and logical coordinates.
     * @return void
     * @since 1.0
     * @version 1.0
     */
    void ApproximationScale(float scale);

    /**
     * @brief Get Approximation Level.
     * @param scale Is the ratio between viewport coordinates and logical coordinates.
     * @return void
     * @since 1.0
     * @version 1.0
     */
    float ApproximationScale() const
    {
        return expansionRatio_;
    }

    /**
     * @brief Called during the sampling phase.
     * @param x Pointer to the value of point coordinate X，y A pointer to the value of the point coordinate y
     * @return void
     * @since 1.0
     * @version 1.0
     */
    unsigned Vertex(float* y, float* x);
    /**
     * @brief Initialize an arc.
     *
     * @since 1.0
     * @version 1.0
     */
    void Init(float centerX, float centerY, float rx, float ry,
              float angle1, float angle2, bool isClockwise = true);

private:
    /**
     * @brief Normalized arcs.
     *
     * @since 1.0
     * @version 1.0
     */
    void Normalize(float angle1, float angle2, bool isClockwise);
    bool initialized_;      // Is it Init
    unsigned pathCommand_;  // Connection Command
    float centerX_;        // Center X-coordinate
    float centerY_;        // Center Y-coordinate
    float radiusX_;        // X Short semiaxis
    float radiusY_;        // Y Long axis
    float currentAngle_;   // Current Angle
    float beginAngle_;     // Starting angle
    float endAngle_;       // End angle
    float expansionRatio_; // Scaling ratio
    float deltaAngle_;     // Rate of radian change
    bool isClockwise_;      // direction
#endif
};
} // namespace OHOS

#endif
