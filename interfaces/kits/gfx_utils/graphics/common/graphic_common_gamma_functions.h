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
 * @file graphic_common_gamma_functions.h
 *
 * @brief Gamma correction method
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_GAMMA_FUNCTIONS_INCLUDED
#define GRAPHIC_GEOMETRY_GAMMA_FUNCTIONS_INCLUDED

#include "gfx_utils/graphics/common/graphic_common_basics.h"

namespace OHOS {
    const double LINEARVALUE = 0.0031308;
    const double SRGB_VALUE = 0.04045;
    const double CARDINAL_NUMBER = 12.92;
    const double BENCHMARK = 0.055;
    const double BASE_VALUE = 1.055;
    const double POW_VALUE = 2.4;
    struct GammaNone {
        double operator()(double x) const
        {
            return x;
        }
    };

    /**
     * @brief Find the value of the gamma power of base
     *
     * @see GammaPower
     * @since 1.0
     * @version 1.0
     */
    class GammaPower {
    public:
        GammaPower() : gamma_(1.0)
        {}
        GammaPower(double gamma) : gamma_(gamma)
        {}

        void Gamma(double gamma)
        {
            gamma_ = gamma;
        }
        double Gamma() const
        {
            return gamma_;
        }

        double operator()(double base) const
        {
            return pow(base, gamma_);
        }

    private:
        double gamma_;
    };

    /**
     * @brief Gamma number multiplication
     *
     * @see GammaMultiply
     * @since 1.0
     * @version 1.0
     */
    class GammaMultiply {
    public:
        GammaMultiply() : multiply_(1.0)
        {}
        GammaMultiply(double value) : multiply_(value)
        {}

        void Value(double value)
        {
            multiply_ = value;
        }
        double Value() const
        {
            return multiply_;
        }

        double operator()(double coefficient) const
        {
            double gammaValue = coefficient * multiply_;
            if (gammaValue > 1.0) {
                gammaValue = 1.0;
            }
            return gammaValue;
        }

    private:
        double multiply_;
    };

    /**
     * @brief From linear type to sRGB type
     * @param Linearvalue is of linear type
     * @return Returns a color value of type sRGB
     * @since 1.0
     * @version 1.0
     */
    inline double LinearToSrgb(double linearValue)
    {
        return (linearValue <= LINEARVALUE) ?
                   (linearValue * CARDINAL_NUMBER) :
                   (BASE_VALUE * pow(linearValue, 1 / POW_VALUE) - BENCHMARK);
    }

    /**
     * @brief From sRGB type to linear type
     * @param Srgbvalue is sRGB type
     * @return Returns a color value of type linear
     * @since 1.0
     * @version 1.0
     */
    inline double SrgbToLinear(double srgbValue)
    {
        return (srgbValue <= SRGB_VALUE) ?
                   (srgbValue / CARDINAL_NUMBER) :
                   pow((srgbValue + BENCHMARK) / (BASE_VALUE), POW_VALUE);
    }
} // namespace OHOS

#endif
