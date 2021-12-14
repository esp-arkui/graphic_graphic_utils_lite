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


#ifndef GRAPHIC_GEOMETRY_SHORTEN_PATH_INCLUDED
#define GRAPHIC_GEOMETRY_SHORTEN_PATH_INCLUDED

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"
#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_vertex_sequence.h"

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
            int nSize = int(vtxSeq.Size() - TWO_STEP);
            while (nSize) {
                d = vtxSeq[nSize].dist_;
                if (distence < d) {
                    break;
                }
                vtxSeq.RemoveLast();
                distence = distence - d;
                --nSize;
            }
            if (vtxSeq.Size() > 1) {
                nSize = vtxSeq.Size() - 1;
                VertexType& prev = vtxSeq[nSize - 1];
                VertexType& last = vtxSeq[nSize];
                d = (prev.dist_ - distence) / prev.dist_;
                double x = prev.x_ + (last.x_ - prev.x_) * d;
                double y = prev.y_ + (last.y_ - prev.y_) * d;
                last.x_ = x;
                last.y_ = y;
                if (!prev(last)) {       //计算两个顶点距离是否很近
                    vtxSeq.RemoveLast(); //删除距离不近的点
                }
                vtxSeq.Close(closed != 0);
            } else {
                vtxSeq.RemoveAll();
            }
        }
    }

} // namespace OHOS

#endif
