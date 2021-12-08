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

#include <gfx_utils/graphics/graphic_geometry/agg_arc.h>

#include <cmath>

namespace OHOS {
Arc::Arc(double centerX, double centerY,
         double rx, double ry,
         double a1, double a2,
         bool ccw)
    : centerX_(centerX), centerY_(centerY), rx_(rx), ry_(ry), scale_(1.0)
{
    Normalize(a1, a2, ccw);
}

void Arc::ApproximationScale(double sale)
{
    scale_ = sale;
    if (initialized_) {
        Normalize(start_, end_, ccw_);
    }
}

unsigned Arc::Vertex(double* x, double* y)
{
    if (IsStop(pathCmd_)) {
        return PATH_CMD_STOP;
    }
    if ((angle_ < end_ - da_ / 4) != ccw_) {
        *x = centerX_ + std::cos(end_) * rx_;
        *y = centerY_ + std::sin(end_) * ry_;
        pathCmd_ = PATH_CMD_STOP;
        return PATH_CMD_LINE_TO;
    }

    *x = centerX_ + std::cos(angle_) * rx_;
    *y = centerY_ + std::sin(angle_) * ry_;

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

void Arc::Normalize(double angle1, double angle2, bool ccw)
{
    double ra = (std::fabs(rx_) + std::fabs(ry_)) / 2;
    da_ = std::acos(ra / (ra + 0.125 / scale_)) * 2;
    if (ccw) {
        while (angle2 < angle1) {
            angle2 += PI * 2.0;
        }
    } else {
        while (angle1 < angle2) {
            angle1 += PI * 2.0;
        }
        da_ = -da_;
    }
    ccw_ = ccw;
    start_ = angle1;
    end_ = angle2;
    initialized_ = true;
}

void Arc::Init(double centerX, double centerY, double rx, double ry, double angle1, double angle2, bool ccw)
{
    centerX_ = centerX;
    centerY_ = centerY;
    rx_ = rx;
    ry_ = ry;
    Normalize(angle1, angle2, ccw);
}
} // namespace OHOS
