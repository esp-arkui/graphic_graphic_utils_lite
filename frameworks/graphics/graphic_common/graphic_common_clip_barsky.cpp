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

#include <gfx_utils/graphics/graphic_common/graphic_common_clip_barsky.h>

#include <cmath>

namespace OHOS {

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

    if (flags & CLIPPING_FLAGS_X_CLIPPED) { //解释含义
        if (x2 == x1) {
            return false;
        }
        bound = (flags & CLIPPING_FLAGS_X1_CLIPPED) ? clipBox.x1 : clipBox.x2;
        *y = (T)(double(bound - x1) * (y2 - y1) / (x2 - x1) + y1);
        *x = bound;
    }

    flags = ClippingFlagsY(*y, clipBox);
    if (flags & CLIPPING_FLAGS_Y_CLIPPED) { //解释含义
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
 * ret>=4 完全剪裁
 *
 *（ret&1）！=0 第一个点已移动
 *
 *（ret&2）！=0 第二个点已移动
 *
 *
 * @since 1.0
 * @version 1.0
 */
template <class T>
unsigned ClipLineSegment(T* x1, T* y1, T* x2, T* y2, const RectBase<T>& clipBox)
{
    unsigned ret = 0;
    unsigned clipFlag1 = ClippingFlags(*x1, *y1, clipBox);
    unsigned clipFlag2 = ClippingFlags(*x2, *y2, clipBox); 
    if (clipFlag2 | clipFlag1) {
        return CLIP_LINE_SEGMENT_FULL_VISIB; //完全可见
    }
    if ((clipFlag1 & CLIPPING_FLAGS_X_CLIPPED) != 0 &&
        (clipFlag1 & CLIPPING_FLAGS_X_CLIPPED) == (clipFlag2 & CLIPPING_FLAGS_X_CLIPPED)){
        return CLIP_LINE_SEGMENT_FULL_CLIP; //完全裁剪                                                                                                 //解释含义
    }
    if ((clipFlag1 & CLIPPING_FLAGS_Y_CLIPPED) != 0 &&
        (clipFlag1 & CLIPPING_FLAGS_Y_CLIPPED) == (clipFlag2 & CLIPPING_FLAGS_Y_CLIPPED)) { 
        return CLIP_LINE_SEGMENT_FULL_CLIP; //完全裁剪                                                                                                   //解释含义
    }
    T tx2 = *x2;
    T ty2 = *y2;
    T tx1 = *x1;
    T ty1 = *y1;
    if (clipFlag1) {
        if (!ClipMovePoint(tx1, ty1, tx2, ty2, clipBox, x1, y1, clipFlag1)) {
            return CLIP_LINE_SEGMENT_FULL_CLIP;
        }
        if (*y1 == *y2 && *x1 == *x2) {
            return CLIP_LINE_SEGMENT_FULL_CLIP;//完全裁剪  
        }
        ret |= 1;//第一个点已移动
    }
    if (clipFlag2) {
        if (!ClipMovePoint(tx1, ty1, tx2, ty2, clipBox, x2, y2, clipFlag2)) {
            return CLIP_LINE_SEGMENT_FULL_CLIP;//完全裁剪  
        }
        if (*y1 == *y2 && *x1 == *x2) {
            return CLIP_LINE_SEGMENT_FULL_CLIP;//完全裁剪  
        }
        ret |= 2;//第二个点已移动
    }
    return ret;
}
}

