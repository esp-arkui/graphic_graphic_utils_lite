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

#include <cmath>
#include <gfx_utils/graphics/graphic_geometry/agg_arc.h>

namespace OHOS{
Arc::Arc(double x,  double y, 
         double rx, double ry, 
         double a1, double a2, 
         bool ccw) :
    x_(x), y_(y), rx_(rx), ry_(ry), scale_(1.0)
{
    Normalize(a1, a2, ccw);
}
  
void Arc::ApproximationScale(double s)
{
    scale_ = s;
    if(initialized_)
    {
        Normalize(start_, end_, ccw_);
    }
}

unsigned Arc::Vertex(double* x, double* y)
{
    if (IsStop(pathCmd_)) {
        return PATH_CMD_STOP;
    }
    if ((angle_ < end_ - da_ / 4) != ccw_) {
        *x = x_ + std::cos(end_) * rx_;
        *y = y_ + std::sin(end_) * ry_;
        pathCmd_ = PATH_CMD_STOP;
        return PATH_CMD_LINE_TO;
    }

    *x = x_ + std::cos(angle_) * rx_;
    *y = y_ + std::sin(angle_) * ry_;

    angle_ += da_;

    unsigned pf = pathCmd_;
    pathCmd_ = PATH_CMD_LINE_TO;
    return pf;
}

void Arc::Rewind(unsigned)
{
    pathCmd_ = PATH_CMD_MOVE_TO; 
    angle_ = start_;
}

void Arc::Normalize(double a1, double a2, bool ccw)
{
    double ra = (std::fabs(rx_) + std::fabs(ry_)) / 2;
    da_ = std::acos(ra / (ra + 0.125 / scale_)) * 2;
    if(ccw)
    {
        while (a2 < a1) {
            a2 += pi * 2.0;
        }
    }
    else
    {
        while (a1 < a2) {
            a1 += pi * 2.0;
        }
        da_ = -da_;
    }
    ccw_   = ccw;
    start_ = a1;
    end_   = a2;
    initialized_ = true;
}

void Arc::Init(double x, double y, double rx, double ry, double a1, double a2, bool ccw)
{
    x_ = x;
    y_ = y;
    rx_ = rx;
    ry_ = ry;
    Normalize(a1, a2, ccw);
}
}
