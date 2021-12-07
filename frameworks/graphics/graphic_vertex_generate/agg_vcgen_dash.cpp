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
#include "gfx_utils/graphics/graphic_vertex_generate/agg_vcgen_dash.h"
#include "gfx_utils/graphics/graphic_geometry/agg_shorten_path.h"

namespace OHOS
{

    //------------------------------------------------------------------------
    VCGenDash::VCGenDash() :
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



    //------------------------------------------------------------------------
    void VCGenDash::RemoveAllDashes()
    {
        totalDashLen_ = 0.0;
        numDashes_ = 0;
        currDashStart_ = 0.0;
        currDash_ = 0;
    }


    //------------------------------------------------------------------------
    void VCGenDash::AddDash(double dashLen, double gapLen)
    {
        if(numDashes_ < MAX_DASHES){
            totalDashLen_ += dashLen + gapLen;
            dashes_[numDashes_++] = dashLen;
            dashes_[numDashes_++] = gapLen;
        }
    }


    //------------------------------------------------------------------------
    void VCGenDash::DashStart(double ds)
    {
        dashStart_ = ds;
        CalcDashStart(std::fabs(ds));
    }


    //------------------------------------------------------------------------
    void VCGenDash::CalcDashStart(double ds)
    {
        currDash_ = 0;
        currDashStart_ = 0.0;
        for(;ds > 0.0;){
            if(ds > dashes_[currDash_]){
                ds -= dashes_[currDash_];
                ++currDash_;
                currDashStart_ = 0.0;
                if(currDash_ >= numDashes_) 
                {
                    currDash_ = 0;
                }
            }else{
                currDashStart_ = ds;
                ds = 0.0;
            }
        }
    }


    //------------------------------------------------------------------------
    void VCGenDash::RemoveAll()
    {
        status_ = INITIAL;
        srcVertices_.remove_all();
        closed_ = 0;
    }


    //------------------------------------------------------------------------
    void VCGenDash::AddVertex(double x, double y, unsigned cmd)
    {
        status_ = INITIAL;
        if(is_move_to(cmd)){
            srcVertices_.modify_last(vertex_dist(x, y));
        }
        else{
            if(is_vertex(cmd)){
                srcVertices_.add(vertex_dist(x, y));
            }else{
                closed_ = get_close_flag(cmd);
            }
        }
    }


    //------------------------------------------------------------------------
    void VCGenDash::Rewind(unsigned)
    {
        if(status_ == INITIAL){
            srcVertices_.close(closed_ != 0);
            shorten_path(srcVertices_, shorten_, closed_);
        }
        status_ = READY;
        srcVertex_ = 0;
    }


    //------------------------------------------------------------------------
    unsigned VCGenDash::Vertex(double* x, double* y)
    {
        unsigned cmd = path_cmd_move_to;
        while(!is_stop(cmd))
        {
            switch(status_)
            {
            case INITIAL:
                Rewind(0);

            case READY:
                if(numDashes_ < 2 || srcVertices_.size() < 2){
                    cmd = path_cmd_stop;
                    break;
                }
                status_ = POLYLINE;
                srcVertex_ = 1;
                vertexDist1_ = &srcVertices_[0];
                vertexDist2_ = &srcVertices_[1];
                currRest = vertexDist1_->dist;
                *x = vertexDist1_->x;
                *y = vertexDist1_->y;
                if(dashStart_ >= 0.0) {
                    CalcDashStart(dashStart_);
                }
                return path_cmd_move_to;

            case POLYLINE:
                {
                    double dash_rest = dashes_[currDash_] - currDashStart_;

                    unsigned cmd = (currDash_ & 1) ? 
                                   path_cmd_move_to : 
                                   path_cmd_line_to;

                    if(currRest > dash_rest){
                        currRest -= dash_rest;
                        ++currDash_;
                        if(currDash_ >= numDashes_) {
                            currDash_ = 0;
                        }
                        currDashStart_ = 0.0;
                        *x = vertexDist2_->x - (vertexDist2_->x - vertexDist1_->x) * currRest / vertexDist1_->dist;
                        *y = vertexDist2_->y - (vertexDist2_->y - vertexDist1_->y) * currRest / vertexDist1_->dist;
                    }else{
                        currDashStart_ += currRest;
                        *x = vertexDist2_->x;
                        *y = vertexDist2_->y;
                        ++srcVertex_;
                        vertexDist1_ = vertexDist2_;
                        currRest = vertexDist1_->dist;
                        if(closed_){
                            if(srcVertex_ > srcVertices_.size()){
                                status_ = STOP;
                            }else{
                                int n=0;
                                if(srcVertex_ >= srcVertices_.size()){
                                    vertexDist2_ = &srcVertices_[0];
                                }else{
                                    vertexDist2_ = &srcVertices_[srcVertex_];
                                }
                            }
                        }else{
                            if(srcVertex_ >= srcVertices_.size()){
                                status_ = STOP;
                            }else{
                                vertexDist2_ = &srcVertices_[srcVertex_];
                            }
                        }
                    }
                    return cmd;
                }
                break;

            case STOP:
                cmd = path_cmd_stop;
                break;
            }

        }
        return path_cmd_stop;
    }


}

