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

#ifndef GRAPHIC_GEOMETRY_BASICS_INCLUDED
#define GRAPHIC_GEOMETRY_BASICS_INCLUDED

#include <cmath>

//---------------------------------------------------------GRAPHIC_GEOMETRY_CUSTOM_ALLOCATOR
#ifdef GRAPHIC_GEOMETRY_CUSTOM_ALLOCATOR
#include "graphic_geometry_allocator.h"
#else
namespace OHOS {
// The policy of all AGG containers and memory allocation strategy
// in general is that no allocated data requires explicit construction.
// It means that the allocator can be really simple; you can even
// replace new/delete to malloc/free. The constructors and destructors
// won't be called in this case, however everything will remain working.
// The second argument of deallocate() is the size of the allocated
// block. You can use this information if you wish.
//------------------------------------------------------------pod_allocator
template <class T> struct PodAllocator {
    static T* Allocate(unsigned num)
    {
        return new T[num];
    }
    static void Deallocate(T* ptr, unsigned)
    {
        delete[] ptr;
    }
};

// Single object allocator. It's also can be replaced with your custom
// allocator. The difference is that it can only allocate a single
// object and the constructor and destructor must be called.
// In AGG there is no need to allocate an array of objects with
// calling their constructors (only single ones). So that, if you
// replace these new/delete to malloc/free make sure that the in-place
// new is called and take care of calling the destructor too.
//------------------------------------------------------------obj_allocator
template <class T> struct ObjAllocator {
    static T* Allocate()
    {
        return new T;
    }
    static void Deallocate(T* ptr)
    {
        delete ptr;
    }
};
} // namespace OHOS
#endif

//-------------------------------------------------------- Default basic types
//
// If the compiler has different capacity of the basic types you can redefine
// them via the compiler command line or by generating agg_config.h that is
// empty by default.
//
#ifndef AGG_INT8
#define AGG_INT8 signed char
#endif

#ifndef AGG_INT8U
#define AGG_INT8U unsigned char
#endif

#ifndef AGG_INT16
#define AGG_INT16 short
#endif

#ifndef AGG_INT16U
#define AGG_INT16U unsigned short
#endif

#ifndef AGG_INT32
#define AGG_INT32 int
#endif

#ifndef AGG_INT32U
#define AGG_INT32U unsigned
#endif

#ifndef AGG_INT64
#if defined(_MSC_VER) || defined(__BORLANDC__)
#define AGG_INT64 signed __int64
#else
#define AGG_INT64 signed long long
#endif
#endif

#ifndef AGG_INT64U
#if defined(_MSC_VER) || defined(__BORLANDC__)
#define AGG_INT64U unsigned __int64
#else
#define AGG_INT64U unsigned long long
#endif
#endif

//------------------------------------------------ Some fixes for MS Visual C++
#if defined(_MSC_VER)
#pragma warning(disable : 4786) // Identifier was truncated...
#endif

#if defined(_MSC_VER)
#define AGG_INLINE __forceinline
#else
#define AGG_INLINE inline
#endif

namespace OHOS {
//-------------------------------------------------------------------------
typedef AGG_INT8 int8;     //----int8
typedef AGG_INT8U int8u;   //----int8u
typedef AGG_INT16 int16;   //----int16
typedef AGG_INT16U int16u; //----int16u
typedef AGG_INT32 int32;   //----int32
typedef AGG_INT32U int32u; //----int32u
typedef AGG_INT64 int64;   //----int64
typedef AGG_INT64U int64u; //----int64u

#if defined(AGG_FISTP)
#pragma warning(push)
#pragma warning(disable : 4035) // Disable warning "no return value"
AGG_INLINE int Iround(double v) //-------iround
{
    int t;
    __asm fld qword ptr[v] __asm fistp dword ptr[t] __asm mov eax, dword ptr[t]
}
AGG_INLINE unsigned Uround(double v) //-------uround
{
    unsigned t;
    __asm fld qword ptr[v] __asm fistp dword ptr[t] __asm mov eax, dword ptr[t]
}
#pragma warning(pop)
AGG_INLINE int Ifloor(double v)
{
    return int(Floor(v));
}
AGG_INLINE unsigned Ufloor(double v) //-------ufloor
{
    return unsigned(Floor(v));
}
AGG_INLINE int Iceil(double v)
{
    return int(Ceil(v));
}
AGG_INLINE unsigned Uceil(double v) //--------uceil
{
    return unsigned(Ceil(v));
}
#elif defined(AGG_QIFIST)
AGG_INLINE int Iround(double v)
{
    return int(v);
}
AGG_INLINE int Uround(double v)
{
    return unsigned(v);
}
AGG_INLINE int Ifloor(double v)
{
    return int(std::floor(v));
}
AGG_INLINE unsigned Ufloor(double v)
{
    return unsigned(std::floor(v));
}
AGG_INLINE int Iceil(double v)
{
    return int(std::ceil(v));
}
AGG_INLINE unsigned Uceil(double v)
{
    return unsigned(std::ceil(v));
}
#else
AGG_INLINE int Iround(double v)
{
    return int((v < 0.0) ? v - 0.5 : v + 0.5);
}
AGG_INLINE int Uround(double v)
{
    return unsigned(v + 0.5);
}
AGG_INLINE int Ifloor(double v)
{
    int i = int(v);
    return i - (i > v);
}
AGG_INLINE unsigned Ufloor(double v)
{
    return unsigned(v);
}
AGG_INLINE int Iceil(double v)
{
    return int(std::ceil(v));
}
AGG_INLINE unsigned Uceil(double v)
{
    return unsigned(std::ceil(v));
}
#endif

//---------------------------------------------------------------Saturation
template <int Limit> struct Saturation {
    AGG_INLINE static int Iround(double v)
    {
        if (v < double(-Limit))
            return -Limit;
        if (v > double(Limit))
            return Limit;
        return OHOS::Iround(v);
    }
};

//------------------------------------------------------------------MulOne
template <unsigned Shift> struct MulOne {
    AGG_INLINE static unsigned Mul(unsigned a, unsigned b)
    {
        unsigned q = a * b + (1 << (Shift - 1));
        return (q + (q >> Shift)) >> Shift;
    }
};

//-------------------------------------------------------------------------
typedef unsigned char CoverType; //----CoverType
enum CoverScaleE {
    COVER_SHIFT = 8,               //----cover_shift
    COVER_SIZE = 1 << COVER_SHIFT, //----cover_size
    COVER_MASK = COVER_SIZE - 1,   //----cover_mask
    COVER_NONE = 0,                //----cover_none
    COVER_FULL = COVER_MASK        //----cover_full
};

//----------------------------------------------------PolySubpixelScaleE
// These constants determine the subpixel accuracy, to be more precise,
// the number of bits of the fractional part of the coordinates.
// The possible coordinate capacity in bits can be calculated by formula:
// sizeof(int) * 8 - poly_subpixel_shift, i.e, for 32-bit integers and
// 8-bits fractional part the capacity is 24 bits.
enum PolySubpixelScaleE {
    // poly_subpixel_shift = 8,                      //----poly_subpixel_shift
    // poly_subpixel_scale = 1<<poly_subpixel_shift, //----poly_subpixel_scale
    // poly_subpixel_mask  = poly_subpixel_scale-1   //----poly_subpixel_mask
    POLY_SUBPIXEL_SHIFT = 8,                        //----poly_subpixel_shift
    POLY_SUBPIXEL_SCALE = 1 << POLY_SUBPIXEL_SHIFT, //----poly_subpixel_scale
    POLY_SUBPIXEL_MASK = POLY_SUBPIXEL_SCALE - 1    //----poly_subpixel_mask
};

//----------------------------------------------------------FillingRuleE
enum FillingRuleE {
    /*fill_non_zero,
    fill_even_odd*/
    FILL_NON_ZERO,
    FILL_EVEN_ODD
};

//-----------------------------------------------------------------------pi
const double pi = 3.14159265358979323846;

//------------------------------------------------------------------Deg2Rad
inline double Deg2Rad(double deg)
{
    return deg * pi / 180.0;
}

//------------------------------------------------------------------Rad2Deg
inline double Rad2Deg(double rad)
{
    return rad * 180.0 / pi;
}

//----------------------------------------------------------------RectBase
template <class T> struct RectBase {
    typedef T ValueType;
    typedef RectBase<T> SelfType;
    T x1, y1, x2, y2;

    RectBase() {}
    RectBase(T x1_, T y1_, T x2_, T y2_) : x1(x1_), y1(y1_), x2(x2_), y2(y2_) {}

    void Init(T x1_, T y1_, T x2_, T y2_)
    {
        x1 = x1_;
        y1 = y1_;
        x2 = x2_;
        y2 = y2_;
    }

    const SelfType& Normalize()
    {
        T t;
        if (x1 > x2) {
            t = x1;
            x1 = x2;
            x2 = t;
        }
        if (y1 > y2) {
            t = y1;
            y1 = y2;
            y2 = t;
        }
        return *this;
    }

    bool Clip(const SelfType& r)
    {
        if (x2 > r.x2)
            x2 = r.x2;
        if (y2 > r.y2)
            y2 = r.y2;
        if (x1 < r.x1)
            x1 = r.x1;
        if (y1 < r.y1)
            y1 = r.y1;
        return x1 <= x2 && y1 <= y2;
    }

    bool IsValid() const
    {
        return x1 <= x2 && y1 <= y2;
    }

    bool HitTest(T x, T y) const
    {
        return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
    }

    bool Overlaps(const SelfType& r) const
    {
        return !(r.x1 > x2 || r.x2 < x1 || r.y1 > y2 || r.y2 < y1);
    }
};

//-----------------------------------------------------IntersectRectangles
template <class Rect> inline Rect IntersectRectangles(const Rect& r1, const Rect& r2)
{
    Rect r = r1;

    // First process x2,y2 because the other order
    // results in Internal Compiler Error under
    // Microsoft Visual C++ .NET 2003 69462-335-0000007-18038 in
    // case of "Maximize Speed" optimization option.
    //-----------------
    if (r.x2 > r2.x2)
        r.x2 = r2.x2;
    if (r.y2 > r2.y2)
        r.y2 = r2.y2;
    if (r.x1 < r2.x1)
        r.x1 = r2.x1;
    if (r.y1 < r2.y1)
        r.y1 = r2.y1;
    return r;
}

//---------------------------------------------------------UniteRectangles
template <class Rect> inline Rect UniteRectangles(const Rect& r1, const Rect& r2)
{
    Rect r = r1;
    if (r.x2 < r2.x2)
        r.x2 = r2.x2;
    if (r.y2 < r2.y2)
        r.y2 = r2.y2;
    if (r.x1 > r2.x1)
        r.x1 = r2.x1;
    if (r.y1 > r2.y1)
        r.y1 = r2.y1;
    return r;
}

typedef RectBase<int> RectI;    //----rect_i
typedef RectBase<float> RectF;  //----rect_f
typedef RectBase<double> RectD; //----rect_d

//---------------------------------------------------------PathCommandsE
enum PathCommandsE {
    // path_cmd_stop     = 0,        //----path_cmd_stop
    // path_cmd_move_to  = 1,        //----path_cmd_move_to
    // path_cmd_line_to  = 2,        //----path_cmd_line_to
    // path_cmd_curve3   = 3,        //----path_cmd_curve3
    // path_cmd_curve4   = 4,        //----path_cmd_curve4
    // path_cmd_curveN   = 5,        //----path_cmd_curveN
    // path_cmd_catrom   = 6,        //----path_cmd_catrom
    // path_cmd_ubspline = 7,        //----path_cmd_ubspline
    // path_cmd_end_poly = 0x0F,     //----path_cmd_end_poly
    // path_cmd_mask     = 0x0F      //----path_cmd_mask
    PATH_CMD_STOP = 0,        //----path_cmd_stop
    PATH_CMD_MOVE_TO = 1,     //----path_cmd_move_to
    PATH_CMD_LINE_TO = 2,     //----path_cmd_line_to
    PATH_CMD_CURVE3 = 3,      //----path_cmd_curve3
    PATH_CMD_CURVE4 = 4,      //----path_cmd_curve4
    PATH_CMD_CURVEN = 5,      //----path_cmd_curveN
    PATH_CMD_CARROM = 6,      //----path_cmd_catrom
    PATH_CMD_UBSPLINE = 7,    //----path_cmd_ubspline
    PATH_CMD_END_POLY = 0x0F, //----path_cmd_end_poly
    PATH_CMD_MASK = 0x0F      //----path_cmd_mask
};

//------------------------------------------------------------PathFlagsE
enum PathFlagsE {
    // path_flags_none  = 0,         //----path_flags_none
    // path_flags_ccw   = 0x10,      //----path_flags_ccw
    // path_flags_cw    = 0x20,      //----path_flags_cw
    // path_flags_close = 0x40,      //----path_flags_close
    // path_flags_mask  = 0xF0       //----path_flags_mask
    PATH_FLAGS_NONE = 0,     //----PATH_FLAGS_NONE
    PATH_FLAGS_CCW = 0x10,   //----PATH_FLAGS_CCW
    PATH_FLAGS_CW = 0x20,    //----PATH_FLAGS_CW
    PATH_FLAGS_CLOSE = 0x40, //----PATH_FLAGS_CLOSE
    PATH_FLAGS_MASK = 0xF0   //----PATH_FLAGS_MASK
};

//---------------------------------------------------------------IsVertex
inline bool IsVertex(unsigned c)
{
    return c >= PATH_CMD_MOVE_TO && c < PATH_CMD_END_POLY;
}

//--------------------------------------------------------------IsDrawing
inline bool IsDrawing(unsigned c)
{
    return c >= PATH_CMD_LINE_TO && c < PATH_CMD_END_POLY;
}

//-----------------------------------------------------------------IsStop
inline bool IsStop(unsigned c)
{
    return c == PATH_CMD_STOP;
}

//--------------------------------------------------------------IsMoveTo
inline bool IsMoveTo(unsigned c)
{
    return c == PATH_CMD_MOVE_TO;
}

//--------------------------------------------------------------IsLineTo
inline bool IsLineTo(unsigned c)
{
    return c == PATH_CMD_LINE_TO;
}

//----------------------------------------------------------------IsCurve
inline bool IsCurve(unsigned c)
{
    return c == PATH_CMD_CURVE3 || c == PATH_CMD_CURVE4;
}

//---------------------------------------------------------------IsCurve3
inline bool IsCurve3(unsigned c)
{
    return c == PATH_CMD_CURVE3;
}

//---------------------------------------------------------------IsCurve4
inline bool IsCurve4(unsigned c)
{
    return c == PATH_CMD_CURVE4;
}

//-------------------------------------------------------------IsEnd_poly
inline bool IsEndPoly(unsigned c)
{
    return (c & PATH_CMD_MASK) == PATH_CMD_END_POLY;
}

//----------------------------------------------------------------IsClose
inline bool IsClose(unsigned c)
{
    return (c & ~(PATH_FLAGS_CW | PATH_FLAGS_CCW)) == (PATH_CMD_END_POLY | PATH_FLAGS_CLOSE);
}

//------------------------------------------------------------IsNextPoly
inline bool IsNextPoly(unsigned c)
{
    return IsStop(c) || IsMoveTo(c) || IsEndPoly(c);
}

//-------------------------------------------------------------------IsCw
inline bool IsCw(unsigned c)
{
    return (c & PATH_FLAGS_CW) != 0;
}

//------------------------------------------------------------------IsCcw
inline bool IsCcw(unsigned c)
{
    return (c & PATH_FLAGS_CCW) != 0;
}

//-------------------------------------------------------------IsOriented
inline bool IsOriented(unsigned c)
{
    return (c & (PATH_FLAGS_CW | PATH_FLAGS_CCW)) != 0;
}

//---------------------------------------------------------------IsClosed
inline bool IsClosed(unsigned c)
{
    return (c & PATH_FLAGS_CLOSE) != 0;
}

//----------------------------------------------------------GetCloseFlag
inline unsigned GetCloseFlag(unsigned c)
{
    return c & PATH_FLAGS_CLOSE;
}

//-------------------------------------------------------ClearOrientation
inline unsigned ClearOrientation(unsigned c)
{
    return c & ~(PATH_FLAGS_CW | PATH_FLAGS_CCW);
}

//---------------------------------------------------------GetOrientation
inline unsigned GetOrientation(unsigned c)
{
    return c & (PATH_FLAGS_CW | PATH_FLAGS_CCW);
}

//---------------------------------------------------------SetOrientation
inline unsigned SetOrientation(unsigned c, unsigned o)
{
    return ClearOrientation(c) | o;
}

//--------------------------------------------------------------PointBase
template <class T> struct PointBase {
    typedef T ValueType;
    T x, y;
    PointBase() {}
    PointBase(T x_, T y_) : x(x_), y(y_) {}
};
typedef PointBase<int> PointI;    //-----PointI
typedef PointBase<float> PointF;  //-----PointF
typedef PointBase<double> PointD; //-----PointD

//-------------------------------------------------------------VertexBase
template <class T> struct VertexBase {
    typedef T value_type;
    T x, y;
    unsigned cmd;
    VertexBase() {}
    VertexBase(T x_, T y_, unsigned cmd_) : x(x_), y(y_), cmd(cmd_) {}
};
typedef VertexBase<int> VertexI;    //-----VertexI
typedef VertexBase<float> VertexF;  //-----VertexF
typedef VertexBase<double> VertexD; //-----VertexD

//----------------------------------------------------------------RowInfo
template <class T> struct RowInfo {
    int x1, x2;
    T* ptr;
    RowInfo() {}
    RowInfo(int x1_, int x2_, T* ptr_) : x1(x1_), x2(x2_), ptr(ptr_) {}
};

//----------------------------------------------------------const_row_info
template <class T> struct ConstRowInfo {
    int x1, x2;
    const T* ptr;
    ConstRowInfo() {}
    ConstRowInfo(int x1_, int x2_, const T* ptr_) : x1(x1_), x2(x2_), ptr(ptr_) {}
};

//------------------------------------------------------------is_equal_eps
template <class T> inline bool IsEqualEps(T v1, T v2, T epsilon)
{
    bool neg1 = v1 < 0.0;
    bool neg2 = v2 < 0.0;

    if (neg1 != neg2)
        return std::fabs(v1) < epsilon && std::fabs(v2) < epsilon;

    int int1, int2;
    std::frexp(v1, &int1);
    std::frexp(v2, &int2);
    int min12 = int1 < int2 ? int1 : int2;

    v1 = std::ldexp(v1, -min12);
    v2 = std::ldexp(v2, -min12);

    return std::fabs(v1 - v2) < epsilon;
}
} // namespace OHOS

#endif
