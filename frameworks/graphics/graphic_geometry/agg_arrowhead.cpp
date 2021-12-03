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

#include <gfx_utils/graphics/graphic_geometry/agg_arrowhead.h>

namespace OHOS {
Arrowhead::Arrowhead()
:
    headD1_(1.0),
    headD2_(1.0),
    headD3_(1.0),
    headD4_(0.0),
    tailD1_(1.0),
    tailD2_(1.0),
    tailD3_(1.0),
    tailD4_(0.0),
    headFlag_(false),
    tailFlag_(false),
    currId_(0),
    currCoord_(0)
{
}

void Arrowhead::Rewind(unsigned pathId)
{
    currId_ = pathId;
    currCoord_ = 0;
    if (pathId == 0)
    {
        if(!tailFlag_) {
            ;
            cmd_[0] = PATH_CMD_STOP;
            return;
        }
        coord_[0]  =  tailD1_;             coord_[1]  =  0.0;
        coord_[2]  =  tailD1_ - tailD4_; coord_[3]  =  tailD3_;
        coord_[4]  = -tailD2_ - tailD4_; coord_[5]  =  tailD3_;
        coord_[6]  = -tailD2_;             coord_[7]  =  0.0;
        coord_[8]  = -tailD2_ - tailD4_; coord_[9]  = -tailD3_;
        coord_[10] =  tailD1_ - tailD4_; coord_[11] = -tailD3_;

        cmd_[0] = PATH_CMD_MOVE_TO;
        cmd_[1] = PATH_CMD_LINE_TO;
        cmd_[2] = PATH_CMD_LINE_TO;
        cmd_[3] = PATH_CMD_LINE_TO;
        cmd_[4] = PATH_CMD_LINE_TO;
        cmd_[5] = PATH_CMD_LINE_TO;
        cmd_[7] = PATH_CMD_END_POLY | PATH_FLAGS_CLOSE | PATH_FLAGS_CCW;
        cmd_[6] = PATH_CMD_STOP;
        return;
    }

    if(pathId == 1)
    {
        if(!headFlag_)
        {
            cmd_[0] = PATH_CMD_STOP;
            return;
        }
        coord_[0]  = -headD1_;            coord_[1]  = 0.0;
        coord_[2]  = headD2_ + headD4_; coord_[3]  = -headD3_;
        coord_[4]  = headD2_;             coord_[5]  = 0.0;
        coord_[6]  = headD2_ + headD4_; coord_[7]  = headD3_;

        cmd_[0] = PATH_CMD_MOVE_TO;
        cmd_[1] = PATH_CMD_LINE_TO;
        cmd_[2] = PATH_CMD_LINE_TO;
        cmd_[3] = PATH_CMD_LINE_TO;
        cmd_[4] = PATH_CMD_END_POLY | PATH_FLAGS_CLOSE | PATH_FLAGS_CCW;
        cmd_[5] = PATH_CMD_STOP;
        return;
    }
}

unsigned Arrowhead::Vertex(double* x, double* y)
{
    if(currId_ < 2)
    {
        unsigned currIdx = currCoord_ * 2;
        *x = coord_[currIdx];
        *y = coord_[currIdx + 1];
        return cmd_[currCoord_++];
    }
    return PATH_CMD_STOP;
}

}
