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
* @brief Defines 裁剪相关方法.
*
* @since 1.0
* @version 1.0
*/

#ifndef GRAPHIC_GEOMETRY_CLIP_LIANG_BARSKY_INCLUDED
#define GRAPHIC_GEOMETRY_CLIP_LIANG_BARSKY_INCLUDED

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"

namespace OHOS {
const int CLIP_LINE_SEGMENT_FULL_VISIB = 0;// 完全可见
const int CLIP_LINE_SEGMENT_FULL_CLIP = 4;// 完全裁剪
enum ClippingFlagsEnum {
    CLIPPING_FLAGS_X1_CLIPPED = 4,
    CLIPPING_FLAGS_X2_CLIPPED = 1,
    CLIPPING_FLAGS_Y1_CLIPPED = 8,
    CLIPPING_FLAGS_Y2_CLIPPED = 2,
    CLIPPING_FLAGS_X_CLIPPED = CLIPPING_FLAGS_X1_CLIPPED | CLIPPING_FLAGS_X2_CLIPPED,
    CLIPPING_FLAGS_Y_CLIPPED = CLIPPING_FLAGS_Y1_CLIPPED | CLIPPING_FLAGS_Y2_CLIPPED
};

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
 * @brief 确定y的位置，纵向裁剪.
 * @param y 纵向位置,clipBox裁剪窗口
 * @return 返回对应的区域编码
 * @since 1.0
 * @version 1.0
 */
template <class T>
inline unsigned ClippingFlagsY(T y, const RectBase<T>& clipBox)
{
    return ((y < clipBox.y1) << 0x03) | ((y > clipBox.y2) << 0x01);
}

/**
 * @brief 确定x的位置，横向裁剪.
 * @param x 横向位置,clipBox裁剪窗口
 * @return 返回对应的区域编码
 * @since 1.0
 * @version 1.0
 */
template <class T>
inline unsigned ClippingFlagsX(T x, const RectBase<T>& clipBox)
{
    return ((x < clipBox.x1) << 0x02) | (x > clipBox.x2);
}

/**
 * @brief 根据顶点的位置确定顶点的剪裁.
 * @param x,y 顶点位置,clipBox裁剪窗口
 * @return 返回对应的区域编码
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
