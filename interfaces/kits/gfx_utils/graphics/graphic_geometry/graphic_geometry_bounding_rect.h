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
 * @file graphic_geometry_bounding_rect.h
 *
 * @brief 计算轮廓的外接矩形
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_BOUNDING_RECT_INCLUDED
#define GRAPHIC_GEOMETRY_BOUNDING_RECT_INCLUDED

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"

namespace OHOS {
    /**
     * @brief 计算外接矩形
     * @param VertexSource是顶点序列，CoordT为顶点坐标，pathId为路径的ID
     * @since 1.0
     * @version 1.0
     */
    template <class VertexSource, class CoordT>
    bool BoundingRectSingle(VertexSource& vertexSource, unsigned pathId,
                            CoordT* x1, CoordT* y1, CoordT* x2, CoordT* y2)
    {
        double x;
        double y;
        bool first = true;

        *x1 = CoordT(1);
        *y1 = CoordT(1);
        *x2 = CoordT(0);
        *y2 = CoordT(0);

        vertexSource.Rewind(pathId);
        unsigned currentCommand;
        while (!IsStop(currentCommand = vertexSource.Vertex(&x, &y))) {
            if (IsVertex(currentCommand)) {
                if (first) {
                    *x1 = CoordT(x);
                    *y1 = CoordT(y);
                    *x2 = CoordT(x);
                    *y2 = CoordT(y);
                    first = false;
                } else {
                    if (CoordT(x) < *x1) {
                        *x1 = CoordT(x);
                    }
                    if (CoordT(y) < *y1) {
                        *y1 = CoordT(y);
                    }
                    if (CoordT(x) > *x2) {
                        *x2 = CoordT(x);
                    }
                    if (CoordT(y) > *y2) {
                        *y2 = CoordT(y);
                    }
                }
            }
        }
        return *x1 <= *x2 && *y1 <= *y2;
    }
} // namespace OHOS

#endif
