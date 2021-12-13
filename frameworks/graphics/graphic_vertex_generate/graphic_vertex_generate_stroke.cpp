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
#include "gfx_utils/graphics/graphic_vertex_generate/graphic_vertex_generate_stroke.h"
#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_shorten_path.h"

namespace OHOS {
    VCGenStroke::VCGenStroke()
        :stroker_(),
         srcVertices_(),
         outVertices_(),
         shorten_(0.0),
         closed_(0),
         status_(INITIAL),
         srcVertex_(0),
         outVertex_(0)
    {
    }

    void VCGenStroke::RemoveAll()
    {
        srcVertices_.RemoveAll();
        closed_ = 0;
        status_ = INITIAL;
    }

    void VCGenStroke::AddVertex(double x, double y, unsigned cmd)
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

    void VCGenStroke::Rewind(unsigned)
    {
        if (status_ == INITIAL) {
            srcVertices_.Close(closed_ != 0);
            ShortenPath(srcVertices_, shorten_, closed_);
            const unsigned verticeNum = 3;
            if (srcVertices_.Size() < verticeNum) {
                closed_ = 0;
            }
        }
        status_ = READY;
        srcVertex_ = 0;
        outVertex_ = 0;
    }

    unsigned VCGenStroke::Vertex(double* x, double* y)
    {
        const unsigned verticesNum = 2;
        unsigned cmd = PATH_CMD_LINE_TO;
        while (!IsStop(cmd)) {
            switch (status_) {
                case INITIAL:
                    Rewind(0);
                case READY:
                    if (srcVertices_.Size() < verticesNum + unsigned(closed_ != 0)) {
                        cmd = PATH_CMD_STOP;
                        break;
                    }
                    if (closed_) {
                        status_ = OUTLINE1;
                    } else {
                        status_ = CAP1;
                    }
                    cmd = PATH_CMD_MOVE_TO;
                    srcVertex_ = 0;
                    outVertex_ = 0;
                    break;
                case CAP1:
                    stroker_.CalcCap(outVertices_,
                                     srcVertices_[0],
                                     srcVertices_[1],
                                     srcVertices_[0].dist);
                    srcVertex_ = 1;
                    prevStatus_ = OUTLINE1;
                    status_ = OUT_VERTICES;
                    outVertex_ = 0;
                    break;
                case CAP2:
                    stroker_.CalcCap(outVertices_,
                                     srcVertices_[srcVertices_.Size() - 1],
                                     srcVertices_[srcVertices_.Size() - verticesNum],
                                     srcVertices_[srcVertices_.Size() - verticesNum].dist);
                    prevStatus_ = OUTLINE2;
                    status_ = OUT_VERTICES;
                    outVertex_ = 0;
                    break;
                case OUTLINE1:
                    if (closed_ && srcVertex_ >= srcVertices_.Size()) {
                        prevStatus_ = CLOSE_FIRST;
                        status_ = END_POLY1;
                        break;
                    }
                    if(!closed_ && srcVertex_ >= srcVertices_.Size() - 1){
                        status_ = CAP2;
                        break;
                    }
                    stroker_.CalcJoin(outVertices_,
                                      srcVertices_.Prev(srcVertex_),
                                      srcVertices_.Curr(srcVertex_),
                                      srcVertices_.Next(srcVertex_),
                                      srcVertices_.Prev(srcVertex_).dist,
                                      srcVertices_.Curr(srcVertex_).dist);
                    ++srcVertex_;
                    prevStatus_ = status_;
                    status_ = OUT_VERTICES;
                    outVertex_ = 0;
                    break;
                case CLOSE_FIRST:
                    status_ = OUTLINE2;
                    cmd = PATH_CMD_MOVE_TO;
                case OUTLINE2:
                    if (srcVertex_ <= unsigned(closed_ == 0)) {
                        status_ = END_POLY2;
                        prevStatus_ = STOP;
                        break;
                    }
                    --srcVertex_;
                    stroker_.CalcJoin(outVertices_,
                                      srcVertices_.Next(srcVertex_),
                                      srcVertices_.Curr(srcVertex_),
                                      srcVertices_.Prev(srcVertex_),
                                      srcVertices_.Curr(srcVertex_).dist,
                                      srcVertices_.Prev(srcVertex_).dist);
                    outVertex_ = 0;
                    prevStatus_ = status_;
                    status_ = OUT_VERTICES;
                    break;
                case OUT_VERTICES:
                    if (outVertex_ >= outVertices_.Size()) {
                        status_ = prevStatus_;
                    } else {
                        const PointD& c = outVertices_[outVertex_++];
                        *x = c.x;
                        *y = c.y;
                        return cmd;
                    }
                    break;
                case END_POLY1:
                    status_ = prevStatus_;
                    return PATH_CMD_END_POLY | PATH_FLAGS_CLOSE | PATH_FLAGS_CCW;
                case END_POLY2:
                    status_ = prevStatus_;
                    return PATH_CMD_END_POLY | PATH_FLAGS_CLOSE | PATH_FLAGS_CW;
                case STOP:
                    cmd = PATH_CMD_STOP;
                    break;
            }
        }
        return cmd;
    }

} // namespace OHOS
