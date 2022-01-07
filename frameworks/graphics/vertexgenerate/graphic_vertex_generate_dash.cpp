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

#include "gfx_utils/graphics/vertexgenerate/graphic_vertex_generate_dash.h"

#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_shorten_path.h"

namespace OHOS {
#if GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE
    VertexGenerateDash::VertexGenerateDash() :
        totalDashLen_(0.0),
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

    void VertexGenerateDash::RemoveAllDashes()
    {
        totalDashLen_ = 0.0;
        numDashes_ = 0;
        currDashStart_ = 0.0;
        currDash_ = 0;
    }

    void VertexGenerateDash::AddDash(float dashLen, float gapLen)
    {
        if (numDashes_ < MAX_DASHES) {
            totalDashLen_ += dashLen + gapLen;
            dashes_[numDashes_++] = dashLen;
            dashes_[numDashes_++] = gapLen;
        }
    }

    void VertexGenerateDash::DashStart(float ds)
    {
        dashStart_ = ds;
        CalcDashStart(std::fabs(ds));
    }

    void VertexGenerateDash::CalcDashStart(float ds)
    {
        currDash_ = 0;
        currDashStart_ = 0.0f;
        for (; ds > 0.0f;) {
            if (ds > dashes_[currDash_]) {
                ds -= dashes_[currDash_];
                ++currDash_;
                currDashStart_ = 0.0f;
                if (currDash_ >= numDashes_) {
                    currDash_ = 0;
                }
            } else {
                currDashStart_ = ds;
                ds = 0.0f;
            }
        }
    }

    void VertexGenerateDash::RemoveAll()
    {
        status_ = INITIAL;
        srcVertices_.RemoveAll();
        closed_ = 0;
    }

    void VertexGenerateDash::AddVertex(float x, float y, unsigned cmd)
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

    void VertexGenerateDash::Rewind(unsigned)
    {
        if (status_ == INITIAL) {
            srcVertices_.Close(closed_ != 0);
            ShortenPath(srcVertices_, shorten_, closed_);
        }
        status_ = READY;
        srcVertex_ = 0;
    }

    unsigned VertexGenerateDash::Vertex(float* x, float* y)
    {
        unsigned cmd = PATH_CMD_MOVE_TO;
        while (!IsStop(cmd)) {
            switch (status_) {
                case INITIAL:
                    Rewind(0);
                case READY:
                    if (numDashes_ < FLOATNUM || srcVertices_.Size() < FLOATNUM) {
                        cmd = PATH_CMD_STOP;
                        break;
                    }
                    status_ = POLYLINE;
                    srcVertex_ = 1;
                    vertexDist1_ = &srcVertices_[0];
                    vertexDist2_ = &srcVertices_[1];
                    currRest = vertexDist1_->vertexDistance;
                    *x = vertexDist1_->vertexXCoord;
                    *y = vertexDist1_->vertexYCoord;
                    if (dashStart_ >= 0.0f) {
                        CalcDashStart(dashStart_);
                    }
                    return PATH_CMD_MOVE_TO;
                case POLYLINE: {
                    float dashRest = dashes_[currDash_] - currDashStart_;
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
                        currDashStart_ = 0.0f;
                        *x = vertexDist2_->vertexXCoord -
                                (vertexDist2_->vertexXCoord - vertexDist1_->vertexXCoord) * currRest / vertexDist1_->vertexDistance;
                        *y = vertexDist2_->vertexYCoord -
                                (vertexDist2_->vertexYCoord - vertexDist1_->vertexYCoord) * currRest / vertexDist1_->vertexDistance;
                    } else {
                        currDashStart_ += currRest;
                        *x = vertexDist2_->vertexXCoord;
                        *y = vertexDist2_->vertexYCoord;
                        ++srcVertex_;
                        vertexDist1_ = vertexDist2_;
                        currRest = vertexDist1_->vertexDistance;
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
                } break;
                case STOP:
                    cmd = PATH_CMD_STOP;
                    break;
            }
        }
        return PATH_CMD_STOP;
    }
#endif
} // namespace OHOS
