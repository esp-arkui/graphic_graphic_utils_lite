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

#ifndef GRAPHIC_GEOMETRY_BEZIER_ARC_INCLUDED
#define GRAPHIC_GEOMETRY_BEZIER_ARC_INCLUDED

#include "gfx_utils/graphics/graphic_depict/agg_conv_transform.h"
#include "gfx_utils/graphics/graphic_depict/agg_conv_transform.h"

namespace OHOS {
void ArcToBezier(double cx, double cy, double rx, double ry, 
                    double startAngle, double sweepAngle,
                    double* curve);

class BezierArc : public HeapBase
{
public:
    
    BezierArc() : vertex_(26), numVertices_(0), cmd_(PATH_CMD_LINE_TO) {}

    BezierArc(double x, double y, 
                double rx, double ry, 
                double startAngle, 
                double sweepAngle)
    {
        Init(x, y, rx, ry, startAngle, sweepAngle);
    }

    void Init(double x,  double y, 
              double rx, double ry, 
              double startAngle, 
              double sweepAngle);

    void Rewind(unsigned)
    {
        vertex_ = 0;
    }

    unsigned Vertex(double* x, double* y)
    {
        if (vertex_ >= numVertices_) {
            return PATH_CMD_STOP;
        }
        *x = vertices_[vertex_];
        *y = vertices_[vertex_ + 1];
        vertex_ += 2;
        return (vertex_ == 2) ? unsigned(PATH_CMD_MOVE_TO) : cmd_;
    }

    unsigned  Nuvertices_() const 
    { 
        return numVertices_; 
    }
    const double* Vertices() const 
    {
        return vertices_;     
    }
    double*       Vertices()       
    { 
        return vertices_;     
    }
 
private:
    unsigned vertex_;
    unsigned numVertices_;
    double   vertices_[26];
    unsigned cmd_;
};

class BezierArcSvg : public HeapBase {
public:
    
    BezierArcSvg() : arc_(), radiiOK_(false) {}

    BezierArcSvg(double x1,
                    double y1, 
                    double rx, double ry, 
                    double angle,
                    bool largeArcFlag,
                    bool sweepFlag,
                    double x2, double y2) : 
        arc_(), radiiOK_(false)
    {
        Init(x1, y1, rx, ry, angle, largeArcFlag, sweepFlag, x2, y2);
    }
   
    bool RadiiOK() const { return radiiOK_; }

    void Init(double x1,
              double y1,
              double rx,
              double ry,
              double angle,
              bool largeArcFlag,
              bool sweepFlag,
              double x2,
              double y2);

    void Rewind(unsigned)
    {
        arc_.Rewind(0);
    }

    unsigned Vertex(double* x, double* y)
    {
        return arc_.Vertex(x, y);
    }

    const double* Vertices() const
    {
        return arc_.Vertices();
    }

    double* Vertices()
    {
        return arc_.Vertices();
    }

    unsigned  Nuvertices_() const 
    { 
        return arc_.Nuvertices_(); 
    }

private:
    bezier_arc arc_;
    bool       radiiOK_;
};

}


#endif
