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
#include "gfx_utils/graphics/graphic_transform/agg_trans_affine.h"



namespace OHOS
{

    //------------------------------------------------------------------------
    const TransAffine& TransAffine::ParlToParl(const double* src, 
                                                   const double* dst)
    {
        sx  = src[2] - src[0];
        shy = src[3] - src[1];
        shx = src[4] - src[0];
        sy  = src[5] - src[1];
        tx  = src[0];
        ty  = src[1];
        invert();
        multiply(TransAffine(dst[2] - dst[0], dst[3] - dst[1], 
                              dst[4] - dst[0], dst[5] - dst[1],
                              dst[0], dst[1]));
        return *this;
    }

    //------------------------------------------------------------------------
    const TransAffine& TransAffine::RectToParl(double x1, double y1, 
                                                   double x2, double y2, 
                                                   const double* parl)
    {
        double src[6];
        src[0] = x1; src[1] = y1;
        src[2] = x2; src[3] = y1;
        src[4] = x2; src[5] = y2;
        ParlToParl(src, parl);
        return *this;
    }


    //------------------------------------------------------------------------
    const TransAffine& TransAffine::multiply(const TransAffine& m)
    {
        double t0 = sx  * m.sx + shy * m.shx;
        double t2 = shx * m.sx + sy  * m.shx;
        double t4 = tx  * m.sx + ty  * m.shx + m.tx;
        shy = sx  * m.shy + shy * m.sy;
        sy  = shx * m.shy + sy  * m.sy;
        ty  = tx  * m.shy + ty  * m.sy + m.ty;
        sx  = t0;
        shx = t2;
        tx  = t4;
        return *this;
    }


    //------------------------------------------------------------------------
    const TransAffine& TransAffine::invert()
    {
        double d  = determinant_reciprocal();

        double t0  =  sy  * d;
               sy  =  sx  * d;
               shy = -shy * d;
               shx = -shx * d;

        double t4 = -tx * t0  - ty * shx;
               ty = -tx * shy - ty * sy;

        sx = t0;
        tx = t4;
        return *this;
    }


    //------------------------------------------------------------------------
    const TransAffine& TransAffine::reset()
    {
        sx  = sy  = 1.0; 
        shy = shx = tx = ty = 0.0;
        return *this;
    }

    //------------------------------------------------------------------------
    bool TransAffine::is_identity(double epsilon) const
    {
        return is_equal_eps(sx,  1.0, epsilon) &&
               is_equal_eps(shy, 0.0, epsilon) &&
               is_equal_eps(shx, 0.0, epsilon) && 
               is_equal_eps(sy,  1.0, epsilon) &&
               is_equal_eps(tx,  0.0, epsilon) &&
               is_equal_eps(ty,  0.0, epsilon);
    }

    //------------------------------------------------------------------------
    bool TransAffine::is_valid(double epsilon) const
    {
        return std::fabs(sx) > epsilon && std::fabs(sy) > epsilon;
    }


}

