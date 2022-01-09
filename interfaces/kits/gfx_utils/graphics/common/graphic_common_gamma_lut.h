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
 * @file graphic_common_gamma_lut.h
 *
 * @brief gamma校正查找表、Srgb查找表、Srgb转化类
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_GAMMA_LUT_INCLUDED
#define GRAPHIC_GEOMETRY_GAMMA_LUT_INCLUDED

#include <cmath>

#include "gfx_utils/graphics/common/graphic_common_basics.h"
#include "graphic_common_gamma_functions.h"
#include "gfx_utils/graphic_math.h"
namespace OHOS {
    const int TABLESIZE = 256;
    const int TABLEBOUNDARY = 255;
    const int TABLESIZE_HALF = 128;
    const int TABLESIZE_QURATER = 64;
    const int TABLESIZE_EIGHTH = 32;
    const int TABLESIZE_ONE_SIXTEENTH = 16;
    const int TABLESIZE_ONE_THIRTYTWO = 8;
    const int TABLESIZE_ONE_SIXTHFOUR = 4;
    const int TABLESIZE_ONE_ONETWENTYEIGHT = 2;
    const int TABLESIZE_ONE_TWOFIFTYSIX = 1;
    const int MOVEBIT = 8;
    const double HALF = 0.5;
    const double SRGBVALUE = 255.0;
    const double RGB16MAX = 65535.0;
    const int ALPHAMAX = 255;
    /**
     * @brief Srgb查找类
     *
     * @see SrgbLutBase
     * @since 1.0
     * @version 1.0
     */
    template <class LinearType>
    class SrgbLutBase {
    public:
        /**
         * @brief 根据下标查找dirTable_中的颜色值
         * @param index为下标值
         * @return LinearType
         * @since 1.0
         * @version 1.0
         */
        LinearType GetDirTableValue(uint8_t index) const
        {
            return dirTable_[index];
        }

        /**
         * @brief 根据lineValue值查找颜色值
         * @param lineValue为线性颜色值
         * @return 返回uint8_t类型的颜色值
         * @since 1.0
         * @version 1.0
         */
        uint8_t GetInverseTableValue(LinearType lineValue) const
        {
            // 折半查找
            uint8_t value = 0;
            if (lineValue > inverseTable_[TABLESIZE_HALF]) {
                value = TABLESIZE_HALF;
            }
            if (lineValue > inverseTable_[value + TABLESIZE_QURATER]) {
                value += TABLESIZE_QURATER;
            }
            if (lineValue > inverseTable_[value + TABLESIZE_EIGHTH]) {
                value += TABLESIZE_EIGHTH;
            }
            if (lineValue > inverseTable_[value + TABLESIZE_ONE_SIXTEENTH]) {
                value += TABLESIZE_ONE_SIXTEENTH;
            }
            if (lineValue > inverseTable_[value + TABLESIZE_ONE_THIRTYTWO]) {
                value += TABLESIZE_ONE_THIRTYTWO;
            }
            if (lineValue > inverseTable_[value + TABLESIZE_ONE_SIXTHFOUR]) {
                value += TABLESIZE_ONE_SIXTHFOUR;
            }
            if (lineValue > inverseTable_[value + TABLESIZE_ONE_ONETWENTYEIGHT]) {
                value += TABLESIZE_ONE_ONETWENTYEIGHT;
            }
            if (lineValue > inverseTable_[value + TABLESIZE_ONE_TWOFIFTYSIX]) {
                value += TABLESIZE_ONE_TWOFIFTYSIX;
            }
            return value;
        }

    protected:
        LinearType dirTable_[TABLESIZE];
        LinearType inverseTable_[TABLESIZE];

        SrgbLutBase()
        {}
    };

    template <class LinearType>
    class SrgbLut;

    /**
     * @brief Srgb查找类、float类型
     *
     * @see SrgbLut
     * @since 1.0
     * @version 1.0
     */
    template <>
    class SrgbLut<float> : public SrgbLutBase<float> {
    public:
        SrgbLut()
        {
            // 生成查找表
            dirTable_[0] = 0;
            inverseTable_[0] = 0;
            for (unsigned i = 1; i <= TABLEBOUNDARY; ++i) {
                // 浮点RGB范围在[0,1]内。
                dirTable_[i] = float(SrgbToLinear(i / SRGBVALUE));
                inverseTable_[i] = float(SrgbToLinear((i - HALF) / SRGBVALUE));
            }
        }
    };

    /**
     * @brief Srgb查找类、uint16_t类型
     *
     * @see SrgbLut
     * @since 1.0
     * @version 1.0
     */
    template <>
    class SrgbLut<uint16_t> : public SrgbLutBase<uint16_t> {
    public:
        SrgbLut()
        {
            // 生成查找表
            dirTable_[0] = 0;
            inverseTable_[0] = 0;
            for (int i = 1; i <= TABLEBOUNDARY; ++i) {
                // 16位RGB范围在[0，65535]内。
                dirTable_[i] = MATH_UROUND(RGB16MAX * SrgbToLinear(i / SRGBVALUE));
                inverseTable_[i] = MATH_UROUND(RGB16MAX * SrgbToLinear((i - HALF) / SRGBVALUE));
            }
        }
    };

    /**
     * @brief Srgb查找类、uint8_t类型
     *
     * @see SrgbLut
     * @since 1.0
     * @version 1.0
     */
    template <>
    class SrgbLut<uint8_t> : public SrgbLutBase<uint8_t> {
    public:
        SrgbLut()
        {
            // 生成查找表
            dirTable_[0] = 0;
            inverseTable_[0] = 0;
            for (int i = 1; i <= TABLEBOUNDARY; ++i) {
                // 8位RGB由简单的双向查找表处理。
                dirTable_[i] = MATH_UROUND(SRGBVALUE * SrgbToLinear(i / SRGBVALUE));
                inverseTable_[i] = MATH_UROUND(SRGBVALUE * LinearToSrgb(i / SRGBVALUE));
            }
        }

        /**
         * @brief 根据下标查找inverseTable_中的颜色值
         * @param index为下标值
         * @return 返回uint8_t类型的颜色值
         * @since 1.0
         * @version 1.0
         */
        uint8_t GetInverseTableValue(uint8_t index) const
        {
            // 在这种情况下，逆变换是一个简单的查找。
            return inverseTable_[index];
        }
    };

    /**
     * @brief Srgb转化类，添加说明
     *
     * @see StandardRgbConvBase
     * @since 1.0
     * @version 1.0
     */
    template <class T>
    class StandardRgbConvBase {
    public:
        /**
         * @brief 由Srgb值转为Rgb值
         * @param srgb为Srgb类型
         * @return 返回Rgb类型的颜色值
         * @since 1.0
         * @version 1.0
         */
        static T RgbFromSrgb(uint8_t srgb)
        {
            return lut_.GetDirTableValue(srgb);
        }

        /**
         * @brief 由Rgb值转为Srgb值
         * @param rgb为Rgb类型
         * @return 返回Srgb类型的颜色值
         * @since 1.0
         * @version 1.0
         */
        static uint8_t RgbToStandardRgb(T rgb)
        {
            return lut_.GetInverseTableValue(rgb);
        }

    private:
        static SrgbLut<T> lut_;
    };

    template <class T>
    SrgbLut<T> StandardRgbConvBase<T>::lut_;

    template <class T>
    class StandardRgbConv;

    /**
     * @brief StandardRgbConv转化类、float类型
     *
     * @see StandardRgbConv
     * @since 1.0
     * @version 1.0
     */
    template <>
    class StandardRgbConv<float> : public StandardRgbConvBase<float> {
    public:
        /**
         * @brief 由Alpha值转为Srgb值
         * @param alphaValue为Alpha值
         * @return 返回Srgb类型的颜色值
         * @since 1.0
         * @version 1.0
         */
        static uint8_t AlphaToStandardRgb(float alphaValue)
        {
            if (alphaValue < 0) {
                return 0;
            }
            if (alphaValue > 1) {
                return ALPHAMAX;
            }
            return uint8_t(HALF + alphaValue * ALPHAMAX);
        }

        /**
         * @brief 由Srgb值转为Alpha值
         * @param srgbValue为Srgb值
         * @return 返回Alpha值
         * @since 1.0
         * @version 1.0
         */
        static float AlphaFromSrgb(uint8_t srgbValue)
        {
            static const double y = 1 / SRGBVALUE;
            return float(srgbValue * y);
        }
    };

    /**
     * @brief StandardRgbConv转化类、uint16_t类型
     *
     * @see StandardRgbConv
     * @since 1.0
     * @version 1.0
     */
    template <>
    class StandardRgbConv<uint16_t> : public StandardRgbConvBase<uint16_t> {
    public:
        /**
         * @brief 由Alpha值转为Srgb值
         * @param alphaValue为Alpha值
         * @return 返回Srgb类型的颜色值
         * @since 1.0
         * @version 1.0
         */
        static uint8_t AlphaToStandardRgb(uint16_t alphaValue)
        {
            return alphaValue >> MOVEBIT;
        }

        /**
         * @brief 由Srgb值转为Alpha值
         * @param srgbValue为Srgb值
         * @return 返回Alpha值
         * @since 1.0
         * @version 1.0
         */
        static uint16_t AlphaFromSrgb(uint8_t srgbValue)
        {
            return (srgbValue << MOVEBIT) | srgbValue;
        }
    };

    /**
     * @brief StandardRgbConv转化类、uint8_t类型
     *
     * @see StandardRgbConv
     * @since 1.0
     * @version 1.0
     */
    template <>
    class StandardRgbConv<uint8_t> : public StandardRgbConvBase<uint8_t> {
    public:
        /**
         * @brief 由Srgb值转为Alpha值
         * @param srgbValue为Srgb值
         * @return 返回Alpha值
         * @since 1.0
         * @version 1.0
         */
        static uint8_t AlphaFromSrgb(uint8_t srgbValue)
        {
            return srgbValue;
        }

        /**
         * @brief 由Alpha值转为Srgb值
         * @param alphaValue为Alpha值
         * @return 返回Srgb类型的颜色值
         * @since 1.0
         * @version 1.0
         */
        static uint8_t AlphaToStandardRgb(uint8_t alphaValue)
        {
            return alphaValue;
        }
    };
} // namespace OHOS

#endif
