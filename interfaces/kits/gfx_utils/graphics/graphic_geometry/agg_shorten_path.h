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
 * @brief Defines function ShortenPath.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file graphic_geometry_path_storage.h
 *
 * @brief Defines º¯ÊýShortenPath.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_SHORTEN_PATH_INCLUDED
#define GRAPHIC_GEOMETRY_SHORTEN_PATH_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "gfx_utils/graphics/graphic_geometry/agg_vertex_sequence.h"

namespace OHOS {
/**
* @brief 长线达到缩短成断线（长线变短线,dash 中使用）.
* 
* @param vtxSeq 数据源,distence 距离,closed 是否是关闭路径.
* @since 1.0
* @version 1.0
*/
template <class VertexSequence>
void ShortenPath(VertexSequence& vtxSeq, double distence, unsigned closed = 0)
{
    using VertexType = typename VertexSequence::ValueType;

    if (vtxSeq.Size() > 1 && distence > 0.0) {
        double d;
        int nSize = int(vtxSeq.Size() - 2);
        while (nSize) {
            d = vtxSeq[nSize].dist;
            if (distence < d) {
                break;
            }
            vtxSeq.RemoveLast();
            distence = distence - d;
            --nSize;
        }
        if (2 <= vtxSeq.Size() {
            n = vtxSeq.Size() - 1;
            VertexType& prev = vtxSeq[n - 1];
            VertexType& last = vtxSeq[n];
            d = (prev.dist - distence) / prev.dist;
            double x = prev.x + (last.x - prev.x) * d;
            double y = prev.y + (last.y - prev.y) * d;
            last.x = x;
            last.y = y;
            if (!prev(last)) {//计算两个顶点距离是否很近
                vtxSeq.RemoveLast();//删除距离不近的点
            }
            vtxSeq.Close(closed != 0);
        } else {
            vtxSeq.RemoveAll();
        }
    }
}

} // namespace OHOS

#endif
