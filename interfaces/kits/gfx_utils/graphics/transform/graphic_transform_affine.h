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

#include "gfx_utils/graphics/common/graphic_common_basics.h"
#include "gfx_utils/graphic_math.h"
namespace OHOS {
    const float affineEpsilon = 1e-14;

    /**
     * @brief 图源映射变换
     * @since 1.0
     * @version 1.0
     */
    class TransAffine : public Matrix3<float> {
    public:
        /**
         * @brief 初始化为单位矩阵
         * @since 1.0
         * @version 1.0
         */
        TransAffine()
            :Matrix3<float>()
        {}
        /**
         * @brief 自定义矩阵
         * @since 1.0
         * @version 1.0
         */
        TransAffine(float v0, float v1, float v2, float v3, float v4, float v5) :
            Matrix3<float>(v0, v2, v4, v1, v3, v5, 0, 0, 1)
        {}
        /**
         * @brief 用于将矩形转换为平行四边形
         * @since 1.0
         * @version 1.0
         */
        TransAffine(float x1, float y1, float x2, float y2, const float* parl)
        {
            RectToParl(x1, y1, x2, y2, parl);
        }
        void SetData(int iIndex, float fValue)
        {
            data_[iIndex] = fValue;
        }
        /**
         * @brief 将原平行四边形转换为目标平行四边形
         * @param src 原平行四边形
         * @param dst 目标平行四边形
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& ParlToParl(const float* src, const float* dst);
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
        const TransAffine& RectToParl(float x1, float y1,
                                      float x2, float y2,
                                      const float* parl);

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
        const TransAffine& Translate(float x, float y);
        /**
         * @brief 图元旋转
         * @param angle 旋转角度
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Rotate(float angle);
        /**
         * @brief 图元放大
         * @param scale 整体放大系数
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Scale(float scale);
        /**
         * @brief 图元放大
         * @param x x轴放大系数
         * @param y y轴放大系数
         * @since 1.0
         * @version 1.0
         */
        const TransAffine& Scale(float x, float y);

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
        void Transform(float* x, float* y) const;

        /**
         * @brief 逆变换
         * @param x x轴坐标
         * @param y y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void InverseTransform(float* x, float* y) const;

        /**
         * @brief 计算行列式的倒数
         * @since 1.0
         * @version 1.0
         */
        float DeterminantReciprocal() const
        {
            return 1.0 / (data_[0] * data_[4] - data_[3] * data_[1]);
        }

        /**
         * @brief 检查矩阵是否退化
         * @since 1.0
         * @version 1.0
         */
        bool IsValid(float epsilon = affineEpsilon) const;

        /**
         * @brief 检查它是否是单位矩阵
         * @since 1.0
         * @version 1.0
         */
        bool IsIdentity(float epsilon = affineEpsilon) const;

        /**
         * @brief 缩放
         * @param x x轴坐标
         * @param y y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void ScalingAbs(float* x, float* y) const;
    };

    inline void TransAffine::Transform(float* x, float* y) const
    {
        float tmp = *x;
        *x = tmp * data_[INDEX_ZERO] + *y * data_[INDEX_ONE] + data_[INDEX_TWO];
        *y = tmp * data_[INDEX_THREE] + *y * data_[INDEX_FOUR] + data_[INDEX_FIVE];
    }

    inline void TransAffine::InverseTransform(float* x, float* y) const
    {
        float reciprocal = DeterminantReciprocal();
        float a = (*x - data_[INDEX_TWO]) * reciprocal;
        float b = (*y - data_[INDEX_FIVE]) * reciprocal;
        *x = a * data_[INDEX_FOUR] - b * data_[INDEX_ONE];
        *y = b * data_[INDEX_ZERO] - a * data_[INDEX_THREE];
    }

    inline const TransAffine& TransAffine::Translate(float deltaX, float deltaY)
    {
        data_[INDEX_TWO] += deltaX;
        data_[INDEX_FIVE] += deltaY;
        return *this;
    }

    inline const TransAffine& TransAffine::Rotate(float angle)
    {
        float scaleXTemp = data_[INDEX_ZERO] * std::cos(angle) - data_[INDEX_THREE] * std::sin(angle);
        float shearXTemp = data_[INDEX_ONE] * std::cos(angle) - data_[INDEX_FOUR] * std::sin(angle);
        float translateXTemp = data_[INDEX_TWO] * std::cos(angle) - data_[INDEX_FIVE] * std::sin(angle);
        data_[INDEX_THREE] = data_[INDEX_ZERO] * std::sin(angle) + data_[INDEX_THREE] * std::cos(angle);
        data_[INDEX_FOUR] = data_[INDEX_ONE] * std::sin(angle) + data_[INDEX_FOUR] * std::cos(angle);
        data_[INDEX_FIVE] = data_[INDEX_TWO] * std::sin(angle) + data_[INDEX_FIVE] * std::cos(angle);
        data_[INDEX_ZERO] = scaleXTemp;
        data_[INDEX_ONE] = shearXTemp;
        data_[INDEX_TWO] = translateXTemp;
        return *this;
    }

    inline const TransAffine& TransAffine::Scale(float scaleX, float scaleY)
    {
        data_[INDEX_ZERO] *= scaleX;
        data_[INDEX_ONE] *= scaleX;
        data_[INDEX_TWO] *= scaleX;
        data_[INDEX_THREE] *= scaleY;
        data_[INDEX_FOUR] *= scaleY;
        data_[INDEX_FIVE] *= scaleY;
        return *this;
    }

    inline const TransAffine& TransAffine::Scale(float scale)
    {
        data_[INDEX_ZERO] *= scale;
        data_[INDEX_ONE] *= scale;
        data_[INDEX_TWO] *= scale;
        data_[INDEX_THREE] *= scale;
        data_[INDEX_FOUR] *= scale;
        data_[INDEX_FIVE] *= scale;
        return *this;
    }

    inline void TransAffine::ScalingAbs(float* x, float* y) const
    {
        *x = std::sqrt(data_[INDEX_ZERO] * data_[INDEX_ZERO] + data_[INDEX_ONE] * data_[INDEX_ONE]);
        *y = std::sqrt(data_[INDEX_THREE] * data_[INDEX_THREE] + data_[INDEX_FOUR] * data_[INDEX_FOUR]);
    }

    /**
     * @brief 旋转矩阵
     * @since 1.0
     * @version 1.0
     */
    class TransAffineRotation : public TransAffine {
    public:
        TransAffineRotation(float angle)
            : TransAffine(std::cos(angle), std::sin(angle), -std::sin(angle), std::cos(angle), 0.0, 0.0)
        {}
    };

    /**
     * @brief 缩放矩阵
     * @since 1.0
     * @version 1.0
     */
    class TransAffineScaling : public TransAffine {
    public:
        TransAffineScaling(float x, float y) : TransAffine(x, 0.0f, 0.0f, y, 0.0f, 0.0f)
        {}

        TransAffineScaling(float scale) : TransAffine(scale, 0.0f, 0.0f, scale, 0.0f, 0.0f)
        {}
    };
    /**
     * @brief 位移矩阵
     * @since 1.0
     * @version 1.0
     */
    class TransAffineTranslation : public TransAffine {
    public:
        TransAffineTranslation(float x, float y) : TransAffine(1.0f, 0.0f, 0.0f, 1.0f, x, y)
        {}
    };
} // namespace OHOS
#endif
