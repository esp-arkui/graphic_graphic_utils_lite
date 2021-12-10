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

#ifndef GRAPHIC_VCGEN_STROKE_INCLUDED
#define GRAPHIC_VCGEN_STROKE_INCLUDED

#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_math_stroke.h"


namespace OHOS
{

    class VCGenStroke
    {
        enum Status
        {
            INITIAL,
            READY,
            CAP1,
            CAP2,
            OUTLINE1,
            CLOSE_FIRST,
            OUTLINE2,
            OUT_VERTICES,
            END_POLY1,
            END_POLY2,
            STOP
        };

    public:
        using VertexStorage= vertex_sequence<vertex_dist, 6>;
        using CoordStorage = pod_bvector<point_d, 6>;

        VCGenStroke();

        void LineCap(LineCap lc)
        { 
            stroker_.line_cap(lc); 
        }

        void LineJoin(LineJoin lj)
        { 
            stroker_.line_join(lj);
        }

        void InnerJoin(InnerJoin ij)
        { 
            stroker_.inner_join(ij); 
        }

        enum LineCap   LineCap()   const
        { 
            return stroker_.line_cap(); 
        }

        enum LineJoin  LineJoin()  const
        { 
            return stroker_.line_join(); 
        }

        enum InnerJoin InnerJoin() const
        { 
            return stroker_.inner_join(); 
        }

        void Width(double w) 
        { 
            stroker_.width(w); 
        }

        void MiterLimit(double ml) 
        { 
            stroker_.miter_limit(ml); 
        }



        void ApproximationScale(double as) 
        { 
            stroker_.approximation_scale(as); 
        }

        double Width() const 
        { 
            return stroker_.width(); 
        }

        double MiterLimit() const 
        { 
            return stroker_.miter_limit(); 
        }



        double ApproximationScale() const 
        { 
            return stroker_.approximation_scale(); 
        }

        void Shorten(double s) 
        { 
            shorten_ = s; 
        }

        double Shorten() const 
        { 
            return shorten_; 
        }

        void RemoveAll();

        void AddVertex(double x, double y, unsigned cmd);

        void     Rewind(unsigned pathId);
        unsigned Vertex(double* x, double* y);

    private:
        VCGenStroke(const VCGenStroke&);
        const VCGenStroke& operator = (const VCGenStroke&);

        math_stroke<CoordStorage> stroker_;
        VertexStorage             srcVertices_;
        CoordStorage              outVertices_;
        double                     shorten_;
        unsigned                   closed_;
        Status                   status_;
        Status                   prevStatus_;
        unsigned                   srcVertex_;
        unsigned                   outVertex_;
    };


}

#endif
