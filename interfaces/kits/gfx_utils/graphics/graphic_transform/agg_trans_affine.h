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
#ifndef AGG_TRANS_AFFINE_INCLUDED
#define AGG_TRANS_AFFINE_INCLUDED

#include <cmath>
#include "gfx_utils/graphics/graphic_common/agg_basics.h"

namespace OHOS
{
    const double affineEpsilon = 1e-14; 

    //============================================================TransAffine
    //
    // See Implementation agg_trans_affine.cpp
    //
    // Affine transformation are linear transformations in Cartesian coordinates
    // (strictly speaking not only in Cartesian, but for the beginning we will 
    // think so). They are rotation, scaling, translation and skewing.  
    // After any affine transformation a line segment remains a line segment 
    // and it will never become a curve. 
    //
    // There will be no math about matrix calculations, since it has been 
    // described many times. Ask yourself a very simple question:
    // "why do we need to understand and use some matrix stuff instead of just 
    // rotating, scaling and so on". The answers are:
    //
    // 1. Any combination of transformations can be done by only 4 multiplications
    //    and 4 additions in floating point.
    // 2. One matrix transformation is equivalent to the number of consecutive
    //    discrete transformations, i.e. the matrix "accumulates" all transformations 
    //    in the order of their settings. Suppose we have 4 transformations: 
    //       * rotate by 30 degrees,
    //       * scale X to 2.0, 
    //       * scale Y to 1.5, 
    //       * move to (100, 100). 
    //    The result will depend on the order of these transformations, 
    //    and the advantage of matrix is that the sequence of discret calls:
    //    rotate(30), scaleX(2.0), scaleY(1.5), move(100,100) 
    //    will have exactly the same result as the following matrix transformations:
    //   
    //    affine_matrix m;
    //    m *= rotate_matrix(30); 
    //    m *= scaleX_matrix(2.0);
    //    m *= scaleY_matrix(1.5);
    //    m *= move_matrix(100,100);
    //
    //    m.transform_my_point_at_last(x, y);
    //
    // What is the good of it? In real life we will set-up the matrix only once
    // and then transform many points, let alone the convenience to set any 
    // combination of transformations.
    //
    // So, how to use it? Very easy - literally as it's shown above. Not quite,
    // let us write a correct example:
    //
    // OHOS::trans_affine m;
    // m *= OHOS::trans_affine_rotation(30.0 * 3.1415926 / 180.0);
    // m *= OHOS::trans_affine_scaling(2.0, 1.5);
    // m *= OHOS::trans_affine_translation(100.0, 100.0);
    // m.transform(&x, &y);
    //
    // The affine matrix is all you need to perform any linear transformation,
    // but all transformations have origin point (0,0). It means that we need to 
    // use 2 translations if we want to rotate someting around (100,100):
    // 
    // m *= OHOS::trans_affine_translation(-100.0, -100.0);         // move to (0,0)
    // m *= OHOS::trans_affine_rotation(30.0 * 3.1415926 / 180.0);  // rotate
    // m *= OHOS::trans_affine_translation(100.0, 100.0);           // move back to (100,100)
    //----------------------------------------------------------------------
    struct TransAffine
    {
        double sx, shy, shx, sy, tx, ty;

        //------------------------------------------ Construction
        // Identity matrix
        TransAffine() :
            sx(1.0), shy(0.0), shx(0.0), sy(1.0), tx(0.0), ty(0.0)
        {}

        // Custom matrix. Usually used in derived classes
        TransAffine(double v0, double v1, double v2, 
                     double v3, double v4, double v5) :
            sx(v0), shy(v1), shx(v2), sy(v3), tx(v4), ty(v5)
        {}

        // Rectangle to a parallelogram.
        TransAffine(double x1, double y1, double x2, double y2, 
                     const double* parl)
        {
            RectToParl(x1, y1, x2, y2, parl);
        }


        //---------------------------------- Parellelogram transformations
        // transform a parallelogram to another one. Src and dst are 
        // pointers to arrays of three points (double[6], x1,y1,...) that 
        // identify three corners of the parallelograms assuming implicit 
        // fourth point. The arguments are arrays of double[6] mapped 
        // to x1,y1, x2,y2, x3,y3  where the coordinates are:
        //        *-----------------*
        //       /          (x3,y3)/
        //      /                 /
        //     /(x1,y1)   (x2,y2)/
        //    *-----------------*
        const TransAffine& ParlToParl(const double* src, 
                                         const double* dst);

        const TransAffine& RectToParl(double x1, double y1, 
                                         double x2, double y2, 
                                         const double* parl);

 

        //------------------------------------------ Operations
        // Reset - load an identity matrix
        const TransAffine& reset();

        // Direct transformations operations
        const TransAffine& translate(double x, double y);
        const TransAffine& rotate(double a);
        const TransAffine& scale(double s);
        const TransAffine& scale(double x, double y);

        // Multiply matrix to another one
        const TransAffine& multiply(const TransAffine& m);


        // Invert matrix. Do not try to invert degenerate matrices, 
        // there's no check for validity. If you set scale to 0 and 
        // then try to invert matrix, expect unpredictable result.
        const TransAffine& invert();

 

        //------------------------------------------- Operators
        
        // Multiply the matrix by another one
        const TransAffine& operator *= (const TransAffine& m)
        {
            return multiply(m);
        }



        //-------------------------------------------- Transformations
        // Direct transformation of x and y
        void transform(double* x, double* y) const;

 
        // Inverse transformation of x and y. It works slower than the 
        // direct transformation. For massive operations it's better to 
        // invert() the matrix and then use direct transformations. 
        void inverse_transform(double* x, double* y) const;



        // Calculate the reciprocal of the determinant
        double determinant_reciprocal() const
        {
            return 1.0 / (sx * sy - shy * shx);
        }

        // Get the average scale (by X and Y). 
        // Basically used to calculate the approximation_scale when
        // decomposinting curves into line segments.
        double scale() const;

        // Check to see if the matrix is not degenerate
        bool is_valid(double epsilon = affineEpsilon) const;

        // Check to see if it's an identity matrix
        bool is_identity(double epsilon = affineEpsilon) const;

        void   scaling_abs(double* x, double* y) const;
    };

    //------------------------------------------------------------------------
    inline void TransAffine::transform(double* x, double* y) const
    {
        double tmp = *x;
        *x = tmp * sx  + *y * shx + tx;
        *y = tmp * shy + *y * sy  + ty;
    }


    //------------------------------------------------------------------------
    inline void TransAffine::inverse_transform(double* x, double* y) const
    {
        double d = determinant_reciprocal();
        double a = (*x - tx) * d;
        double b = (*y - ty) * d;
        *x = a * sy - b * shx;
        *y = b * sx - a * shy;
    }

    //------------------------------------------------------------------------
    inline double TransAffine::scale() const
    {
        double x = 0.707106781 * sx  + 0.707106781 * shx;
        double y = 0.707106781 * shy + 0.707106781 * sy;
        return std::sqrt(x*x + y*y);
    }

    //------------------------------------------------------------------------
    inline const TransAffine& TransAffine::translate(double x, double y) 
    { 
        tx += x;
        ty += y; 
        return *this;
    }

    //------------------------------------------------------------------------
    inline const TransAffine& TransAffine::rotate(double a) 
    {
        double ca = std::cos(a); 
        double sa = std::sin(a);
        double t0 = sx  * ca - shy * sa;
        double t2 = shx * ca - sy * sa;
        double t4 = tx  * ca - ty * sa;
        shy = sx  * sa + shy * ca;
        sy  = shx * sa + sy * ca; 
        ty  = tx  * sa + ty * ca;
        sx  = t0;
        shx = t2;
        tx  = t4;
        return *this;
    }

    //------------------------------------------------------------------------
    inline const TransAffine& TransAffine::scale(double x, double y) 
    {
        double mm0 = x; // Possible hint for the optimizer
        double mm3 = y; 
        sx  *= mm0;
        shx *= mm0;
        tx  *= mm0;
        shy *= mm3;
        sy  *= mm3;
        ty  *= mm3;
        return *this;
    }

    //------------------------------------------------------------------------
    inline const TransAffine& TransAffine::scale(double s) 
    {
        double m = s; // Possible hint for the optimizer
        sx  *= m;
        shx *= m;
        tx  *= m;
        shy *= m;
        sy  *= m;
        ty  *= m;
        return *this;
    }


    //------------------------------------------------------------------------
    inline void TransAffine::scaling_abs(double* x, double* y) const
    {
        // Used to calculate scaling coefficients in image resampling. 
        // When there is considerable shear this method gives us much
        // better estimation than just sx, sy.
        *x = std::sqrt(sx  * sx  + shx * shx);
        *y = std::sqrt(shy * shy + sy  * sy);
    }

    //====================================================trans_affine_rotation
    // Rotation matrix. sin() and cos() are calculated twice for the same angle.
    // There's no harm because the performance of sin()/cos() is very good on all
    // modern processors. Besides, this operation is not going to be invoked too 
    // often.
    class trans_affine_rotation : public TransAffine
    {
    public:
        trans_affine_rotation(double a) : 
          TransAffine(std::cos(a), std::sin(a), -std::sin(a), std::cos(a), 0.0, 0.0)
        {}
    };

    //====================================================trans_affine_scaling
    // Scaling matrix. x, y - scale coefficients by X and Y respectively
    class trans_affine_scaling : public TransAffine
    {
    public:
        trans_affine_scaling(double x, double y) : 
          TransAffine(x, 0.0, 0.0, y, 0.0, 0.0)
        {}

        trans_affine_scaling(double s) : 
          TransAffine(s, 0.0, 0.0, s, 0.0, 0.0)
        {}
    };

    //================================================trans_affine_translation
    // Translation matrix
    class trans_affine_translation : public TransAffine
    {
    public:
        trans_affine_translation(double x, double y) : 
          TransAffine(1.0, 0.0, 0.0, 1.0, x, y)
        {}
    };

}


#endif

