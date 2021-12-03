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
 * @brief Defines ����ShortenPath.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_SHORTEN_PATH_INCLUDED
#define GRAPHIC_GEOMETRY_SHORTEN_PATH_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "gfx_utils/graphics/graphic_geometry/agg_vertex_sequence.h"

namespace OHOS{
template<class VertexSequence> 
void ShortenPath(VertexSequence& vs, double s, unsigned closed = 0)
{
    using  VertexType = typename VertexSequence::ValueType;

    if(s > 0.0 && vs.Size() > 1)
    {
        double d;
        int n = int(vs.Size() - 2);
        while(n)
        {
            d = vs[n].dist;
            if(d > s) break;
            vs.RemoveLast();
            s -= d;
            --n;
        }
        if (vs.Size() < 2)
        {
            vs.RemoveAll();
        }
        else
        {
            n = vs.Size() - 1;
            VertexType& prev = vs[n - 1];
            VertexType& last = vs[n];
            d = (prev.dist - s) / prev.dist;
            double x = prev.x + (last.x - prev.x) * d;
            double y = prev.y + (last.y - prev.y) * d;
            last.x = x;
            last.y = y;
            if (!prev(last)) {
                vs.RemoveLast();
            }
            vs.Close(closed != 0);
        }
    }
}


}

#endif
