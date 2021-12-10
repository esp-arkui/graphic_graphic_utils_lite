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
#include "gfx_utils/graphics/graphic_vertex_generate/agg_vcgen_stroke.h"

#include "gfx_utils/graphics/graphic_geometry/agg_shorten_path.h"

namespace OHOS {

    //------------------------------------------------------------------------
    VCGenStroke::VCGenStroke() :
        stroker_(),
        srcVertices_(),
        outVertices_(),
        shorten_(0.0),
        closed_(0),
        status_(INITIAL),
        srcVertex_(0),
        outVertex_(0)
    {
    }

    //------------------------------------------------------------------------
    void VCGenStroke::RemoveAll()
    {
        srcVertices_.remove_all();
        closed_ = 0;
        status_ = INITIAL;
    }

    //------------------------------------------------------------------------
    void VCGenStroke::AddVertex(double x, double y, unsigned cmd)
    {
        status_ = INITIAL;
        if (is_move_to(cmd)) {
            srcVertices_.modify_last(vertex_dist(x, y));
        } else {
            if (is_vertex(cmd)) {
                srcVertices_.add(vertex_dist(x, y));
            } else {
                closed_ = get_close_flag(cmd);
            }
        }
    }

    //------------------------------------------------------------------------
    void VCGenStroke::Rewind(unsigned)
    {
        if (status_ == INITIAL) {
            srcVertices_.close(closed_ != 0);
            shorten_path(srcVertices_, shorten_, closed_);
            if (srcVertices_.size() < 3) {
                closed_ = 0;
            }
        }
        status_ = READY;
        srcVertex_ = 0;
        outVertex_ = 0;
    }

    //------------------------------------------------------------------------
    unsigned VCGenStroke::Vertex(double* x, double* y)
    {
        unsigned cmd = path_cmd_line_to;
        while (!is_stop(cmd)) {
            switch (status_) {
                case INITIAL:
                    Rewind(0);
                case READY:
                    if (srcVertices_.size() < 2 + unsigned(closed_ != 0)) {
                        cmd = path_cmd_stop;
                        break;
                    }
                    if (closed_) {
                        status_ = OUTLINE1;
                    } else {
                        status_ = CAP1;
                    }
                    cmd = path_cmd_move_to;
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
                                     srcVertices_[srcVertices_.size() - 1],
                                     srcVertices_[srcVertices_.size() - 2],
                                     srcVertices_[srcVertices_.size() - 2].dist);
                    prevStatus_ = OUTLINE2;
                    status_ = OUT_VERTICES;
                    outVertex_ = 0;
                    break;
                case OUTLINE1:
                    if (closed_) {
                        if (srcVertex_ >= srcVertices_.size()) {
                            prevStatus_ = CLOSE_FIRST;
                            status_ = END_POLY1;
                            break;
                        }
                    } else {
                        if (srcVertex_ >= srcVertices_.size() - 1) {
                            status_ = CAP2;
                            break;
                        }
                    }
                    stroker_.CalcJoin(outVertices_,
                                      srcVertices_.prev(srcVertex_),
                                      srcVertices_.curr(srcVertex_),
                                      srcVertices_.next(srcVertex_),
                                      srcVertices_.prev(srcVertex_).dist,
                                      srcVertices_.curr(srcVertex_).dist);
                    ++srcVertex_;
                    prevStatus_ = status_;
                    status_ = OUT_VERTICES;
                    outVertex_ = 0;
                    break;
                case CLOSE_FIRST:
                    status_ = OUTLINE2;
                    cmd = path_cmd_move_to;
                case OUTLINE2:
                    if (srcVertex_ <= unsigned(closed_ == 0)) {
                        status_ = END_POLY2;
                        prevStatus_ = STOP;
                        break;
                    }
                    --srcVertex_;
                    stroker_.CalcJoin(outVertices_,
                                      srcVertices_.next(srcVertex_),
                                      srcVertices_.curr(srcVertex_),
                                      srcVertices_.prev(srcVertex_),
                                      srcVertices_.curr(srcVertex_).dist,
                                      srcVertices_.prev(srcVertex_).dist);
                    outVertex_ = 0;
                    prevStatus_ = status_;
                    status_ = OUT_VERTICES;
                    break;
                case OUT_VERTICES:
                    if (outVertex_ >= outVertices_.size()) {
                        status_ = prevStatus_;
                    } else {
                        const point_d& c = outVertices_[outVertex_++];
                        *x = c.x;
                        *y = c.y;
                        return cmd;
                    }
                    break;
                case END_POLY1:
                    status_ = prevStatus_;
                    return path_cmd_end_poly | path_flags_close | path_flags_ccw;
                case END_POLY2:
                    status_ = prevStatus_;
                    return path_cmd_end_poly | path_flags_close | path_flags_cw;
                case STOP:
                    cmd = path_cmd_stop;
                    break;
            }
        }
        return cmd;
    }

} // namespace OHOS
