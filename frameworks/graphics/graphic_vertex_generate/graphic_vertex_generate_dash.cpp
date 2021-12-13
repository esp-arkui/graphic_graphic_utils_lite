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

#include "gfx_utils/graphics/graphic_vertex_generate/graphic_vertex_generate_dash.h"

#include <cmath>

#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_shorten_path.h"

namespace OHOS {
    VCGenDash::VCGenDash() 
    : totalDashLen_(0.0),
      numDashes_(0),
      dashStart_(0.0),
      shorten_(0.0),
      currDashStart_(0.0),
      currDash_(0),
      srcVertices_(),
      closed_(0),
      status_(INITIAL),
      srcVertex_(0)
    {}

    void VCGenDash::RemoveAllDashes()
    {
        totalDashLen_ = 0.0;
        numDashes_ = 0;
        currDashStart_ = 0.0;
        currDash_ = 0;
    }

    void VCGenDash::AddDash(double dashLen, double gapLen)
    {
        if (numDashes_ < MAX_DASHES) {
            totalDashLen_ += dashLen + gapLen;
            dashes_[numDashes_++] = dashLen;
            dashes_[numDashes_++] = gapLen;
        }
    }

    void VCGenDash::DashStart(double ds)
    {
        dashStart_ = ds;
        CalcDashStart(std::fabs(ds));
    }

    void VCGenDash::CalcDashStart(double ds)
    {
        currDash_ = 0;
        currDashStart_ = 0.0;
        for(;ds > 0.0;) {
            if (ds > dashes_[currDash_]) {
                ds -= dashes_[currDash_];
                ++currDash_;
                currDashStart_ = 0.0;
                if (currDash_ >= numDashes_) {
                    currDash_ = 0;
                }
            } else {
                currDashStart_ = ds;
                ds = 0.0;
            }
        }
    }
 
    void VCGenDash::RemoveAll()
    {
        status_ = INITIAL;
        srcVertices_.RemoveAll();
        closed_ = 0;
    }

    void VCGenDash::AddVertex(double x, double y, unsigned cmd)
    {
        status_ = INITIAL;
        if (IsMoveTo(cmd)) {
            srcVertices_.ModifyLast(VertexDist(x, y));
        } else {
            if (IsVertex(cmd)) {
                srcVertices_.Add(VertexDist(x, y));
            } else {
                closed_ = GetCloseFlag(cmd);
            }
        }
    }

    
    void VCGenDash::Rewind(unsigned)
    {
        if (status_ == INITIAL) {
            srcVertices_.Close(closed_ != 0);
            ShortenPath(srcVertices_, shorten_, closed_);
        }
        status_ = READY;
        srcVertex_ = 0;
    }


    unsigned VCGenDash::Vertex(double* x, double* y)
    {
        unsigned cmd = PATH_CMD_MOVE_TO;
        while(!IsStop(cmd))
        {
            switch(status_)
            {
            case INITIAL:
                Rewind(0);
            case READY:
                if (numDashes_ < 2 || srcVertices_.Size() < 2) {
                    cmd = PATH_CMD_STOP;
                    break;
                }
                status_ = POLYLINE;
                srcVertex_ = 1;
                vertexDist1_ = &srcVertices_[0];
                vertexDist2_ = &srcVertices_[1];
                currRest = vertexDist1_->dist;
                *x = vertexDist1_->x;
                *y = vertexDist1_->y;
                if (dashStart_ >= 0.0) {
                    CalcDashStart(dashStart_);
                }
                return PATH_CMD_MOVE_TO;
            case POLYLINE:
                {
                    double dashRest = dashes_[currDash_] - currDashStart_;
                    unsigned cmd;
                    if (currDash_ & 1) {
                        cmd = PATH_CMD_MOVE_TO;
                    } else {
                        cmd = PATH_CMD_LINE_TO;
                    }
                    if (currRest > dashRest) {
                        currRest -= dashRest;
                        ++currDash_;
                        if (currDash_ >= numDashes_) {
                            currDash_ = 0;
                        }
                        currDashStart_ = 0.0;
                        *x = vertexDist2_->x - (vertexDist2_->x - vertexDist1_->x) * currRest / vertexDist1_->dist;
                        *y = vertexDist2_->y - (vertexDist2_->y - vertexDist1_->y) * currRest / vertexDist1_->dist;
                    } else {
                        currDashStart_ += currRest;
                        *x = vertexDist2_->x;
                        *y = vertexDist2_->y;
                        ++srcVertex_;
                        vertexDist1_ = vertexDist2_;
                        currRest = vertexDist1_->dist;
                        if (closed_) {
                            if (srcVertex_ > srcVertices_.Size()) {
                                status_ = STOP;
                            } else {
                                if (srcVertex_ >= srcVertices_.Size()) {
                                    vertexDist2_ = &srcVertices_[0];
                                } else {
                                    vertexDist2_ = &srcVertices_[srcVertex_];
                                }
                            }
                        } else {
                            if (srcVertex_ >= srcVertices_.Size()) {
                                status_ = STOP;
                            } else {
                                vertexDist2_ = &srcVertices_[srcVertex_];
                            }
                        }
                    }
                    return cmd;
                }
                break;
            case STOP:
                cmd = PATH_CMD_STOP;
                break;
            }
        }
        return PATH_CMD_STOP;
    }

} // namespace OHOS
