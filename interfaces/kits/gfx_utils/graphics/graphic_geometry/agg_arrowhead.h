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
 * @brief Defines Arrowhead.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file graphic_geometry_arrowhead.h
 *
 * @brief Defines 箭头形状结构类.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_ARROWHEAD_INCLUDED
#define GRAPHIC_GEOMETRY_ARROWHEAD_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"

namespace OHOS {
class Arrowhead : public HeapBase{
public:
    Arrowhead();

    void NoHead()
    {
        headFlag_ = false;
    }

    void Head(double d1, double d2, double d3, double d4)
    {
        headFlag_ = true;
        headD1_ = d1;
        headD2_ = d2;
        headD3_ = d3;
        headD4_ = d4;
       
    }

    void Head()
    {
        headFlag_ = true;
    }

    void Tail(double d1, double d2, double d3, double d4)
    {
        tailFlag_ = true;
        tailD1_ = d1;
        tailD2_ = d2;
        tailD3_ = d3;
        tailD4_ = d4;
       
    }

    void Tail()    
    { 
        tailFlag_ = true; 
    }

    void NoTail() 
    {
        tailFlag_ = false;
    }

    void Rewind(unsigned pathId);

    unsigned Vertex(double* x, double* y);

private:
    double   headD1_;
    double   headD2_;
    double   headD3_;
    double   headD4_;
    double   tailD1_;
    double   tailD2_;
    double   tailD3_;
    double   tailD4_;
    bool     headFlag_;
    bool     tailFlag_;
    double   coord_[16];
    unsigned cmd_[8];
    unsigned currId_;
    unsigned currCoord_;
};

}

#endif
