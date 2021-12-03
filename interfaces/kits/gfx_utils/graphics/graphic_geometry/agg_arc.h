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
 * @brief Defines color types gray8, gray16.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file graphic_geometry_color_gray.h
 *
 * @brief Defines xxxxxxxxxxxxxxxxxxxxx.
 *
 * @since 1.0
 * @version 1.0
 */
#ifndef GRAPHIC_GEOMETRY_ARC_INCLUDED
#define GRAPHIC_GEOMETRY_ARC_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"

namespace OHOS {
class Arc : public HeapBase {
public:
    Arc() : scale_(1.0), initialized_(false) {}

    Arc(double x, double y, 
        double rx, double ry, 
        double a1, double a2, 
        bool ccw=true);
        
    void Rewind(unsigned);

    void ApproximationScale(double s);

    double ApproximationScale() const 
    { 
        return scale_;  
    }

    unsigned Vertex(double* x, double* y);

    void Init(double x, double y, double rx, double ry, double a1, double a2, bool ccw = true);

private:
    void Normalize(double a1, double a2, bool ccw);
    bool initialized_;
    unsigned pathCmd_;
    double   x_;
    double   y_;
    double   rx_;
    double   ry_;
    double   angle_;
    double   start_;
    double   end_;
    double   scale_;
    double   da_;
    bool     ccw_;

};


}


#endif
