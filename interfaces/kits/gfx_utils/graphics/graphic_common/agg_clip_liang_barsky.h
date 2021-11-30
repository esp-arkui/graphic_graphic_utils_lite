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

#ifndef GRAPHIC_GEOMETRY_CLIP_LIANG_BARSKY_INCLUDED
#define GRAPHIC_GEOMETRY_CLIP_LIANG_BARSKY_INCLUDED

#include "graphic_geometry_basics.h"

namespace OHOS
{

    //------------------------------------------------------------------------
    enum ClippingFlagsE
    {
        CLIPPING_FLAGS_X1_CLIPPED = 4,
        CLIPPING_FLAGS_X2_CLIPPED = 1,
        CLIPPING_FLAGS_Y1_CLIPPED = 8,
        CLIPPING_FLAGS_Y2_CLIPPED = 2,
        CLIPPING_FLAGS_X_CLIPPED = CLIPPING_FLAGS_X1_CLIPPED | CLIPPING_FLAGS_X2_CLIPPED,
        CLIPPING_FLAGS_Y_CLIPPED = CLIPPING_FLAGS_Y1_CLIPPED | CLIPPING_FLAGS_Y2_CLIPPED
    };

    //----------------------------------------------------------clipping_flags
    // Determine the clipping code of the vertex according to the 
    // Cyrus-Beck line clipping algorithm
    //
    //        |        |
    //  0110  |  0010  | 0011
    //        |        |
    // -------+--------+-------- clip_box.y2
    //        |        |
    //  0100  |  0000  | 0001
    //        |        |
    // -------+--------+-------- clip_box.y1
    //        |        |
    //  1100  |  1000  | 1001
    //        |        |
    //  clipBox.x1  clipBox.x2
    //
    // 
    template<class T>
    inline unsigned ClippingFlags(T x, T y, const RectBase<T>& clipBox)
    {
        return  (x > clipBox.x2) | ((y > clipBox.y2) << 1) | ((x < clipBox.x1) << 2) | ((y < clipBox.y1) << 3);
    }

    //--------------------------------------------------------ClippingFlagsX
    template <class T> inline unsigned ClippingFlagsX(T x, const RectBase<T>& clipBox)
    {
        return (x > clipBox.x2) | ((x < clipBox.x1) << 2);
    }


    //--------------------------------------------------------ClippingFlagsY
    template <class T> inline unsigned ClippingFlagsY(T y, const RectBase<T>& clipBox)
    {
        return ((y > clipBox.y2) << 1) | ((y < clipBox.y1) << 3);
    }


    //-------------------------------------------------------ClipLiangBarsky
    template<class T> inline unsigned ClipLiangBarsky(T x1, T y1, T x2, T y2, const RectBase<T>& clipBox,
                                      T* x, T* y)
    {
        const double nearzero = 1e-30;

        double deltax = x2 - x1;
        double deltay = y2 - y1; 
        double xin;
        double xout;
        double yin;
        double yout;
        double tinx;
        double tiny;
        double toutx;
        double touty;  
        double tin1;
        double tin2;
        double tout1;
        unsigned np = 0;

        if(deltax == 0.0) 
        {   
            // bump off of the vertical
            deltax = (x1 > clipBox.x1) ? -nearzero : nearzero;
        }

        if(deltay == 0.0) 
        { 
            // bump off of the horizontal 
            deltay = (y1 > clipBox.y1) ? -nearzero : nearzero;
        }
        
        if(deltax > 0.0) 
        {                
            // points to right
            xin = clipBox.x1;
            xout = clipBox.x2;
        }
        else 
        {
            xin = clipBox.x2;
            xout = clipBox.x1;
        }

        if(deltay > 0.0) 
        {
            // points up
            yin = clipBox.y1;
            yout = clipBox.y2;
        }
        else 
        {
            yin = clipBox.y2;
            yout = clipBox.y1;
        }
        
        tinx = (xin - x1) / deltax;
        tiny = (yin - y1) / deltay;
        
        if (tinx < tiny) 
        {
            // hits x first
            tin1 = tinx;
            tin2 = tiny;
        }
        else
        {
            // hits y first
            tin1 = tiny;
            tin2 = tinx;
        }
        
        if(tin1 <= 1.0) 
        {
            if(0.0 < tin1) 
            {
                *x++ = (T)xin;
                *y++ = (T)yin;
                ++np;
            }

            if(tin2 <= 1.0)
            {
                toutx = (xout - x1) / deltax;
                touty = (yout - y1) / deltay;
                
                tout1 = (toutx < touty) ? toutx : touty;
                
                if(tin2 > 0.0 || tout1 > 0.0) 
                {
                    if(tin2 <= tout1) 
                    {
                        if(tin2 > 0.0) 
                        {
                            if(tinx > tiny) 
                            {
                                *x++ = (T)xin;
                                *y++ = (T)(y1 + tinx * deltay);
                            }
                            else 
                            {
                                *x++ = (T)(x1 + tiny * deltax);
                                *y++ = (T)yin;
                            }
                            ++np;
                        }

                        if(tout1 < 1.0) 
                        {
                            if(toutx < touty) 
                            {
                                *x++ = (T)xout;
                                *y++ = (T)(y1 + toutx * deltay);
                            }
                            else 
                            {
                                *x++ = (T)(x1 + touty * deltax);
                                *y++ = (T)yout;
                            }
                        }
                        else 
                        {
                            *x++ = x2;
                            *y++ = y2;
                        }
                        ++np;
                    }
                    else 
                    {
                        if(tinx > tiny) 
                        {
                            *x++ = (T)xin;
                            *y++ = (T)yout;
                        }
                        else 
                        {
                            *x++ = (T)xout;
                            *y++ = (T)yin;
                        }
                        ++np;
                    }
                }
            }
        }
        return np;
    }


    //----------------------------------------------------------------------------ClipMovePoint
    template<class T>
    bool ClipMovePoint(T x1, T y1, T x2, T y2, 
                         const RectBase<T>& clipBox, 
                         T* x, T* y, unsigned flags)
    {
       T bound;

       if(flags & CLIPPING_FLAGS_X_CLIPPED)
       {
           if(x1 == x2)
           {
               return false;
           }
           bound = (flags & CLIPPING_FLAGS_X1_CLIPPED) ? clipBox.x1 : clipBox.x2;
           *y = (T)(double(bound - x1) * (y2 - y1) / (x2 - x1) + y1);
           *x = bound;
       }

       flags = ClippingFlagsY(*y, clipBox);
       if(flags & CLIPPING_FLAGS_Y_CLIPPED)
       {
           if(y1 == y2)
           {
               return false;
           }
           bound = (flags & CLIPPING_FLAGS_Y1_CLIPPED) ? clipBox.y1 : clipBox.y2;
           *x = (T)(double(bound - y1) * (x2 - x1) / (y2 - y1) + x1);
           *y = bound;
       }
       return true;
    }

    //-------------------------------------------------------ClipLineSegment
    // Returns: ret >= 4        - Fully clipped
    //          (ret & 1) != 0  - First point has been moved
    //          (ret & 2) != 0  - Second point has been moved
    //
    template<class T>
    unsigned ClipLineSegment(T* x1, T* y1, T* x2, T* y2, const RectBase<T>& clipBox)
    {
        unsigned f1 = ClippingFlags(*x1, *y1, clipBox);
        unsigned f2 = ClippingFlags(*x2, *y2, clipBox);
        unsigned ret = 0;

        if((f2 | f1) == 0)
        {
            // Fully visible
            return 0;
        }

        if((f1 & CLIPPING_FLAGS_X_CLIPPED) != 0 && 
           (f1 & CLIPPING_FLAGS_X_CLIPPED) == (f2 & CLIPPING_FLAGS_X_CLIPPED))
        {
            // Fully clipped
            return 4;
        }

        if((f1 & CLIPPING_FLAGS_Y_CLIPPED) != 0 && 
           (f1 & CLIPPING_FLAGS_Y_CLIPPED) == (f2 & CLIPPING_FLAGS_Y_CLIPPED))
        {
            // Fully clipped
            return 4;
        }

        T tx1 = *x1;
        T ty1 = *y1;
        T tx2 = *x2;
        T ty2 = *y2;
        if(f1) 
        {   
            if(!ClipMovePoint(tx1, ty1, tx2, ty2, clipBox, x1, y1, f1)) 
            {
                return 4;
            }
            if(*x1 == *x2 && *y1 == *y2) 
            {
                return 4;
            }
            ret |= 1;
        }
        if(f2) 
        {
            if (!ClipMovePoint(tx1, ty1, tx2, ty2, clipBox, x2, y2, f2))
            {
                return 4;
            }
            if(*x1 == *x2 && *y1 == *y2) 
            {
                return 4;
            }
            ret |= 2;
        }
        return ret;
    }


}


#endif
