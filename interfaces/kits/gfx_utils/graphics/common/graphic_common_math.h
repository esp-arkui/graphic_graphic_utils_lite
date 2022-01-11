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
 * @file graphic_geometry_math.h
 *
 * @brief Common library functions in Mathematics
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_MATH_INCLUDED
#define GRAPHIC_GEOMETRY_MATH_INCLUDED

#include <cmath>

#include "gfx_utils/graphics/common/graphic_common_basics.h"

namespace OHOS {
    /**
     * @brief Calculate vector cross product
     * @param X1, X2, X are the X coordinates of the vector point, Y1, Y2, y are the Y coordinates of the vector point
     * @return If the return value is greater than 0, it is counterclockwise; If the return value is less than 0,
     * it is clockwise; If the return value is equal to 0, it is collinear
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE float CrossProduct(float x1, float y1, float x2, float y2, float x, float y)
    {
        return (x - x2) * (y2 - y1) - (y - y2) * (x2 - x1);
    }

    /**
     * @brief Calculate the distance between two points
     * @param X1 and X2 are the X coordinates of the point, Y1 and Y2 are the Y coordinates of the point
     * @return The return value is the distance between two points
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE float CalcDistance(float x1, float y1, float x2, float y2)
    {
        float dx = x2 - x1;
        float dy = y2 - y1;
        return std::sqrt(dx * dx + dy * dy);
    }

    /**
     * @brief Calculate the square of two points
     * @param X1 and X2 are the X coordinates of the point, Y1 and Y2 are the Y coordinates of the point
     * @return The return value is the square of two points
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE float CalcSqDistance(float x1, float y1, float x2, float y2)
    {
        float dx = x2 - x1;
        float dy = y2 - y1;
        return dx * dx + dy * dy;
    }

    /**
     * @brief Calculate whether the two line segments intersect and find the intersection
     * @param aX,bX,cX,dX are the X coordinates of the point, and ay, by, cy and dy are the Y coordinates of the point
     * @param X is the X coordinate pointer of the intersection point,
     * and Y is the Y coordinate pointer of the intersection point
     * @return The return value true is intersecting and false is disjoint
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE bool CalcIntersection(float aX, float aY, float bX, float bY,
                                                  float cX, float cY, float dX, float dY,
                                                  float* x, float* y)
    {
        float num = (aY - cY) * (dX - cX) - (aX - cX) * (dY - cY);
        float den = (bX - aX) * (dY - cY) - (bY - aY) * (dX - cX);
        if (std::fabs(den) < INTERSECTIONEPSILON) {
            return false;
        }
        float r = num / den;
        *x = aX + r * (bX - aX);
        *y = aY + r * (bY - aY);
        return true;
    }

    /**
     * @brief Calculate polygon area
     * @param Storage is a sequence of polygon vertices
     * @return The return value is polygon area
     * @since 1.0
     * @version 1.0
     */
    template <class Storage>
    float CalcPolygonArea(const Storage& storageVertex)
    {
        unsigned i;
        float sum = 0.0;
        float x = storageVertex[0].x;
        float y = storageVertex[0].y;
        float xs = x;
        float ys = y;

        for (i = 1; i < storageVertex.size(); i++) {
            const typename Storage::value_type& valueType = storageVertex[i];
            sum += x * valueType.y - y * valueType.x;
            x = valueType.x;
            y = valueType.y;
        }
        return (sum + x * ys - y * xs) * ALPHAHALF;
    }
} // namespace OHOS

#endif
