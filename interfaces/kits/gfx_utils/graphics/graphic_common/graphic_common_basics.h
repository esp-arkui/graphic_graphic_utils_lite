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

namespace OHOS {
#define GRAPHIC_GEOMETRY_INLINE inline

    /**
     * @brief 填充规则.
     * @since 1.0
     * @version 1.0
     */
    enum FillingRuleEnum {
        FILL_NON_ZERO,
        FILL_EVEN_ODD
    };

    /**
     * @brief 子像素的偏移以及掩码标志.
     * @since 1.0
     * @version 1.0
     */
    enum PolySubpixelScaleEnum {
        POLY_SUBPIXEL_SHIFT = 8,
        POLY_SUBPIXEL_SCALE = 1 << POLY_SUBPIXEL_SHIFT,
        POLY_SUBPIXEL_MASK = POLY_SUBPIXEL_SCALE - 1
    };

    enum GradientSubpixelScaleEnum {
        GRADIENT_SUBPIXEL_SHIFT = 4,
        GRADIENT_SUBPIXEL_SCALE = 1 << GRADIENT_SUBPIXEL_SHIFT,
        GRADIENT_SUBPIXEL_MASK = GRADIENT_SUBPIXEL_SCALE - 1
    };
    /**
     * @brief 覆盖率的弹性处理.
     * @since 1.0
     * @version 1.0
     */
    enum CoverScaleEnum {
        COVER_SHIFT = 8,
        COVER_SIZE = 1 << COVER_SHIFT,
        COVER_MASK = COVER_SIZE - 1,
        COVER_NONE = 0,
        COVER_FULL = COVER_MASK
    };

    enum PathCommandsEnum {
        PATH_CMD_STOP = 0,
        PATH_CMD_MOVE_TO = 1,
        PATH_CMD_LINE_TO = 2,
        PATH_CMD_CURVE3 = 3,      // 二次曲线命令
        PATH_CMD_CURVE4 = 4,      // 三次曲线命令
        PATH_CMD_CURVEN = 5,      // 曲线命令
        PATH_CMD_CARROM = 6,      // 二次曲线命令
        PATH_CMD_UBSPLINE = 7,    // 曲线命令
        PATH_CMD_END_POLY = 0x0F, // 多边形闭合
        PATH_CMD_MASK = 0x0F
    };

    enum PathFlagsEnum {
        PATH_FLAGS_NONE = 0,
        PATH_FLAGS_CLOCKWISE = 0x10,      // 顺时针
        PATH_FLAGS_ANTI_CLOCKWISE = 0x20, // 逆时针
        PATH_FLAGS_CLOSE = 0x40,
        PATH_FLAGS_MASK = 0xF0
    };

    /**
     * @brief 共线的四种情况.
     * @since 1.0
     * @version 1.0
     */
    enum RecursiveBezierEnum {
        COLLINEAR = 0,
        COLLINEAR1,
        COLLINEAR2,
        COLLINEAR3
    };
    enum ClippingFlagsEnum {
        CLIPPING_FLAGS_X1_CLIPPED = 4,
        CLIPPING_FLAGS_X2_CLIPPED = 1,
        CLIPPING_FLAGS_Y1_CLIPPED = 8,
        CLIPPING_FLAGS_Y2_CLIPPED = 2,
        CLIPPING_FLAGS_X_CLIPPED = CLIPPING_FLAGS_X1_CLIPPED | CLIPPING_FLAGS_X2_CLIPPED,
        CLIPPING_FLAGS_Y_CLIPPED = CLIPPING_FLAGS_Y1_CLIPPED | CLIPPING_FLAGS_Y2_CLIPPED
    };
    template <class Type>
    struct RectBase;

    template <class Type>
    struct PointBase;

    template <class Type>
    struct VertexBase;
#define GRAPHIC_GEOMETYR_ENABLE_LINECAP_STYLES_VERTEX_SOURCE 1
#define GRAPHIC_GEOMETYR_ENABLE_LINEJOIN_STYLES_VERTEX_SOURCE 1
#define GRAPHIC_GEOMETYR_ENABLE_ELLIPSE_CIRCLE_VERTEX_SOURCE 1
#define GRAPHIC_GEOMETYR_ENABLE_BEZIER_ARC_VERTEX_SOURCE 1
#define GRAPHIC_GEOMETYR_ENABLE_ARC_VERTEX_SOURCE 1
#define GRAPHIC_GEOMETYR_ENABLE_ROUNDEDRECT_VERTEX_SOURCE 1
#define GRAPHIC_GEOMETYR_ENABLE_DASH_GENERATE_VERTEX_SOURCE 1
#define GRAPHIC_GEOMETYR_ENABLE_BLUR_EFFECT_VERTEX_SOURCE 0
#define GRAPHIC_GEOMETYR_ENABLE_SHADOW_EFFECT_VERTEX_SOURCE 0

    using int8 = signed char;
    using int8u = unsigned char;
    using int16 = short;
    using int16u = unsigned short;
    using int32 = int;
    using int32u = unsigned;
    using int64 = signed long long;
    using int64u = unsigned long long;
    using RectI = RectBase<int>;
    using RectD = RectBase<double>;
    using PointD = PointBase<double>;
    using PointI = PointBase<int>;
    using VertexF = VertexBase<float>;
    using VertexD = VertexBase<double>;
    using VertexI = VertexBase<int>;

    const double COEFFICIENT = 0.7;
    const double ALPHAHALF = 0.5;
    const double VERTEX_DIST_EPSILON = 1e-14;
    const double INTERSECTIONEPSILON = 1.0e-30;
    /**
     * @brief 圆周率
     */
    const double PI = 3.14159265358979323846;
    /**
     * @brief 半圆弧角度
     */
    const double RAD_BASE = 180.0;
    const double ROUND_BASE = 0.5;

    /**
     * @brief 平角的度数
     */
    const double BOXER = 180.0;

    /**
     * @brief BUF_SIZE
     */
    const double BUF_SIZE = 1024.0;
    /**
     * @brief 颜色的最大值
     */
    const unsigned MAX_COLOR_NUM = 255;
    /**
     * @brief 颜色的最大值数组长度
     */
    const unsigned MAX_COLOR_SIZE = 256;

    /**
     * @brief 颜色的最大值的一半
     */
    const unsigned HALF_COLOR_NUM = 128;
    /**
     * @brief 颜色的最大值的八分之一
     */
    const unsigned EIGHTH_COLOR_NUM = 32;
    /**
     * @brief 颜色的最大值的三十二分之一
     */
    const unsigned THIRTY_TWO_COLOR_NUM = 8;

    /**
     * @brief 默认最大斜接
     */
    const unsigned DEFAULTMITERLIMIT = 10;
    /**
     * @brief 一个字节的长度
     */
    const int BYTE_LENGTH = 8;
    const int ARRAY_SIZE = 16;
    /**
     * @brief dash数组最大长度
     */
    const int MAX_DASHES = 32;
    /**
     * @brief 默认的块偏移量
     */
    const int BLOCK_SHIFT_SIZE = 6;
    /**
     * @brief 绘制椭圆所需的顶点数
     */
    const int ELLIPSE_VERTEX_NUM = 4;
    /**
     * 一般步幅是宽度的四倍
     */
    const int STRIDE_FACTOR = 4;
    const int PXSIZE2STRIDE_FACTOR = 3;
    /**
     * @brief 1/√2 即1 除以 √2
     */
    const double SIN45 = 0.7071068;
    /**
     * @brief 1/√2 即1 除以 √2
     */
    const double COS45 = 0.7071068;
    /**
     * @brief 两倍
     */
    const int TWO_TIMES = 2;
    /**
     * @brief 三倍
     */
    const int THREE_TIMES = 3;
    /**
     * @brief 四倍
     */
    const int FOUR_TIMES = 4;
    /**
     * @brief 六倍
     */
    const int SIX_TIMES = 6;
    /**
     * @brief 一百倍
     */
    const int HUNDRED_TIMES = 100;
    /**
     * @brief 一次走两步
     */
    const int TWO_STEP = 2;
    const double HALFNUM = 0.5;
    const double DOUBLENUM = 2.0;
    const double RADDALETAELPS = 0.125;

    const int CONSTITUTION = 16384;
    /**
     * @brief 正下方
     */
    const int DIRECTLY_BELOW = 10;
    /**
     * 下标
     */
    const int INDEX_ZERO = 0;
    const int INDEX_ONE = 1;
    const int INDEX_TWO = 2;
    const int INDEX_THREE = 3;
    const int INDEX_FOUR = 4;
    const int INDEX_FIVE = 5;
    const int INDEX_SIX = 6;
    const int INDEX_SEVEN = 7;

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
        return int((val < 0.0) ? val - ROUND_BASE : val + ROUND_BASE);
    }
    GRAPHIC_GEOMETRY_INLINE int Uround(double val)
    {
        return unsigned(val + ROUND_BASE);
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

    /**
     * @brief 两个数是否相近.
     *
     * @param val1,val2 两个数,epsilon 误差.
     * @return Returns 两个数是否相近.
     * @since 1.0
     * @version 1.0
     */
    template <class T>
    bool IsEqualEps(T val1, T val2, T epsilon)
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
    inline double RadianToDegree(double val)
    {
        return val * RAD_BASE / PI;
    }
    /**
     * @brief 度转弧度.
     * @since 1.0
     * @version 1.0
     */
    inline double DegreeToRadian(double val)
    {
        return val * PI / RAD_BASE;
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
    inline bool IsQuadraticBezierCurve(unsigned val)
    {
        return PATH_CMD_CURVE3 == val;
    }

    /**
     * @brief 判断当前状态是否是绘制三次贝塞尔曲线CURVE状态.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsCubicBezierCurve(unsigned val)
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
        return (val & ~(PATH_FLAGS_ANTI_CLOCKWISE | PATH_FLAGS_CLOCKWISE)) == (PATH_CMD_END_POLY | PATH_FLAGS_CLOSE);
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
    inline bool IsAntiClockWise(unsigned val)
    {
        return (val & PATH_FLAGS_ANTI_CLOCKWISE) != 0;
    }

    /**
     * @brief 判断当前状态是否是顺时针.
     * @since 1.0
     * @version 1.0
     */
    inline bool IsClockWise(unsigned val)
    {
        return (val & PATH_FLAGS_CLOCKWISE) != 0;
    }

    /**
     * @brief 判断当前方向
     * @since 1.0
     * @version 1.0
     */
    inline bool IsOriented(unsigned val)
    {
        return (val & (PATH_FLAGS_ANTI_CLOCKWISE | PATH_FLAGS_CLOCKWISE)) != 0;
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
        return val & ~(PATH_FLAGS_ANTI_CLOCKWISE | PATH_FLAGS_CLOCKWISE);
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
        return val & (PATH_FLAGS_ANTI_CLOCKWISE | PATH_FLAGS_CLOCKWISE);
    }

#ifdef GRAPHIC_GEOMETRY_CUSTOM_ALLOCATOR
#    include "graphic_geometry_allocator.h"
#else
    template <class T>
    struct GeometryObjectAllocator {
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
    struct GeometryArrayAllocator {
        /**
         * @brief 数组内存分配
         * @since 1.0
         * @version 1.0
         */
        static T* Allocate(unsigned num)
        {
            if (num < 1) {
                num = 1;
            }
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
    Rect IntersectRectangles(const Rect& rect1, const Rect& rect2)
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
    Rect UniteRectangles(const Rect& rect1, const Rect& rect2)
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
