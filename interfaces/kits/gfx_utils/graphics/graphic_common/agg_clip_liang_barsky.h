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
* @brief Defines ClippingFlagsE, ClipLiangBarsky, ClipMovePoint, ClipLineSegment.
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

#include "gfx_utils/graphics/graphic_common/agg_basics.h"

namespace OHOS {
enum ClippingFlagsEnum
{
    CLIPPING_FLAGS_X1_CLIPPED = 4,
    CLIPPING_FLAGS_X2_CLIPPED = 1,
    CLIPPING_FLAGS_Y1_CLIPPED = 8,
    CLIPPING_FLAGS_Y2_CLIPPED = 2,
    CLIPPING_FLAGS_X_CLIPPED = CLIPPING_FLAGS_X1_CLIPPED | CLIPPING_FLAGS_X2_CLIPPED,
    CLIPPING_FLAGS_Y_CLIPPED = CLIPPING_FLAGS_Y1_CLIPPED | CLIPPING_FLAGS_Y2_CLIPPED
};

/**
 * @brief 确定y的位置，纵向裁剪.
 * @param y 纵向位置,clipBox裁剪窗口
 * @return 返回对应的区域编码
 * @since 1.0
 * @version 1.0
 */
template <class T>
inline unsigned ClippingFlagsY(T y, const RectBase<T>& clipBox)
{
    return ((y < clipBox.y1) << 3) | ((y > clipBox.y2) << 1);
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
    return ((x < clipBox.x1) << 2) | (x > clipBox.x2);
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
    return ((x < clipBox.x1) << 2) | ((y < clipBox.y1) << 3) | (x > clipBox.x2) | ((y > clipBox.y2) << 1);
}

#define CLIP_LINAGE_BARSKY_VAL_DEF  \
    double tinY;                    \
    double toutX;                   \
    double toutY;                   \
    double tin1;                    \
    double tin2;                    \
    double tout1;                   \
    double xin;                     \
    double xout;                    \
    double yin;                     \
    double yout;                    \
    double tinX;                    \
    unsigned np = 0;                \
    const double limitZero = 1e-30; \
    double deltaX = x2 - x1;        \
    double deltaY = y2 - y1;

/**
 * @brief 确定顶点的位置，按点裁剪.
 * @param x1,y1 线段起点位置,x2,y2 线段结束位置,clipBox裁剪窗口,x,y 返回结果
 * @return 返回对应的区域编码
 * @since 1.0
 * @version 1.0
 */
template <class T>
inline unsigned ClipLiangBarsky(T x1, T y1, T x2, T y2, const RectBase<T>& clipBox,
                                T* x, T* y)
{
    CLIP_LINAGE_BARSKY_VAL_DEF
    if (deltaX == 0.0) {
        deltaX = (x1 > clipBox.x1) ? -limitZero : limitZero;
    }
    if (deltaY == 0.0) {
        deltaY = (y1 > clipBox.y1) ? -limitZero : limitZero;
    }
    if (deltaX > 0.0) {
        xin = clipBox.x1;
        xout = clipBox.x2;
    } else {
        xin = clipBox.x2;
        xout = clipBox.x1;
    }
    if (deltaY > 0.0) {
        yin = clipBox.y1;
        yout = clipBox.y2;
    } else {
        yin = clipBox.y2;
        yout = clipBox.y1;
    }
    tinX = (xin - x1) / deltaX;
    tinY = (yin - y1) / deltaY;
    if (tinX < tinY) {
        tin1 = tinX;
        tin2 = tinY;
    } else {
        tin1 = tinY;
        tin2 = tinX;
    }
    if (tin1 <= 1.0) {
        if (tin1 > 0.0) {
            *x++ = (T)xin;
            *y++ = (T)yin;
            ++np;
        }
        if (tin2 <= 1.0) {
            toutX = (xout - x1) / deltaX;
            toutY = (yout - y1) / deltaY;
            tout1 = (toutX < toutY) ? toutX : toutY;
            if (tout1 > 0.0 || tin2 > 0.0) {
                if (tin2 <= tout1) {
                    if (tin2 > 0.0) {
                        if (tinX > tinY) {
                            *x++ = (T)xin;
                            *y++ = (T)(y1 + tinX * deltaY);
                        } else {
                            *x++ = (T)(x1 + tinY * deltaX);
                            *y++ = (T)yin;
                        }
                        ++np;
                    }
                    if (tout1 < 1.0) {
                        if (toutX < toutY) {
                            *x++ = (T)xout;
                            *y++ = (T)(y1 + toutX * deltaY);
                        } else {
                            *x++ = (T)(x1 + toutY * deltaX);
                            *y++ = (T)yout;
                        }
                    } else {
                        *x++ = x2;
                        *y++ = y2;
                    }
                    ++np;
                } else {
                    if (tinX < tinY) {
                        *x++ = (T)xout;
                        *y++ = (T)yin;
                    } else {
                        *x++ = (T)xin;
                        *y++ = (T)yout;
                    }
                    ++np;
                }
            }
        }
    }
    return np;
}

/**
 * @brief 裁剪移动的点.
 * @param x1,y1 起点位置,x2,y2 终点位置,clipBox裁剪窗口,x,y 返回结果
 * @return 返回对应的区域编码
 * @since 1.0
 * @version 1.0
 */
template <class T>
bool ClipMovePoint(T x1, T y1, T x2, T y2, const RectBase<T>& clipBox, T* x, T* y, unsigned flags)
{
    T bound;

    if (flags & CLIPPING_FLAGS_X_CLIPPED) {
        if (x2 == x1) {
            return false;
        }
        bound = (flags & CLIPPING_FLAGS_X1_CLIPPED) ? clipBox.x1 : clipBox.x2;
        *y = (T)(double(bound - x1) * (y2 - y1) / (x2 - x1) + y1);
        *x = bound;
    }

    flags = ClippingFlagsY(*y, clipBox);
    if (flags & CLIPPING_FLAGS_Y_CLIPPED) {
        if (y2 == y1) {
            return false;
        }
        bound = (flags & CLIPPING_FLAGS_Y1_CLIPPED) ? clipBox.y1 : clipBox.y2;
        *x = (T)(double(bound - y1) * (x2 - x1) / (y2 - y1) + x1);
        *y = bound;
    }
    return true;
}

/**
 * @brief 线段裁剪.
 * @param x1,y1 线段起点位置,x2,y2 线段结束位置,clipBox裁剪窗口.
 * @return 返回对应的区域编码与裁剪后的线段起始位置.
 * @since 1.0
 * @version 1.0
 */
template <class T>
unsigned ClipLineSegment(T* x1, T* y1, T* x2, T* y2, const RectBase<T>& clipBox)
{
    unsigned ret = 0;
    unsigned f2 = ClippingFlags(*x2, *y2, clipBox);
    unsigned f1 = ClippingFlags(*x1, *y1, clipBox);
    if (0 == (f2 | f1)) {
        return 0;
    }
    if ((f1 & CLIPPING_FLAGS_X_CLIPPED) != 0 && (f1 & CLIPPING_FLAGS_X_CLIPPED) == (f2 & CLIPPING_FLAGS_X_CLIPPED)) {
        return 4;
    }
    if ((f1 & CLIPPING_FLAGS_Y_CLIPPED) != 0 && (f1 & CLIPPING_FLAGS_Y_CLIPPED) == (f2 & CLIPPING_FLAGS_Y_CLIPPED)) {
        return 4;
    }
    T tx2 = *x2;
    T ty2 = *y2;
    T tx1 = *x1;
    T ty1 = *y1;
    if (f1) {
        if (!ClipMovePoint(tx1, ty1, tx2, ty2, clipBox, x1, y1, f1)) {
            return 4;
        }
        if (*y1 == *y2 && *x1 == *x2) {
            return 4;
        }
        ret |= 1;
    }
    if (f2) {
        if (!ClipMovePoint(tx1, ty1, tx2, ty2, clipBox, x2, y2, f2)) {
            return 4;
        }
        if (*y1 == *y2 && *x1 == *x2) {
            return 4;
        }
        ret |= 2;
    }
    return ret;
}

} // namespace OHOS

#endif
