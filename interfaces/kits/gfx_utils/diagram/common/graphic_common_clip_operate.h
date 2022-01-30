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
 * @brief Defines ClippingFlagsE, ClipMovePoint, ClipLineSegment.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file graphic_geometry_clip_liang_barsky.h
 *
 * @brief Defines Cutting related methods
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_CLIP_LIANG_BARSKY_INCLUDED
#define GRAPHIC_GEOMETRY_CLIP_LIANG_BARSKY_INCLUDED

#include "gfx_utils/diagram/common/graphic_common_basics.h"

namespace OHOS {
const int CLIP_LINE_SEGMENT_FULL_VISIB = 0; // Fully visible
const int CLIP_LINE_SEGMENT_FULL_CLIP = 4;  // Full clipping
/**
 *   0110  |  0010  | 0011
 *         |        |
 *  -------+--------+-------- clipBox.y2
 *         |        |
 *   0100  |  0000  | 0001
 *         |        |
 *  -------+--------+-------- clipBox.y1
 *         |        |
 *   1100  |  1000  | 1001
 *         |        |
 * clipBox.x1 clipBox.x2
 * @brief Determine the position of Y and cut it longitudinally
 * @param Y vertical position, Clipbox clipping window
 * @return Return the corresponding area code
 * @since 1.0
 * @version 1.0
 */
template <class T>
inline unsigned ClippingFlagsY(T y, const RectBase<T>& clipBox)
{
    return ((y < clipBox.y1) << 0x03) | ((y > clipBox.y2) << 0x01);
}

/**
 * @brief Determine the position of X and cut horizontally
 * @param X horizontal position, Clipbox clipping window
 * @return Return the corresponding area code
 * @since 1.0
 * @version 1.0
 */
template <class T>
inline unsigned ClippingFlagsX(T x, const RectBase<T>& clipBox)
{
    return ((x < clipBox.x1) << 0x02) | (x > clipBox.x2);
}

/**
 * @brief The clipping of vertices is determined according to the position of vertices
 * @param x. y vertex position, Clipbox clipping window
 * @return Return the corresponding area code
 * @since 1.0
 * @version 1.0
 */
template <class T>
inline unsigned ClippingFlags(T x, T y, const RectBase<T>& clipBox)
{
    return ((x < clipBox.x1) << 0x02) | ((y < clipBox.y1) << 0x03) | (x > clipBox.x2) | ((y > clipBox.y2) << 0x01);
}
} // namespace OHOS
#endif
