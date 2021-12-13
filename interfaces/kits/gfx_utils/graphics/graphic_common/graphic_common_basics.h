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
 * @brief Defines 图形操作基本数据类型与通用函数.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_BASICS_INCLUDED
#define GRAPHIC_GEOMETRY_BASICS_INCLUDED

#include <cmath>

#include "securec.h"
namespace OHOS {
#ifndef GRAPHIC_GEOMETRY_INT8
#define GRAPHIC_GEOMETRY_INT8 signed char
#endif

#ifndef GRAPHIC_GEOMETRY_INT8U
#    define GRAPHIC_GEOMETRY_INT8U unsigned char
#endif

#ifndef GRAPHIC_GEOMETRY_INT16
#    define GRAPHIC_GEOMETRY_INT16 short
#endif

#ifndef GRAPHIC_GEOMETRY_INT16U
#    define GRAPHIC_GEOMETRY_INT16U unsigned short
#endif

#ifndef GRAPHIC_GEOMETRY_INT32
#    define GRAPHIC_GEOMETRY_INT32 int
#endif

#ifndef GRAPHIC_GEOMETRY_INT32U
#    define GRAPHIC_GEOMETRY_INT32U unsigned
#endif

#ifndef GRAPHIC_GEOMETRY_INT64
#   define GRAPHIC_GEOMETRY_INT64 signed long long
#endif

#ifndef GRAPHIC_GEOMETRY_INT64U
#   define GRAPHIC_GEOMETRY_INT64U unsigned long long
#endif

#define GRAPHIC_GEOMETRY_INLINE inline

    /**
 * @brief 填充规则.
 * @since 1.0
 * @version 1.0
 */
    enum FillingRuleEnum
    {
        FILL_NON_ZERO,
        FILL_EVEN_ODD
    };

    /**
 * @brief 子像素的偏移以及掩码标志.
 * @since 1.0
 * @version 1.0
 */
    enum PolySubpixelScaleEnum
    {
        POLY_SUBPIXEL_SHIFT = 8,
        POLY_SUBPIXEL_SCALE = 1 << POLY_SUBPIXEL_SHIFT,
        POLY_SUBPIXEL_MASK = POLY_SUBPIXEL_SCALE - 1
    };

    /**
 * @brief 覆盖率的弹性处理.
 * @since 1.0
 * @version 1.0
 */
    enum CoverScaleEnum
    {
        COVER_SHIFT = 8,
        COVER_SIZE = 1 << COVER_SHIFT,
        COVER_MASK = COVER_SIZE - 1,
        COVER_NONE = 0,
        COVER_FULL = COVER_MASK
    };

    enum PathCommandsEnum
    {
        PATH_CMD_STOP = 0,
        PATH_CMD_MOVE_TO = 1,
        PATH_CMD_LINE_TO = 2,
        PATH_CMD_CURVE3 = 3,      //二次曲线命令
        PATH_CMD_CURVE4 = 4,      //三次曲线命令
        PATH_CMD_CURVEN = 5,      //曲线命令
        PATH_CMD_CARROM = 6,      //二次曲线命令
        PATH_CMD_UBSPLINE = 7,    //曲线命令
        PATH_CMD_END_POLY = 0x0F, //多边形闭合
        PATH_CMD_MASK = 0x0F
    };

    enum PathFlagsEnum
    {
        PATH_FLAGS_NONE = 0,
        PATH_FLAGS_CCW = 0x10, //顺时针
        PATH_FLAGS_CW = 0x20,  //逆时针
        PATH_FLAGS_CLOSE = 0x40,
        PATH_FLAGS_MASK = 0xF0
    };

    enum cover_scale_e
    {
        cover_shift = 8,               //----cover_shift
        cover_size = 1 << cover_shift, //----cover_size
        cover_mask = cover_size - 1,   //----cover_mask
        cover_none = 0,                //----cover_none
        cover_full = cover_mask        //----cover_full
    };

    enum poly_subpixel_scale_e
    {
        poly_subpixel_shift = 8,                        //----poly_subpixel_shift
        poly_subpixel_scale = 1 << poly_subpixel_shift, //----poly_subpixel_scale
        poly_subpixel_mask = poly_subpixel_scale - 1    //----poly_subpixel_mask
    };

    enum gradient_subpixel_scale_e
    {
        gradient_subpixel_shift = 4,
        gradient_subpixel_scale = 1 << gradient_subpixel_shift,
        gradient_subpixel_mask = gradient_subpixel_scale - 1
    };

    enum filling_rule_e
    {
        fill_non_zero,
        fill_even_odd
    };

    const double PI = 3.14159265358979323846;
    const double INFINITE_SIMAL = 1e-16;
    const double WIDTH = 0.5;
    const double WIDTH_ABS = 0.5;
    const double WIDTH_EPS = 0.5/1024.0;
    const double WIDTH_SIGN = 1;
    const double MITER_LIMIT = 4.0;
    const double APPROX_SCALE = 1.0;
    const double CIRCLE_CENTER_X = 0.0;
    const double CIRCLE_CENTER_Y = 0.0;
    const double CIRCLE_RADIUS_X = 1.0;
    const double CIRCLE_RADIUS_Y = 1.0;
    const double SCALE = 1.0;
    const double NUM = 4;
    const double STEP = 0;
    const double CLOCKWISE = false;
    const double CONSTANT_1 = 0.125;
    const double CONSTANT_2 = 2.0;
    const double CONSTANT_3 = 2;
    const double CONSTANT_4 = 0.5;
    const double CONSTANT_5 = 1024.0;

    using int8 = GRAPHIC_GEOMETRY_INT8;
    using int8u = GRAPHIC_GEOMETRY_INT8U;
    using int16 = GRAPHIC_GEOMETRY_INT16;
    using int16u = GRAPHIC_GEOMETRY_INT16U;
    using int32 = GRAPHIC_GEOMETRY_INT32;
    using int32u = GRAPHIC_GEOMETRY_INT32U;
    using int64 = GRAPHIC_GEOMETRY_INT64;
    using int64u = GRAPHIC_GEOMETRY_INT64U;

#if defined(GRAPHIC_GEOMETRY_FISTP)
#    pragma warning(push)
#    pragma warning(disable : 4035) // Disable warning "no return value"
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
#    pragma warning(pop)
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
    GRAPHIC_GEOMETRY_INLINE int Iceil(double val)
    {
        return int(std::ceil(val));
    }
    GRAPHIC_GEOMETRY_INLINE unsigned Uceil(double val)
    {
        return unsigned(std::ceil(val));
    }
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
#endif
    /**
     * @brief 两个数是否相近.
     *
     * @param val1,val2 两个数,epsilon 误差.
     * @return Returns 两个数是否相近.
     *@since 1.0
     * @version 1.0
     */
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

    /**
     * @brief 弧度转度.
     * @since 1.0
     * @version 1.0
     */
    inline double Rad2Deg(double val)
    {
        return val * 180.0 / PI;
    }
    /**
     * @brief 度转弧度.
     * @since 1.0
     * @version 1.0
     */
    inline double Deg2Rad(double val)
    {
        return val * PI / 180.0;
    }

    /**
     * @brief 判断是否在绘制图元.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsDrawing(unsigned val)
    {
        return val < PATH_CMD_END_POLY && val >= PATH_CMD_LINE_TO;
    }

    /**
     * @brief 判断值是否是顶点.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsVertex(unsigned val)
    {
        return val < PATH_CMD_END_POLY && val >= PATH_CMD_MOVE_TO;
    }

    /**
     * @brief 判断当前状态是否MOVE_TO.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsMoveTo(unsigned val)
    {
        return PATH_CMD_MOVE_TO == val;
    }

    /**
     * @brief 判断当前状态是否Stop状态.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsStop(unsigned val)
    {
        return PATH_CMD_STOP == val;
    }

    /**
     * @brief 判断当前状态是否LINE_TO状态.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsLineTo(unsigned val)
    {
        return PATH_CMD_LINE_TO == val;
    }

    /**
     * @brief 判断当前状态是否是绘制贝塞尔曲线CURVE状态.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsCurve(unsigned val)
    {
        return PATH_CMD_CURVE4 == val || PATH_CMD_CURVE3 == val;
    }

    /**
     * @brief 判断当前状态是否是绘制二次贝塞尔曲线CURVE状态.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsCurve3(unsigned val) //TODO 函数名修改
    {
        return PATH_CMD_CURVE3 == val;
    }

    /**
     * @brief 判断当前状态是否是绘制三次贝塞尔曲线CURVE状态.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsCurve4(unsigned val) //TODO 函数名修改
    {
        return PATH_CMD_CURVE4 == val;
    }

    /**
     * @brief 判断当前状态是否是绘制曲线结尾状态.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsEndPoly(unsigned val)
    {
        return PATH_CMD_END_POLY == (val & PATH_CMD_MASK);
    }

    /**
     * @brief 判断当前状态是否是绘制曲线闭合状态.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsClose(unsigned val)
    {
        return (val & ~(PATH_FLAGS_CW | PATH_FLAGS_CCW)) == (PATH_CMD_END_POLY | PATH_FLAGS_CLOSE);
    }

    /**
     * @brief 判断当前状态是否是绘制曲线下一个多边形状态.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsNextPoly(unsigned val)
    {
        return IsStop(val) || IsMoveTo(val) || IsEndPoly(val);
    }

    /**
     * @brief 判断当前状态是否是逆时针.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsCw(unsigned val) //TODO 函数名修改
    {
        return 0 != (val & PATH_FLAGS_CW);
    }

    /**
     * @brief 判断当前状态是否是顺时针.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsCcw(unsigned val)
    {
        return (val & PATH_FLAGS_CCW) != 0;
    }

    /**
     * @brief 判断当前方向
     * @since 1.0
     * @version 1.0
     */
    inline bool IsOriented(unsigned val)
    {
        return (val & (PATH_FLAGS_CW | PATH_FLAGS_CCW)) != 0;
    }

    /**
     * @brief 判断当前是否封闭
     * @since 1.0
     * @version 1.0
     */
    inline bool IsClosed(unsigned val)
    {
        return (val & PATH_FLAGS_CLOSE) != 0;
    }

    /**
     * @brief 清除方向标记
     * @since 1.0
     * @version 1.0
     */
    inline unsigned ClearOrientation(unsigned val)
    {
        return val & ~(PATH_FLAGS_CW | PATH_FLAGS_CCW);
    }

    /**
     * @brief 获取封闭状态
     * @since 1.0
     * @version 1.0
     */
    inline unsigned GetCloseFlag(unsigned val)
    {
        return val & PATH_FLAGS_CLOSE;
    }

    /**
     * @brief 设置朝向
     * @since 1.0
     * @version 1.0
     */
    inline unsigned SetOrientation(unsigned cleanVal, unsigned addVal)
    {
        return ClearOrientation(cleanVal) | addVal;
    }

    /**
     * @brief 获取朝向
     * @since 1.0
     * @version 1.0
     */
    inline unsigned GetOrientation(unsigned val)
    {
        return val & (PATH_FLAGS_CW | PATH_FLAGS_CCW);
    }

#ifdef GRAPHIC_GEOMETRY_CUSTOM_ALLOCATOR
#    include "graphic_geometry_allocator.h"
#else
    template <class T>
    struct ObjAllocator {
        /**
         * @brief 对象内存分配
         * @since 1.0
         * @version 1.0
         */
        static T* Allocate()
        {
            return new T;
        }
        /**
         * @brief 对象内存释放
         * @since 1.0
         * @version 1.0
         */
        static void Deallocate(T* ptr)
        {
            delete ptr;
        }
    };

    template <class T>
    struct ArrAllocator {
        /**
         * @brief 数组内存分配
         * @since 1.0
         * @version 1.0
         */
        static T* Allocate(unsigned num)
        {
            return new T[num];
        }
        /**
         * @brief 数组内存释放
         * @since 1.0
         * @version 1.0
         */
        static void Deallocate(T* ptr, unsigned)
        {
            delete[] ptr;
        }
    };

    using CoverType = unsigned char;

    /**
    * @brief 定义矩形类.
    * @since 1.0
    * @version 1.0
    */
    template <class T>
    struct RectBase {
        using ValueType = T;
        using SelfType = RectBase<T>;
        T x1;
        T y1;
        T x2;
        T y2;

        RectBase(T x1_, T y1_, T x2_, T y2_) :
            x1(x1_), y1(y1_), x2(x2_), y2(y2_)
        {}
        RectBase()
        {}
        /**
        * @brief 规范化矩形.
        * @since 1.0
        * @version 1.0
        */
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

        /**
        * @brief 坐标裁剪到指定范围内.
        * @since 1.0
        * @version 1.0
        */
        bool Clip(const SelfType& rect)
        {
            if (x2 > rect.x2) {
                x2 = rect.x2;
            }
            if (y2 > rect.y2) {
                y2 = rect.y2;
            }
            if (x1 < rect.x1) {
                x1 = rect.x1;
            }
            if (y1 < rect.y1) {
                y1 = rect.y1;
            }
            return y1 <= y2 && x1 <= x2;
        }

        /**
        * @brief 坐标是否有效.
        * @since 1.0
        * @version 1.0
        */
        bool IsValid() const
        {
            return y1 <= y2 && x1 <= x2;
        }

        /**
        * @brief 坐标的有效范围.
        * @since 1.0
        * @version 1.0
        */
        bool Overlaps(const SelfType& rect) const
        {
            return !(rect.y1 > y2 || rect.y2 < y1 || rect.x1 > x2 || rect.x2 < x1);
        }

        /**
        * @brief 判断坐标在指定范围内.
        * @since 1.0
        * @version 1.0
        */
        bool HitTest(T x, T y) const
        {
            return (y >= y1 && y <= y2 && x >= x1 && x <= x2);
        }
    };

    /**
    * @brief 获取两矩形是否相交区域.
    *
    * @param rect1,rect2 两个矩形.
    * @return Returns 相交的矩形.
    * @since 1.0
    * @version 1.0
    */
    template <class Rect>
    inline Rect IntersectRectangles(const Rect& rect1, const Rect& rect2)
    {
        Rect rect = rect1;
        if (rect.x2 > rect2.x2) {
            rect.x2 = rect2.x2;
        }
        if (rect.y2 > rect2.y2) {
            rect.y2 = rect2.y2;
        }
        if (rect.x1 < rect2.x1) {
            rect.x1 = rect2.x1;
        }
        if (rect.y1 < rect2.y1) {
            rect.y1 = rect2.y1;
        }
        return rect;
    }

    /**
    * @brief 获取两矩形的并集区域.
    *
    * @param rect1,rect2 两个矩形.
    * @return Returns 并集区域的矩形.
    * @since 1.0
    * @version 1.0
    */
    template <class Rect>
    inline Rect UniteRectangles(const Rect& rect1, const Rect& rect2)
    {
        Rect rect = rect1;
        if (rect.x2 < rect2.x2) {
            rect.x2 = rect2.x2;
        }
        if (rect.y2 < rect2.y2) {
            rect.y2 = rect2.y2;
        }
        if (rect.x1 > rect2.x1) {
            rect.x1 = rect2.x1;
        }
        if (rect.y1 > rect2.y1) {
            rect.y1 = rect2.y1;
        }
        return rect;
    }
    using RectI = RectBase<int>;
    //using RectF = RectBase<float>;
    using RectD = RectBase<double>;

    template <class T>
    struct PointBase {
        using ValueType = T;
        T x;
        T y;
        PointBase()
        {}
        PointBase(T x_, T y_) :
            x(x_), y(y_)
        {}
    };
    //using PointF = PointBase<float>;
    using PointD = PointBase<double>;
    using PointI = PointBase<int>;
    template <class T>
    struct VertexBase {
        using ValueType = T;
        T x;
        T y;
        unsigned cmd;
        VertexBase()
        {}
        VertexBase(T x_, T y_, unsigned cmd_) :
            x(x_), y(y_), cmd(cmd_)
        {}
    };

    using VertexF = VertexBase<float>;
    using VertexD = VertexBase<double>;
    using VertexI = VertexBase<int>;
    template <class T>
    struct ConstRowInfo {
        int x1;
        int x2;
        const T* ptr;
        ConstRowInfo()
        {}
        ConstRowInfo(int x1_, int x2_, const T* ptr_) :
            x1(x1_), x2(x2_), ptr(ptr_)
        {}
    };

    template <class T>
    struct RowInfo {
        int x1;
        int x2;
        T* ptr;
        RowInfo()
        {}
        RowInfo(int x1_, int x2_, T* ptr_) :
            x1(x1_), x2(x2_), ptr(ptr_)
        {}
    };

} // namespace OHOS
#endif

#endif
