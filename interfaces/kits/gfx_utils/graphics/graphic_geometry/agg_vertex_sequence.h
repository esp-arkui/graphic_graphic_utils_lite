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
 * @brief Defines VertexSequence VertexDist VertexDistCmd.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file graphic_geometry_vertex_sequence.h
 *
 * @brief Defines ∂®“ÂVertexSequence¿‡.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_VERTEX_SEQUENCE_INCLUDED
#define GRAPHIC_GEOMETRY_VERTEX_SEQUENCE_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "gfx_utils/graphics/graphic_geometry/agg_array.h"
#include "gfx_utils/graphics/graphic_common/agg_math.h"

namespace OHOS{
template<class T, unsigned S=6> 
class VertexSequence : public PodBvector<T, S> {
public:
    using BaseYype = PodBvector<T, S>;

    void Close(bool remove_flag);

    void Add(const T& val);

    void ModifyLast(const T& val);
};

template <class T, unsigned S> 
void VertexSequence<T, S>::Close(bool closed)
{
    while (BaseType::Size() > 1) {
        if ((*this)[BaseType::Size() - 2]((*this)[BaseType::Size() - 1]))
            break;
        T t = (*this)[BaseType::Size() - 1];
        BaseType::remove_last();
        ModifyLast(t);
    }

    if (closed) {
        while (BaseType::Size() > 1) {
            if ((*this)[BaseType::Size() - 1]((*this)[0]))
                break;
            BaseType::RemoveLast();
        }
    }
}

template<class T, unsigned S> 
void VertexSequence<T, S>::Add(const T& val)
{
    if (BaseYype::Size() > 1)
    {
        if (!(*this)[BaseYype::Size() - 2]((*this)[BaseYype::Size() - 1])) 
        {
            BaseYype::RemoveLast();
        }
    }
    BaseYype::Add(val);
}

template<class T, unsigned S> 
void VertexSequence<T, S>::ModifyLast(const T& val)
{
    BaseType::RemoveLast();
    Add(val);
}

struct VertexDist{
    double   x;
    double   y;
    double   dist;

    VertexDist() {}
    VertexDist(double x_, double y_) :
        x(x_),
        y(y_),
        dist(0.0)
    {
    }

    bool operator()(const VertexDist& val)
    {
        bool ret = (dist = CalcDistance(x, y, val.x, val.y)) > VERTEX_DIST_EPSILON;
        if(!ret) dist = 1.0 / VERTEX_DIST_EPSILON;
        return ret;
    }
};

struct VertexDistCmd : public VertexDist{
    unsigned cmd;

    VertexDistCmd() {}
    VertexDistCmd(double x_, double y_, unsigned cmd_) : VertexDist(x_, y_),
        cmd(cmd_)
    {
    }
};


}

#endif
