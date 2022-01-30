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
 * @brief Gamma correction lookup table, sRGB lookup table, sRGB conversion class
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_GAMMA_LUT_INCLUDED
#define GRAPHIC_GEOMETRY_GAMMA_LUT_INCLUDED

#include <cmath>

#include "gfx_utils/diagram/common/graphic_common_basics.h"
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
const float HALF = 0.5f;
const float SRGBVALUE = 255.0f;
const float RGB16MAX = 65535.0f;
const int ALPHAMAX = 255;
/**
 * @brief SRGB lookup class
 *
 * @see SrgbLutBase
 * @since 1.0
 * @version 1.0
 */
template <class LinearType>
class SrgbLutBase {
public:
    /**
     * @brief Find the color value in dirtable_ according to the subscript
     * @param Index is the subscript value
     * @return LinearType
     * @since 1.0
     * @version 1.0
     */
    LinearType GetDirTableValue(uint8_t index) const
    {
        return dirTable_[index];
    }

    /**
     * @brief Find color values based on linevalue values
     * @param Linevalue is a linear color value
     * @return Returns a color value of type uint8_t
     * @since 1.0
     * @version 1.0
     */
    uint8_t GetInverseTableValue(LinearType lineValue) const
    {
        // Half search
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
 * @brief SRGB lookup class, float type
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
        // Generate lookup table
        dirTable_[0] = 0;
        inverseTable_[0] = 0;
        for (unsigned i = 1; i <= TABLEBOUNDARY; ++i) {
            // Floating point RGB range is within [0,1].
            dirTable_[i] = float(SrgbToLinear(i / SRGBVALUE));
            inverseTable_[i] = float(SrgbToLinear((i - HALF) / SRGBVALUE));
        }
    }
};

/**
 * @brief SRGB lookup class, uint16_t type
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
        // Generate lookup table
        dirTable_[0] = 0;
        inverseTable_[0] = 0;
        for (int i = 1; i <= TABLEBOUNDARY; ++i) {
            // The 16 bit RGB range is within [065535].
            dirTable_[i] = MATH_UROUND(RGB16MAX * SrgbToLinear(i / SRGBVALUE));
            inverseTable_[i] = MATH_UROUND(RGB16MAX * SrgbToLinear((i - HALF) / SRGBVALUE));
        }
    }
};

/**
 * @brief SRGB lookup class, uint8_t type
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
        // Generate lookup table
        dirTable_[0] = 0;
        inverseTable_[0] = 0;
        for (int i = 1; i <= TABLEBOUNDARY; ++i) {
            // 8-bit RGB is processed by a simple bidirectional lookup table.
            dirTable_[i] = MATH_UROUND(SRGBVALUE * SrgbToLinear(i / SRGBVALUE));
            inverseTable_[i] = MATH_UROUND(SRGBVALUE * LinearToSrgb(i / SRGBVALUE));
        }
    }

    /**
     * @brief Find the color value in inverseTable_ according to the subscript
     * @param Index is the subscript value
     * @return Returns a color value of type uint8_t
     * @since 1.0
     * @version 1.0
     */
    uint8_t GetInverseTableValue(uint8_t index) const
    {
        // In this case, the inverse transform is a simple search.
        return inverseTable_[index];
    }
};

/**
 * @brief SRGB transformation class
 *
 * @see StandardRgbConvBase
 * @since 1.0
 * @version 1.0
 */
template <class T>
class StandardRgbConvBase {
public:
    /**
     * @brief From sRGB value to RGB value
     * @param srgb is sRGB type
     * @return Returns a color value of type RGB
     * @since 1.0
     * @version 1.0
     */
    static T RgbFromSrgb(uint8_t srgb)
    {
        return lut_.GetDirTableValue(srgb);
    }

    /**
     * @brief From RGB value to sRGB value
     * @param rgb is of type RGB
     * @return Returns a color value of type sRGB
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
 * @brief Standardrgbconv conversion class, float type
 *
 * @see StandardRgbConv
 * @since 1.0
 * @version 1.0
 */
template <>
class StandardRgbConv<float> : public StandardRgbConvBase<float> {
public:
    /**
     * @brief From alpha value to sRGB value
     * @param alphaValue is the alpha value
     * @return Returns a color value of type sRGB
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
     * @brief From sRGB value to alpha value
     * @param Srgbvalue is sRGB value
     * @return Returns the alpha value
     * @since 1.0
     * @version 1.0
     */
    static float AlphaFromSrgb(uint8_t srgbValue)
    {
        static const float y = 1 / SRGBVALUE;
        return float(srgbValue * y);
    }
};

/**
 * @brief StandardRgbConv conversion class, uint16_t type
 *
 * @see StandardRgbConv
 * @since 1.0
 * @version 1.0
 */
template <>
class StandardRgbConv<uint16_t> : public StandardRgbConvBase<uint16_t> {
public:
    /**
     * @brief From alpha value to sRGB value
     * @param alphaValue is the alpha value
     * @return Returns a color value of type sRGB
     * @since 1.0
     * @version 1.0
     */
    static uint8_t AlphaToStandardRgb(uint16_t alphaValue)
    {
        return alphaValue >> MOVEBIT;
    }

    /**
     * @brief From sRGB value to alpha value
     * @param Srgbvalue is sRGB value
     * @return Returns the alpha value
     * @since 1.0
     * @version 1.0
     */
    static uint16_t AlphaFromSrgb(uint8_t srgbValue)
    {
        return (srgbValue << MOVEBIT) | srgbValue;
    }
};

/**
 * @brief StandardRgbConv Conversion class, uint8_t type
 *
 * @see StandardRgbConv
 * @since 1.0
 * @version 1.0
 */
template <>
class StandardRgbConv<uint8_t> : public StandardRgbConvBase<uint8_t> {
public:
    /**
     * @brief From sRGB value to alpha value
     * @param srgbValueis sRGB value
     * @return Returns the alpha value
     * @since 1.0
     * @version 1.0
     */
    static uint8_t AlphaFromSrgb(uint8_t srgbValue)
    {
        return srgbValue;
    }

    /**
     * @brief From alpha value to sRGB value
     * @param alphaValue is the alpha value
     * @return Returns a color value of type sRGB
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
