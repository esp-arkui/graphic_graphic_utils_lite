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

    /**
     * @brief 图源映射变换
     * @since 1.0
     * @version 1.0
     */
    struct TransAffine {
        double scaleX_, shearY, shearX, scaleY_, translateX, translateY;
        /**
         * @brief 初始化为单位矩阵
         * @since 1.0
         * @version 1.0
         */
        TransAffine() :
            scaleX_(1.0), shearY(0.0), shearX(0.0), scaleY_(1.0), translateX(0.0), translateY(0.0)
        {}
        /**
         * @brief 自定义矩阵
         * @since 1.0
         * @version 1.0
         */
        TransAffine(double v0, double v1, double v2, double v3, double v4, double v5) :
            scaleX_(v0), shearY(v1), shearX(v2), scaleY_(v3), translateX(v4), translateY(v5)
        {}
        /**
         * @brief 用于将矩形转换为平行四边形
         * @since 1.0
         * @version 1.0
         */
        TransAffine(double x1, double y1, double x2, double y2, const double* parl)
        {
            RectToParl(x1, y1, x2, y2, parl);
        }

        /**
         * @brief 将原平行四边形转换为目标平行四边形
         * @param src 原平行四边形
         * @param dst 目标平行四边形
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& ParlToParl(const double* src, const double* dst);
        /**
         * @brief 将原矩形转换为目标平行四边形
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
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Reset();

        /**
         * @brief 图元偏移
         * @param deltaX x轴偏移量
         * @param deltaY y轴偏移量
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Translate(double x, double y);
        /**
         * @brief 图元旋转
         * @param angle 旋转角度
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Rotate(double angle);
        /**
         * @brief 图元放大
         * @param scale 整体放大系数
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Scale(double scale);
        /**
         * @brief 图元放大
         * @param x x轴放大系数
         * @param y y轴放大系数
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Scale(double x, double y);

        /**
         * @brief 转换矩阵相乘
         * @param metrix 矩阵
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Multiply(const TransAffine& metrix);

        /**
         * @brief 矩阵翻转
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Invert();

        const TransAffine& operator*=(const TransAffine& metrix)
        {
            return Multiply(metrix);
        }

        /**
         * @brief 对x,y进行转换
         * @param x x轴坐标
         * @param y y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void Transform(double* x, double* y) const;

        /**
         * @brief 逆变换
         * @param x x轴坐标
         * @param y y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void InverseTransform(double* x, double* y) const;

        /**
         * @brief 计算行列式的倒数
         * @since 1.0
         * @version 1.0
         */
        double DeterminantReciprocal() const
        {
            return 1.0 / (scaleX_ * scaleY_ - shearY * shearX);
        }

        /**
         * @brief 检查矩阵是否退化
         * @since 1.0
         * @version 1.0
         */
        bool IsValid(double epsilon = affineEpsilon) const;

        /**
         * @brief 检查它是否是单位矩阵
         * @since 1.0
         * @version 1.0
         */
        bool IsIdentity(double epsilon = affineEpsilon) const;

        /**
         * @brief 缩放
         * @param x x轴坐标
         * @param y y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void ScalingAbs(double* x, double* y) const;
    };

    inline void TransAffine::Transform(double* x, double* y) const
    {
        double tmp = *x;
        *x = tmp * scaleX_ + *y * shearX + translateX;
        *y = tmp * shearY + *y * scaleY_ + translateY;
    }

    inline void TransAffine::InverseTransform(double* x, double* y) const
    {
        double reciprocal = DeterminantReciprocal();
        double a = (*x - translateX) * reciprocal;
        double b = (*y - translateY) * reciprocal;
        *x = a * scaleY_ - b * shearX;
        *y = b * scaleX_ - a * shearY;
    }

    inline const TransAffine& TransAffine::Translate(double deltaX, double deltaY)
    {
        translateX += deltaX;
        translateY += deltaY;
        return *this;
    }

    inline const TransAffine& TransAffine::Rotate(double angle)
    {
        double scaleXTemp = scaleX_ * std::cos(angle) - shearY * std::sin(angle);
        double shearXTemp = shearX * std::cos(angle) - scaleY_ * std::sin(angle);
        double translateXTemp = translateX * std::cos(angle) - translateY * std::sin(angle);
        shearY = scaleX_ * std::sin(angle) + shearY * std::cos(angle);
        scaleY_ = shearX * std::sin(angle) + scaleY_ * std::cos(angle);
        translateY = translateX * std::sin(angle) + translateY * std::cos(angle);
        scaleX_ = scaleXTemp;
        shearX = shearXTemp;
        translateX = translateXTemp;
        return *this;
    }

    inline const TransAffine& TransAffine::Scale(double scaleX, double scaleY)
    {
        double mm0 = scaleX;
        double mm3 = scaleY;
        scaleX_ *= mm0;
        shearX *= mm0;
        translateX *= mm0;
        shearY *= mm3;
        scaleY_ *= mm3;
        translateY *= mm3;
        return *this;
    }

    inline const TransAffine& TransAffine::Scale(double scale)
    {
        double m = scale;
        scaleX_ *= m;
        shearX *= m;
        translateX *= m;
        shearY *= m;
        scaleY_ *= m;
        translateY *= m;
        return *this;
    }

    inline void TransAffine::ScalingAbs(double* x, double* y) const
    {
        *x = std::sqrt(scaleX_ * scaleX_ + shearX * shearX);
        *y = std::sqrt(shearY * shearY + scaleY_ * scaleY_);
    }

    /**
     * @brief 旋转矩阵
     * @since 1.0
     * @version 1.0
     */
    class TransAffineRotation : public TransAffine {
    public:
        TransAffineRotation(double angle) :
            TransAffine(std::cos(angle), std::sin(angle), -std::sin(angle), std::cos(angle), 0.0, 0.0)
        {}
    };

    /**
     * @brief 缩放矩阵
     * @since 1.0
     * @version 1.0
     */
    class TransAffineScaling : public TransAffine {
    public:
        TransAffineScaling(double x, double y) :
            TransAffine(x, 0.0, 0.0, y, 0.0, 0.0)
        {}

        TransAffineScaling(double scale) :
            TransAffine(scale, 0.0, 0.0, scale, 0.0, 0.0)
        {}
    };
    /**
     * @brief 位移矩阵
     * @since 1.0
     * @version 1.0
     */
    class TransAffineTranslation : public TransAffine {
    public:
        TransAffineTranslation(double x, double y) :
            TransAffine(1.0, 0.0, 0.0, 1.0, x, y)
        {}
    };
} // namespace OHOS

#endif
