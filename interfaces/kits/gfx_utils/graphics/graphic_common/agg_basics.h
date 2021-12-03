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

/**
 * @addtogroup GraphicGeometry
 * @{
 *
 * @brief Defines 图形操作
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file graphic_geometry_basics.h
 *
 * @brief Defines 图形操作基本数据类型.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_BASICS_INCLUDED
#define GRAPHIC_GEOMETRY_BASICS_INCLUDED

#include <cmath>

#ifdef GRAPHIC_GEOMETRY_CUSTOM_ALLOCATOR
#include "graphic_geometry_allocator.h"
#else
namespace OHOS {
template <class T> 
struct ObjAllocator {
    static T* Allocate()
    {
        return new T;
    }
    static void Deallocate(T* ptr)
    {
        delete ptr;
    }
};

template <class T> 
struct ArrAllocator {
    static T* Allocate(unsigned num)
    {
        return new T[num];
    }
    static void Deallocate(T* ptr, unsigned)
    {
        delete[] ptr;
    }
};
} // namespace OHOS
#endif

#ifndef GRAPHIC_GEOMETRY_INT8
#define GRAPHIC_GEOMETRY_INT8 signed char
#endif

#ifndef GRAPHIC_GEOMETRY_INT8U
#define GRAPHIC_GEOMETRY_INT8U unsigned char
#endif

#ifndef GRAPHIC_GEOMETRY_INT16
#define GRAPHIC_GEOMETRY_INT16 short
#endif

#ifndef GRAPHIC_GEOMETRY_INT16U
#define GRAPHIC_GEOMETRY_INT16U unsigned short
#endif

#ifndef GRAPHIC_GEOMETRY_INT32
#define GRAPHIC_GEOMETRY_INT32 int
#endif

#ifndef GRAPHIC_GEOMETRY_INT32U
#define GRAPHIC_GEOMETRY_INT32U unsigned
#endif

#ifndef GRAPHIC_GEOMETRY_INT64
#define GRAPHIC_GEOMETRY_INT64 signed __int64
#else
#define GRAPHIC_GEOMETRY_INT64 signed long long
#endif


#ifndef GRAPHIC_GEOMETRY_INT64U
#define GRAPHIC_GEOMETRY_INT64U unsigned __int64
#else
#define GRAPHIC_GEOMETRY_INT64U unsigned long long
#endif

//------------------------------------------------ Some fixes for MS Visual C++
#if defined(_MSC_VER)
#pragma warning(disable : 4786) // Identifier was truncated...
#endif

//#if defined(_MSC_VER)
//#define GRAPHIC_GEOMETRY_INLINE __forceinline
//#else
//#define GRAPHIC_GEOMETRY_INLINE inline
//#endif

#define GRAPHIC_GEOMETRY_INLINE inline

namespace OHOS {
using int8 = GRAPHIC_GEOMETRY_INT8;
using int8u = GRAPHIC_GEOMETRY_INT8U;
using int16 = GRAPHIC_GEOMETRY_INT16;
using int16u = GRAPHIC_GEOMETRY_INT16U;
using int32 = GRAPHIC_GEOMETRY_INT32;
using int32u = GRAPHIC_GEOMETRY_INT32U;
using int64 = GRAPHIC_GEOMETRY_INT64;
using int64u = GRAPHIC_GEOMETRY_INT64U;
#if defined(GRAPHIC_GEOMETRY_FISTP)
#pragma warning(push)
#pragma warning(disable : 4035) // Disable warning "no return value"
GRAPHIC_GEOMETRY_INLINE int Iround(double v) //-------iround
{
    int t;
    __asm fld qword ptr[v] __asm fistp dword ptr[t] __asm mov eax, dword ptr[t]
}
GRAPHIC_GEOMETRY_INLINE unsigned Uround(double v) //-------uround
{
    unsigned t;
    __asm fld qword ptr[v] __asm fistp dword ptr[t] __asm mov eax, dword ptr[t]
}
#pragma warning(pop)
GRAPHIC_GEOMETRY_INLINE int Ifloor(double val)
{
    return int(Floor(val));
}
GRAPHIC_GEOMETRY_INLINE unsigned Ufloor(double val) //-------ufloor
{
    return unsigned(Floor(val));
}
GRAPHIC_GEOMETRY_INLINE int Iceil(double val)
{
    return int(Ceil(val));
}
GRAPHIC_GEOMETRY_INLINE unsigned Uceil(double val) //--------uceil
{
    return unsigned(Ceil(val));
}
#elif defined(GRAPHIC_GEOMETRY_QIFIST)
GRAPHIC_GEOMETRY_INLINE int Iround(double val)
{
    return int(val);
}
GRAPHIC_GEOMETRY_INLINE int Uround(double val)
{
    return unsigned(val);
}
GRAPHIC_GEOMETRY_INLINE int Ifloor(double val)
{
    return int(std::floor(val));
}
GRAPHIC_GEOMETRY_INLINE unsigned Ufloor(double val)
{
    return unsigned(std::floor(val));
}
GRAPHIC_GEOMETRY_INLINE int Iceil(double val)
{
    return int(std::ceil(val));
}
GRAPHIC_GEOMETRY_INLINE unsigned Uceil(double val)
{
    return unsigned(std::ceil(val));
}
#else
GRAPHIC_GEOMETRY_INLINE int Iround(double val)
{
    return int((val < 0.0) ? val - 0.5 : val + 0.5);
}
GRAPHIC_GEOMETRY_INLINE int Uround(double val)
{
    return unsigned(val + 0.5);
}
GRAPHIC_GEOMETRY_INLINE int Ifloor(double val)
{
    int i = int(val);
    return i - (i > val);
}
GRAPHIC_GEOMETRY_INLINE unsigned Ufloor(double val)
{
    return unsigned(val);
}
GRAPHIC_GEOMETRY_INLINE int Iceil(double val)
{
    return int(std::ceil(val));
}
GRAPHIC_GEOMETRY_INLINE unsigned Uceil(double val)
{
    return unsigned(std::ceil(val));
}
#endif

template <int Limit>
struct Saturation {
    GRAPHIC_GEOMETRY_INLINE static int Iround(double roundVal)
    {
        if (roundVal < double(-Limit)) {
            return -Limit;
        } 
        if (roundVal > double(Limit)) {
            return Limit;
        }
        return OHOS::Iround(roundVal);
    }
};

template <unsigned Shift> 
struct MulOne {
    GRAPHIC_GEOMETRY_INLINE static unsigned Mul(unsigned val1, unsigned val2)
    {
        unsigned q = val1 * val2 + (1 << (Shift - 1));
        return (q + (q >> Shift)) >> Shift;
    }
};

using CoverType = unsigned char;

enum FillingRuleE {
    FILL_NON_ZERO,
    FILL_EVEN_ODD
};

enum PolySubpixelScaleE {
    POLY_SUBPIXEL_SHIFT = 8,
    POLY_SUBPIXEL_SCALE = 1 << POLY_SUBPIXEL_SHIFT,
    POLY_SUBPIXEL_MASK = POLY_SUBPIXEL_SCALE - 1
};

enum CoverScaleE {
    COVER_SHIFT = 8,
    COVER_SIZE = 1 << COVER_SHIFT,
    COVER_MASK = COVER_SIZE - 1,
    COVER_NONE = 0,
    COVER_FULL = COVER_MASK
};

const double PI = 3.14159265358979323846;

inline double Rad2Deg(double val)
{
    return val * 180.0 / PI;
}

inline double Deg2Rad(double val)
{
    return val * PI / 180.0;
}

template <class T>
struct RectBase {
    using ValueType = T;
    using SelfType = RectBase<T>;
    T x1;
    T y1;
    T x2;
    T y2;
    
    RectBase(T x1_, T y1_, T x2_, T y2_) : x1(x1_), y1(y1_), x2(x2_), y2(y2_) {}
    RectBase() {}

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

    void Init(T x1_, T y1_, T x2_, T y2_)
    {
        x1 = x1_;
        y1 = y1_;
        x2 = x2_;
        y2 = y2_;
    }

    bool Clip(const SelfType& r)
    {
        if (x2 > r.x2) {
            x2 = r.x2;
        } 
        if (y2 > r.y2) {
            y2 = r.y2;
        }  
        if (x1 < r.x1) {
            x1 = r.x1;
        }   
        if (y1 < r.y1) {
            y1 = r.y1;
        } 
        return y1 <= y2 && x1 <= x2;
    }

    bool IsValid() const
    {
        return y1 <= y2 && x1 <= x2;
    }

    bool Overlaps(const SelfType& r) const
    {
        return !(r.y1 > y2 || r.y2<y1 || r.x1> x2 || r.x2 < x1);
    }

    bool HitTest(T x, T y) const
    {
        return (y >= y1 && y <= y2 && x >= x1 && x <= x2);
    }

};

template <class Rect> 
inline Rect IntersectRectangles(const Rect& r1, const Rect& r2)
{
    Rect r = r1;
    if (r.x2 > r2.x2) {
        r.x2 = r2.x2;
    }
    if (r.y2 > r2.y2) {
        r.y2 = r2.y2;
    }  
    if (r.x1 < r2.x1) {
        r.x1 = r2.x1;
    } 
    if (r.y1 < r2.y1) {
        r.y1 = r2.y1;
    }
    return r;
}

template <class Rect> 
inline Rect UniteRectangles(const Rect& r1, const Rect& r2)
{
    Rect r = r1;
    if (r.x2 < r2.x2) {
        r.x2 = r2.x2;
    }
    if (r.y2 < r2.y2) {
        r.y2 = r2.y2;
    }
    if (r.x1 > r2.x1) {
        r.x1 = r2.x1;
    }
    if (r.y1 > r2.y1) {
        r.y1 = r2.y1;
    }
    return r;
}

using RectI = RectBase<int>;
using RectF = RectBase<float>;
using RectD = RectBase<double>;

enum PathCommandsE {
    PATH_CMD_STOP = 0,        
    PATH_CMD_MOVE_TO = 1,     
    PATH_CMD_LINE_TO = 2,     
    PATH_CMD_CURVE3 = 3,      
    PATH_CMD_CURVE4 = 4,      
    PATH_CMD_CURVEN = 5,      
    PATH_CMD_CARROM = 6,      
    PATH_CMD_UBSPLINE = 7,    
    PATH_CMD_END_POLY = 0x0F, 
    PATH_CMD_MASK = 0x0F      
};

enum PathFlagsE {
    PATH_FLAGS_NONE = 0,     
    PATH_FLAGS_CCW = 0x10,   
    PATH_FLAGS_CW = 0x20,    
    PATH_FLAGS_CLOSE = 0x40, 
    PATH_FLAGS_MASK = 0xF0   
};

inline bool IsDrawing(unsigned val)
{
    return val < PATH_CMD_END_POLY && val >= PATH_CMD_LINE_TO;
}

inline bool IsVertex(unsigned val)
{
    return val < PATH_CMD_END_POLY && val >= PATH_CMD_MOVE_TO;
}

inline bool IsMoveTo(unsigned val)
{
    return PATH_CMD_MOVE_TO == val;
}

inline bool IsStop(unsigned val)
{
    return PATH_CMD_STOP == val;
}

inline bool IsLineTo(unsigned val)
{
    return PATH_CMD_LINE_TO == val;
}

inline bool IsCurve(unsigned val)
{
    return PATH_CMD_CURVE4 == val ||  PATH_CMD_CURVE3 == val;
}
inline bool IsCurve3(unsigned val)
{
    return PATH_CMD_CURVE3 == val;
}

inline bool IsCurve4(unsigned val)
{
    return PATH_CMD_CURVE4 == val;
}

inline bool IsEndPoly(unsigned val)
{
    return PATH_CMD_END_POLY == (val & PATH_CMD_MASK);
}

inline bool IsClose(unsigned val)
{
    return (val & ~(PATH_FLAGS_CW | PATH_FLAGS_CCW)) == (PATH_CMD_END_POLY | PATH_FLAGS_CLOSE);
}

inline bool IsNextPoly(unsigned val)
{
    return IsStop(val) || IsMoveTo(val) || IsEndPoly(val);
}

inline bool IsCw(unsigned val)
{
    return 0 ! = (val & PATH_FLAGS_CW);
}

inline bool IsCcw(unsigned val)
{
    return (val & PATH_FLAGS_CCW) != 0;
}

inline bool IsOriented(unsigned val)
{
    return (val & (PATH_FLAGS_CW | PATH_FLAGS_CCW)) != 0;
}

inline bool IsClosed(unsigned val)
{
    return (val & PATH_FLAGS_CLOSE) != 0;
}

inline unsigned ClearOrientation(unsigned val)
{
    return val & ~(PATH_FLAGS_CW | PATH_FLAGS_CCW);
}

inline unsigned GetCloseFlag(unsigned val)
{
    return val & PATH_FLAGS_CLOSE;
}

inline unsigned SetOrientation(unsigned val, unsigned val1)
{
    return ClearOrientation(val) | val1;
}

inline unsigned GetOrientation(unsigned val)
{
    return val & (PATH_FLAGS_CW | PATH_FLAGS_CCW);
}

template <class T> 
struct PointBase {
    using ValueType = T;
    T x;
    T y;
    PointBase() {}
    PointBase(T x_, T y_) : x(x_), y(y_) {}
};
using PointF = PointBase<float>;  
using PointD = PointBase<double>; 
using PointI = PointBase<int>;    

template <class T> 
struct VertexBase {
    using ValueType = T;
    T x;
    T y;
    unsigned cmd;
    VertexBase() {}
    VertexBase(T x_, T y_, unsigned cmd_) : x(x_), y(y_), cmd(cmd_) {}
};

using VertexF = VertexBase<float> ; 
using VertexD = VertexBase<double>; 
using VertexI = VertexBase<int>;    

template <class T> 
struct ConstRowInfo {
    int x1;
    int x2;
    const T* ptr;
    ConstRowInfo() {}
    ConstRowInfo(int x1_, int x2_, const T* ptr_) : x1(x1_), x2(x2_), ptr(ptr_) {}
};

template <class T> 
struct RowInfo {
    int x1;
    int x2;
    T* ptr;
    RowInfo() {}
    RowInfo(int x1_, int x2_, T* ptr_) : x1(x1_), x2(x2_), ptr(ptr_) {}
};

template <class T> 
inline bool IsEqualEps(T val1, T val2, T epsilon)
{
    bool neg1 = val1 < 0.0;
    bool neg2 = val2 < 0.0;

    if (neg1 != neg2) {
        return std::fabs(val1) < epsilon && std::fabs(val2) < epsilon;
    }

    int int1;
    int int2;
    std::frexp(val1, &int1);
    std::frexp(val2, &int2);
    int min12 = int1 < int2 ? int1 : int2;

    val1 = std::ldexp(val1, -min12);
    val2 = std::ldexp(val2, -min12);

    return std::fabs(val1 - val2) < epsilon;
}
} // namespace OHOS

#endif
