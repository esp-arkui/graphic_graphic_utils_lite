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
 * @brief 数学常用库函数
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
     * @brief 计算向量叉积
     * @param x1,x2,x为向量点的x坐标，y1,y2,y为向量点的y坐标
     * @return 返回值大于0，为逆时针方向；返回值小于0，为顺时针方向；返回值等于0，为共线
     * @since 1.0
     * @version 1.0
     */
    GRAPHIC_GEOMETRY_INLINE float CrossProduct(float x1, float y1, float x2, float y2, float x, float y)
    {
        return (x - x2) * (y2 - y1) - (y - y2) * (x2 - x1);
    }

    /**
     * @brief 计算两点距离
     * @param x1,x2为点的x坐标，y1,y2为点的y坐标
     * @return 返回值为两点的距离
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
     * @brief 计算两点平方
     * @param x1,x2为点的x坐标，y1,y2为点的y坐标
     * @return 返回值为两点平方值
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
     * @brief 计算两条线段是否相交，并求出交点
     * @param aX,bX,cX,dX为点的x坐标，aY,bY,cY,dY为点的y坐标
     * @param x为相交点的x坐标指针，y为相交点的y坐标指针
     * @return 返回值true为相交，false为不相交
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
     * @brief 计算多边形面积
     * @param Storage为多边形顶点序列
     * @return 返回值为多边形面积
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
