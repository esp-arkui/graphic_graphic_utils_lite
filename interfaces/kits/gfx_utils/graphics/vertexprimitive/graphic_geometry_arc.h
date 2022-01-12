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

#include "gfx_utils/graphics/common/graphic_common_basics.h"
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
    class Arc : public HeapBase {
#if GRAPHIC_GEOMETYR_ENABLE_ARC_VERTEX_SOURCE
    public:
        Arc() : mInitialized(false), mExpansionRatio(1.0)
        {
        }

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
        Arc(float centerX, float centerY,
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
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        void ApproximationScale(float scale);

        /**
         * @brief Get Approximation Level.
         * @param scale Is the ratio between viewport coordinates and logical coordinates.
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        float ApproximationScale() const
        {
            return mExpansionRatio;
        }

        /**
         * @brief Called during the sampling phase.
         * @param x Pointer to the value of point coordinate X，y A pointer to the value of the point coordinate y
         * @return 无
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
        bool mInitialized;      // Is it Init
        unsigned mPathCommand;  // Connection Command
        float mCenterX;        // Center X-coordinate
        float mCenterY;        // Center Y-coordinate
        float mRadiusX;        // X Short semiaxis
        float mRadiusY;        // Y Long axis
        float mCurrentAngle;   // Current Angle
        float mBeginAngle;     // Starting angle
        float mEndAngle;       // End angle
        float mExpansionRatio; // Scaling ratio
        float mDeltaAngle;     // Rate of radian change
        bool mIsClockwise;      // direction
#endif
    };
} // namespace OHOS

#endif
