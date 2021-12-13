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

#include <gfx_utils/graphics/graphic_geometry/graphic_geometry_arc.h>

#include <cmath>

namespace OHOS {
Arc::Arc(double centerX, double centerY,
         double rx, double ry,
         double start_, double end_,
         bool isClockwise)
    : centerX_(centerX), centerY_(centerY), rx_(rx), ry_(ry), scale_(1.0)
{
    Normalize(start_, end_, isClockwise);
}

void Arc::ApproximationScale(double sale)
{
    scale_ = sale;
    if (initialized_) {
        Normalize(start_, end_, isClockwise_);
    }
}

unsigned Arc::Vertex(double* x, double* y)
{
    //当前命令是结束点没有顶点
    if (IsStop(pathCmd_)) {
        return PATH_CMD_STOP;
    }
    if ((angle_ < end_ - delatAngle_ / 4) != isClockwise_) {
        *x = centerX_ + std::cos(end_) * rx_;
        *y = centerY_ + std::sin(end_) * ry_;
        pathCmd_ = PATH_CMD_STOP;
        return PATH_CMD_LINE_TO;
    }

    *x = centerX_ + std::cos(angle_) * rx_;
    *y = centerY_ + std::sin(angle_) * ry_;

    angle_ += delatAngle_;

    unsigned pf = pathCmd_;
    pathCmd_ = PATH_CMD_LINE_TO;
    return pf;
}

void Arc::Rewind(unsigned)
{
    pathCmd_ = PATH_CMD_MOVE_TO;
    angle_ = start_;
}

void Arc::Normalize(double startAngle, double endAngle, bool isClockwise)
{   
    const double aa = 0.125;
    double ra = (std::fabs(rx_) + std::fabs(ry_)) / 2;
    delatAngle_ = std::acos(ra / (ra + 0.125 / scale_)) * 2;//计算出弧度变化率
    if (isClockwise) {
        while (endAngle < startAngle) {
            endAngle += PI * 2.0;
        }
    } else {
        while (startAngle < endAngle) {
            startAngle += PI * 2.0;
        }
        delatAngle_ = -delatAngle_;
    }
    isClockwise_ = isClockwise;
    start_ = startAngle;
    end_ = endAngle;
    initialized_ = true;
}

void Arc::Init(double centerX, double centerY, double rx, double ry, double startAngle, double endAngle, bool isClockwise)
{
    centerX_ = centerX;
    centerY_ = centerY;
    rx_ = rx;
    ry_ = ry;
    Normalize(startAngle, endAngle, isClockwise);
}
} // namespace OHOS
