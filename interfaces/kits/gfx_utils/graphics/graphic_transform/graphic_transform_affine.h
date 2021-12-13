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
#ifndef GRAPHIC_TRANS_AFFINE_INCLUDED
#define GRAPHIC_TRANS_AFFINE_INCLUDED

#include <cmath>

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"

namespace OHOS {
    const double affineEpsilon = 1e-14;

    struct TransAffine {
        double scaleX, shearY, shearX, scaleY, translateX, translateY;

        TransAffine() :
            scaleX(1.0), shearY(0.0), shearX(0.0), scaleY(1.0), translateX(0.0), translateY(0.0)
        {}

        TransAffine(double v0, double v1, double v2,
                    double v3, double v4, double v5) :
            scaleX(v0),
            shearY(v1), shearX(v2), scaleY(v3), translateX(v4), translateY(v5)
        {}

        TransAffine(double x1, double y1, double x2, double y2,
                    const double* parl)
        {
            RectToParl(x1, y1, x2, y2, parl);
        }

        /**
         * @brief 将原平行四边形转换为目标平行四边形
         * 
         * @param src 原平行四边形
         * @param dst 目标平行四边形
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& ParlToParl(const double* src,
                                      const double* dst);
        /**
         * @brief 将原矩形转换为目标平行四边形
         * 
         * @param x1 矩形左上角x轴坐标
         * @param y1 矩形左上角y轴坐标
         * @param x2 矩形右下角x轴坐标
         * @param y2 矩形右下角y轴坐标
         * @param parl 目标平行四边形
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& RectToParl(double x1, double y1,
                                      double x2, double y2,
                                      const double* parl);

        /**
         * @brief 重置矩阵
         * 
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Reset();

        /**
         * @brief 图元偏移
         * 
         * @param x x轴偏移量
         * @param y y轴偏移量
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Translate(double x, double y);
        /**
         * @brief 图元旋转
         * 
         * @param a 旋转角度
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Rotate(double a);
        /**
         * @brief 图元放大
         * 
         * @param s 整体放大系数
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Scale(double s);
        /**
         * @brief 图元放大
         * 
         * @param x x轴放大系数
         * @param y y轴放大系数
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Scale(double x, double y);

        /**
         * @brief 转换矩阵相乘
         * 
         * @param m 矩阵
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Multiply(const TransAffine& m);

        /**
         * @brief 矩阵翻转
         * 
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Invert();

        const TransAffine& operator*=(const TransAffine& m)
        {
            return Multiply(m);
        }

        /**
         * @brief 对x,y进行转换
         * 
         * @param x x轴坐标
         * @param y y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void Transform(double* x, double* y) const;

        /**
         * @brief 逆变换
         * 
         * @param x x轴坐标
         * @param y y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void InverseTransform(double* x, double* y) const;

        /**
         * @brief 计算行列式的倒数
         * 
         * @since 1.0
         * @version 1.0
         */
        double DeterminantReciprocal() const
        {
            return 1.0 / (scaleX * scaleY - shearY * shearX);
        }

        /**
         * @brief 检查矩阵是否退化
         *
         * @since 1.0
         * @version 1.0
         */
        bool IsValid(double epsilon = affineEpsilon) const;

        /**
         * @brief 检查它是否是单位矩阵
         * 
         * @since 1.0
         * @version 1.0
         */
        bool IsIdentity(double epsilon = affineEpsilon) const;
        /**
         * @brief 缩放
         * 
         * @param x x轴坐标
         * @param y y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void ScalingAbs(double* x, double* y) const;
    };

    //------------------------------------------------------------------------
    inline void TransAffine::Transform(double* x, double* y) const
    {
        double tmp = *x;
        *x = tmp * scaleX + *y * shearX + translateX;
        *y = tmp * shearY + *y * scaleY + translateY;
    }

    //------------------------------------------------------------------------
    inline void TransAffine::InverseTransform(double* x, double* y) const
    {
        double d = DeterminantReciprocal();
        double a = (*x - translateX) * d;
        double b = (*y - translateY) * d;
        *x = a * scaleY - b * shearX;
        *y = b * scaleX - a * shearY;
    }

    //------------------------------------------------------------------------
    inline const TransAffine& TransAffine::Translate(double x, double y)
    {
        translateX += x;
        translateY += y;
        return *this;
    }

    //------------------------------------------------------------------------
    inline const TransAffine& TransAffine::Rotate(double a)
    {
        double ca = std::cos(a);
        double sa = std::sin(a);
        double t0 = scaleX * ca - shearY * sa;
        double t2 = shearX * ca - scaleY * sa;
        double t4 = translateX * ca - translateY * sa;
        shearY = scaleX * sa + shearY * ca;
        scaleY = shearX * sa + scaleY * ca;
        translateY = translateX * sa + translateY * ca;
        scaleX = t0;
        shearX = t2;
        translateX = t4;
        return *this;
    }

    //------------------------------------------------------------------------
    inline const TransAffine& TransAffine::Scale(double x, double y)
    {
        double mm0 = x; // Possible hint for the optimizer
        double mm3 = y;
        scaleX *= mm0;
        shearX *= mm0;
        translateX *= mm0;
        shearY *= mm3;
        scaleY *= mm3;
        translateY *= mm3;
        return *this;
    }

    //------------------------------------------------------------------------
    inline const TransAffine& TransAffine::Scale(double s)
    {
        double m = s; // Possible hint for the optimizer
        scaleX *= m;
        shearX *= m;
        translateX *= m;
        shearY *= m;
        scaleY *= m;
        translateY *= m;
        return *this;
    }

    inline void TransAffine::ScalingAbs(double* x, double* y) const
    {
        *x = std::sqrt(scaleX * scaleX + shearX * shearX);
        *y = std::sqrt(shearY * shearY + scaleY * scaleY);
    }

    class TransAffineRotation : public TransAffine {
    public:
        TransAffineRotation(double a) :
            TransAffine(std::cos(a), std::sin(a), -std::sin(a), std::cos(a), 0.0, 0.0)
        {}
    };

    class TransAffineScaling : public TransAffine {
    public:
        TransAffineScaling(double x, double y) :
            TransAffine(x, 0.0, 0.0, y, 0.0, 0.0)
        {}

        TransAffineScaling(double s) :
            TransAffine(s, 0.0, 0.0, s, 0.0, 0.0)
        {}
    };

    class TransAffineTranslation : public TransAffine {
    public:
        TransAffineTranslation(double x, double y) :
            TransAffine(1.0, 0.0, 0.0, 1.0, x, y)
        {}
    };

} // namespace OHOS

#endif
