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
 * @file graphic_geometry_color_rgba.h
 *
 * @brief 颜色格式：Rgba,Rgba32
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_COLOR_RGBA_INCLUDED
#define GRAPHIC_GEOMETRY_COLOR_RGBA_INCLUDED

#include <cmath>

#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "gfx_utils/graphics/graphic_common/agg_gamma_lut.h"

namespace OHOS {
#define PURPLE_MIN 380.0
#define PURPLE_MIDDLE 420.0
#define PURPLE_MAX 440.0
#define BLUE_MAX 490.0
#define CYAN_MAX 510.0
#define GREEN_MAX 580.0
#define ORANGE_MAX 645.0
#define RED_MIN 700.0
#define RED_MAX 780.0
#define COEFFICIENT 0.7
#define FIXED_VALUE 0.3

    struct OrderRgb {
        enum RgbEnum
        {
            RED = 0,
            GREEN = 1,
            BLUE = 2,
            N = 3
        };
    };

    struct OrderBgr {
        enum BgrEnum
        {
            BLUE = 0,
            GREEN = 1,
            RED = 2,
            N = 3
        };
    };

    struct OrderRgba {
        enum RgbaEnum
        {
            RED = 0,
            GREEN = 1,
            BLUE = 2,
            ALPHA = 3,
            N = 4
        };
    };

    struct OrderArgb {
        enum ArgbEnum
        {
            ALPHA = 0,
            RED = 1,
            GREEN = 2,
            BLUE = 3,
            N = 4
        };
    };
    struct OrderAbgr {
        enum AbgrEnum
        {
            ALPHA = 0,
            BLUE = 1,
            GREEN = 2,
            RED = 3,
            N = 4
        };
    };
    struct OrderBgra {
        enum BgraEnum
        {
            BLUE = 0,
            GREEN = 1,
            RED = 2,
            ALPHA = 3,
            N = 4
        };
    };

    struct Linear {
    };
    struct Srgb {
    };

    /**
     * @brief Rgba
     *
     * 颜色排列顺序：红、绿、蓝、透明度
     *
     * @see Rgba
     * @since 1.0
     * @version 1.0
     */
    struct Rgba {
        using ValueType = double;

        double redValue;
        double greenValue;
        double blueValue;
        double alphaValue;

        Rgba()
        {}

        /**
         * @brief Rgba构造函数
         *
         * @param red红色值、green绿色值、blue蓝色值、alpha透明度
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        Rgba(double red, double green, double blue, double alpha = 1.0) :
            redValue(red), greenValue(green), blueValue(blue), alphaValue(alpha)
        {}

        /**
         * @brief Rgba构造函数
         *
         * @param color是Rgba对象、alpha透明度
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        Rgba(const Rgba& color, double alpha) :
            redValue(color.redValue), greenValue(color.greenValue), blueValue(color.blueValue), alphaValue(alpha)
        {}

        /**
         * @brief 清除，颜色透明度置0
         *
         * @param 无
         * @return 返回Rgba对象的引用
         * @since 1.0
         * @version 1.0
         */
        Rgba& Clear()
        {
            redValue = 0;
            greenValue = 0;
            blueValue = 0;
            alphaValue = 0;
            return *this;
        }

        /**
         * @brief 全透明
         *
         * @param 无
         * @return 返回Rgba对象的引用
         * @since 1.0
         * @version 1.0
         */
        Rgba& Transparent()
        {
            alphaValue = 0;
            return *this;
        }

        /**
         * @brief 设置透明度
         *
         * @param alpha 透明度
         * @return 返回Rgba对象的引用
         * @since 1.0
         * @version 1.0
         */
        Rgba& Opacity(double alpha)
        {
            if (alpha < 0) {
                alphaValue = 0;
            } else if (alpha > 1) {
                alphaValue = 1;
            } else {
                alphaValue = alpha;
            }
            return *this;
        }

        /**
         * @brief 获取透明度
         *
         * @param 无
         * @return 返回透明度
         * @since 1.0
         * @version 1.0
         */
        double Opacity() const
        {
            return alphaValue;
        }

        /**
         * @brief 预乘
         *
         * @param 无
         * @return 返回Rgba对象的引用
         * @since 1.0
         * @version 1.0
         */
        Rgba& Premultiply()
        {
            redValue *= alphaValue;
            greenValue *= alphaValue;
            blueValue *= alphaValue;
            return *this;
        }

        /**
         * @brief 预乘
         *
         * @param alpha 透明度
         * @return 返回Rgba对象的引用
         * @since 1.0
         * @version 1.0
         */
        Rgba& Premultiply(double alpha)
        {
            if (alphaValue <= 0 || alpha <= 0) {
                redValue = 0;
                greenValue = 0;
                blueValue = 0;
                alphaValue = 0;
            } else {
                alpha /= alphaValue;
                redValue *= alpha;
                greenValue *= alpha;
                blueValue *= alpha;
                alphaValue = alpha;
            }
            return *this;
        }

        /**
         * @brief 倍减
         *
         * @param 无
         * @return 返回Rgba对象的引用
         * @since 1.0
         * @version 1.0
         */
        Rgba& Demultiply()
        {
            if (alphaValue == 0) {
                redValue = 0;
                greenValue = 0;
                blueValue = 0;
            } else {
                double alpha = 1.0 / alphaValue;
                redValue *= alpha;
                greenValue *= alpha;
                blueValue *= alpha;
            }
            return *this;
        }

        /**
         * @brief 渐变
         *
         * @param rgba为Rgba对象，k为变化系数
         * @return 返回Rgba对象
         * @since 1.0
         * @version 1.0
         */
        Rgba Gradient(Rgba rgba, double k) const
        {
            Rgba ret;
            ret.redValue = redValue + (rgba.redValue - redValue) * k;
            ret.greenValue = greenValue + (rgba.greenValue - greenValue) * k;
            ret.blueValue = blueValue + (rgba.blueValue - blueValue) * k;
            ret.alphaValue = alphaValue + (rgba.alphaValue - alphaValue) * k;
            return ret;
        }

        /**
         * @brief 重载操作符+=函数
         *
         * @param rgba为Rgba对象的引用
         * @return 返回Rgba对象的引用
         * @since 1.0
         * @version 1.0
         */
        Rgba& operator+=(const Rgba& rgba)
        {
            redValue += rgba.redValue;
            greenValue += rgba.greenValue;
            blueValue += rgba.blueValue;
            alphaValue += rgba.alphaValue;
            return *this;
        }

        /**
         * @brief 重载操作符*=函数
         *
         * @param multiplyValue为相乘的系数
         * @return 返回Rgba对象的引用
         * @since 1.0
         * @version 1.0
         */
        Rgba& operator*=(double multiplyValue)
        {
            redValue *= multiplyValue;
            greenValue *= multiplyValue;
            blueValue *= multiplyValue;
            alphaValue *= multiplyValue;
            return *this;
        }

        static Rgba NoColor()
        {
            return Rgba(0, 0, 0, 0);
        }

        /**
         * @brief 根据波长以及gamma值计算颜色值
         *
         * @param waveLength波长，gamma伽马值
         * @return 返回Rgba对象
         * @since 1.0
         * @version 1.0
         */
        static Rgba FromWavelength(double waveLength, double gamma = 1.0);

        explicit Rgba(double wavelen, double gamma = 1.0)
        {
            *this = FromWavelength(wavelen, gamma);
        }
    };

    /**
     * @brief 重载操作符+
     *
     * @param rgbaA是Rgba对象的常引用、rgbaB是Rgba对象的常引用
     * @return 返回Rgba对象
     * @since 1.0
     * @version 1.0
     */
    inline Rgba operator+(const Rgba& rgbaA, const Rgba& rgbaB)
    {
        return Rgba(rgbaA) += rgbaB;
    }

    /**
     * @brief 重载操作符*
     *
     * @param a是Rgba对象的常引用、multiplyValue是相乘的系数
     * @return 返回Rgba对象
     * @since 1.0
     * @version 1.0
     */
    inline Rgba operator*(const Rgba& rgbaA, double multiplyValue)
    {
        return Rgba(rgbaA) *= multiplyValue;
    }

    inline Rgba Rgba::FromWavelength(double waveLength, double gamma)
    {
        Rgba rgba(0.0, 0.0, 0.0);

        if (waveLength >= PURPLE_MIN && waveLength <= PURPLE_MAX) {
            rgba.redValue = -1.0 * (waveLength - PURPLE_MAX) / (PURPLE_MAX - PURPLE_MIN);
            rgba.blueValue = 1.0;
        } else if (waveLength >= PURPLE_MAX && waveLength <= BLUE_MAX) {
            rgba.greenValue = (waveLength - PURPLE_MAX) / (BLUE_MAX - PURPLE_MAX);
            rgba.blueValue = 1.0;
        } else if (waveLength >= BLUE_MAX && waveLength <= CYAN_MAX) {
            rgba.greenValue = 1.0;
            rgba.blueValue = -1.0 * (waveLength - CYAN_MAX) / (CYAN_MAX - BLUE_MAX);
        } else if (waveLength >= CYAN_MAX && waveLength <= GREEN_MAX) {
            rgba.redValue = (waveLength - CYAN_MAX) / (GREEN_MAX - CYAN_MAX);
            rgba.greenValue = 1.0;
        } else if (waveLength >= GREEN_MAX && waveLength <= ORANGE_MAX) {
            rgba.redValue = 1.0;
            rgba.greenValue = -1.0 * (waveLength - ORANGE_MAX) / (ORANGE_MAX - GREEN_MAX);
        } else if (waveLength >= ORANGE_MAX && waveLength <= RED_MAX) {
            rgba.redValue = 1.0;
        }

        // 计算比率系数
        double ratio = 1.0;
        if (waveLength > RED_MIN) {
            ratio = FIXED_VALUE + COEFFICIENT * (RED_MAX - waveLength) / (RED_MAX - RED_MIN);
        } else if (waveLength < PURPLE_MIDDLE) {
            ratio = FIXED_VALUE + COEFFICIENT * (waveLength - PURPLE_MIN) / (PURPLE_MIDDLE - PURPLE_MIN);
        }

        rgba.redValue = std::pow(rgba.redValue * ratio, gamma);
        rgba.greenValue = std::pow(rgba.greenValue * ratio, gamma);
        rgba.blueValue = std::pow(rgba.blueValue * ratio, gamma);
        return rgba;
    }

    /**
     * @brief Rgba预乘
     *
     * @param redValue红色值、greenValue绿色值、blueValue蓝色值、alphaValue透明度
     * @return 返回Rgba对象
     * @since 1.0
     * @version 1.0
     */
    inline Rgba RgbaPre(double redValue, double greenValue, double blueValue, double alphaValue)
    {
        return Rgba(redValue, greenValue, blueValue, alphaValue).Premultiply();
    }

    /**
     * @brief Rgba8T颜色序列转化
     *
     * 颜色排列顺序：红、绿、蓝、透明度
     *
     * @see Rgba8T
     * @since 1.0
     * @version 1.0
     */
    template <class Colorspace>
    struct Rgba8T {
        using ValueType = int8u;
        using CalcType = int32u;
        using LongType = int32;
        using SelfType = Rgba8T;

        ValueType redValue;
        ValueType greenValue;
        ValueType blueValue;
        ValueType alphaValue;

        enum BaseScaleEnum
        {
            BASESHIFT = 8,
            BASESCALE = 1 << BASESHIFT,
            BASEMASK = BASESCALE - 1,
            BASEMSB = 1 << (BASESHIFT - 1)
        };

        Rgba8T()
        {}

        /**
         * @brief Rgba8T构造函数
         *
         * @param red红色值、green绿色值、blue蓝色值、alpha透明度
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        Rgba8T(unsigned red, unsigned green, unsigned blue, unsigned alpha = BASEMASK) :
            redValue(ValueType(red)),
            greenValue(ValueType(green)),
            blueValue(ValueType(blue)),
            alphaValue(ValueType(alpha))
        {}

        /**
         * @brief Rgba8T构造函数
         *
         * @param color为Rgba对象的引用
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        Rgba8T(const Rgba& color)
        {
            Convert(*this, color);
        }

        /**
         * @brief Rgba8T构造函数
         *
         * @param color为Rgba8T对象的引用，alpha为透明度
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        Rgba8T(const SelfType& color, unsigned alpha) :
            redValue(color.redValue),
            greenValue(color.greenValue),
            blueValue(color.blueValue),
            alphaValue(ValueType(alpha))
        {}

        /**
         * @brief Rgba8T构造函数
         *
         * @param color为Rgba8T<T>对象的引用
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        template <class T>
        Rgba8T(const Rgba8T<T>& color)
        {
            Convert(*this, color);
        }

        /**
         * @brief 重载Rgba函数
         *
         * @param 无
         * @return 返回Rgba对象
         * @since 1.0
         * @version 1.0
         */
        operator Rgba() const
        {
            Rgba color;
            Convert(color, *this);
            return color;
        }

        /**
         * @brief 将Rgba8T<Srgb>中的颜色值赋值到Rgba8T<Linear>中
         *
         * @param dst为Rgba8T<Linear>对象的引用，src为Rgba8T<Srgb>对象的常引用
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        static void Convert(Rgba8T<Linear>& dst, const Rgba8T<Srgb>& src)
        {
            dst.redValue = SrgbConv<ValueType>::RgbFromSrgb(src.redValue);
            dst.greenValue = SrgbConv<ValueType>::RgbFromSrgb(src.greenValue);
            dst.blueValue = SrgbConv<ValueType>::RgbFromSrgb(src.blueValue);
            dst.alphaValue = src.alphaValue;
        }

        /**
         * @brief 将Rgba8T<Linear>中的颜色值赋值到Rgba8T<Srgb>中
         *
         * @param dst为Rgba8T<Srgb>对象的引用，src为Rgba8T<Linear>对象的常引用
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        static void Convert(Rgba8T<Srgb>& dst, const Rgba8T<Linear>& src)
        {
            dst.redValue = SrgbConv<ValueType>::RgbToSrgb(src.redValue);
            dst.greenValue = SrgbConv<ValueType>::RgbToSrgb(src.greenValue);
            dst.blueValue = SrgbConv<ValueType>::RgbToSrgb(src.blueValue);
            dst.alphaValue = src.alphaValue;
        }

        /**
         * @brief 将Rgba中的颜色值赋值到Rgba8T<Linear>中
         *
         * @param dst为Rgba8T<Linear>对象的引用，src为Rgba对象的常引用
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        static void Convert(Rgba8T<Linear>& dst, const Rgba& src)
        {
            dst.redValue = ValueType(Uround(src.redValue * BASEMASK));
            dst.greenValue = ValueType(Uround(src.greenValue * BASEMASK));
            dst.blueValue = ValueType(Uround(src.blueValue * BASEMASK));
            dst.alphaValue = ValueType(Uround(src.alphaValue * BASEMASK));
        }

        /**
         * @brief 将Rgba中的颜色值赋值到Rgba8T<Srgb>中
         *
         * @param dst为Rgba8T<Srgb>对象的引用，src为Rgba对象的常引用
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        static void Convert(Rgba8T<Srgb>& dst, const Rgba& src)
        {
            dst.redValue = SrgbConv<float>::RgbToSrgb(float(src.redValue));
            dst.greenValue = SrgbConv<float>::RgbToSrgb(float(src.greenValue));
            dst.blueValue = SrgbConv<float>::RgbToSrgb(float(src.blueValue));
            dst.alphaValue = SrgbConv<float>::AlphaToSrgb(float(src.alphaValue));
        }

        /**
         * @brief 将Rgba8T<Linear>中的颜色值赋值到Rgba中
         *
         * @param dst为Rgba对象的引用，src为Rgba8T<Linear>对象的常引用
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        static void Convert(Rgba& dst, const Rgba8T<Linear>& src)
        {
            dst.redValue = src.redValue / 255.0;
            dst.greenValue = src.greenValue / 255.0;
            dst.blueValue = src.blueValue / 255.0;
            dst.alphaValue = src.alphaValue / 255.0;
        }

        /**
         * @brief 将Rgba8T<Srgb>中的颜色值赋值到Rgba中
         *
         * @param dst为Rgba对象的引用，src为Rgba8T<Srgb>对象的常引用
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        static void Convert(Rgba& dst, const Rgba8T<Srgb>& src)
        {
            dst.redValue = SrgbConv<float>::RgbFromSrgb(src.redValue);
            dst.greenValue = SrgbConv<float>::RgbFromSrgb(src.greenValue);
            dst.blueValue = SrgbConv<float>::RgbFromSrgb(src.blueValue);
            dst.alphaValue = SrgbConv<float>::AlphaFromSrgb(src.alphaValue);
        }

        static GRAPHIC_GEOMETRY_INLINE double ToDouble(ValueType valueType)
        {
            return double(valueType) / BASEMASK;
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType FromDouble(double value)
        {
            return ValueType(Uround(value * BASEMASK));
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType EmptyValue()
        {
            return 0;
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType FullValue()
        {
            return BASEMASK;
        }

        GRAPHIC_GEOMETRY_INLINE bool IsTransparent() const
        {
            return alphaValue == 0;
        }

        GRAPHIC_GEOMETRY_INLINE bool IsOpaque() const
        {
            return alphaValue == BASEMASK;
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType Invert(ValueType valueType)
        {
            return BASEMASK - valueType;
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType Multiply(ValueType valueA, ValueType valueB)
        {
            CalcType calcType = valueA * valueB + BASEMSB;
            return ValueType(((calcType >> BASESHIFT) + calcType) >> BASESHIFT);
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType Demultiply(ValueType valueA, ValueType valueB)
        {
            if (valueA * valueB == 0) {
                return 0;
            } else if (valueA >= valueB) {
                return BASEMASK;
            } else {
                return ValueType((valueA * BASEMASK + (valueB >> 1)) / valueB);
            }
        }

        template <typename T>
        static GRAPHIC_GEOMETRY_INLINE T Downshift(T value, unsigned digit)
        {
            return value >> digit;
        }

        template <typename T>
        static GRAPHIC_GEOMETRY_INLINE T Downscale(T value)
        {
            return value >> BASESHIFT;
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType MultCover(ValueType valueA, CoverType coverValue)
        {
            return Multiply(valueA, coverValue);
        }

        static GRAPHIC_GEOMETRY_INLINE CoverType ScaleCover(CoverType coverValue, ValueType value)
        {
            return Multiply(value, coverValue);
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType Prelerp(ValueType valueP, ValueType valueQ, ValueType valueA)
        {
            return valueP + valueQ - Multiply(valueP, valueA);
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType Lerp(ValueType valueP, ValueType valueQ, ValueType valueA)
        {
            int t = (valueQ - valueP) * valueA + BASEMSB - (valueP > valueQ);
            return ValueType(valueP + (((t >> BASESHIFT) + t) >> BASESHIFT));
        }

        SelfType& Clear()
        {
            redValue = 0;
            greenValue = 0;
            blueValue = 0;
            alphaValue = 0;
            return *this;
        }

        SelfType& Transparent()
        {
            alphaValue = 0;
            return *this;
        }

        /**
         * @brief 设置透明度
         *
         * @param alpha为透明度
         * @return 返回Rgba8T对象的引用
         * @since 1.0
         * @version 1.0
         */
        SelfType& Opacity(double alpha)
        {
            if (alpha < 0) {
                alphaValue = 0;
            } else if (alpha > 1) {
                alphaValue = 1;
            } else {
                alphaValue = (ValueType)Uround(alpha * double(BASEMASK));
            }
            return *this;
        }

        double Opacity() const
        {
            return double(alphaValue) / double(BASEMASK);
        }

        /**
         * @brief 预乘
         *
         * @param 无
         * @return 返回Rgba8T对象的引用
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE SelfType& Premultiply()
        {
            if (alphaValue != BASEMASK) {
                if (alphaValue == 0) {
                    redValue = 0;
                    greenValue = 0;
                    blueValue = 0;
                } else {
                    redValue = Multiply(redValue, alphaValue);
                    greenValue = Multiply(greenValue, alphaValue);
                    blueValue = Multiply(blueValue, alphaValue);
                }
            }
            return *this;
        }

        /**
         * @brief 预乘
         *
         * @param alpha为透明度
         * @return 返回Rgba8T对象的引用
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE SelfType& Premultiply(unsigned alpha)
        {
            if (alphaValue != BASEMASK || alpha < BASEMASK) {
                if (alphaValue == 0 || alpha == 0) {
                    redValue = 0;
                    greenValue = 0;
                    blueValue = 0;
                    alphaValue = 0;
                } else {
                    CalcType red = (CalcType(redValue) * alpha) / alphaValue;
                    CalcType green = (CalcType(greenValue) * alpha) / alphaValue;
                    CalcType blue = (CalcType(blueValue) * alpha) / alphaValue;
                    redValue = ValueType((red > alpha) ? alpha : red);
                    greenValue = ValueType((green > alpha) ? alpha : green);
                    blueValue = ValueType((blue > alpha) ? alpha : blue);
                    alphaValue = ValueType(alpha);
                }
            }
            return *this;
        }

        /**
         * @brief 倍减
         *
         * @param 无
         * @return 返回Rgba8T对象的引用
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE SelfType& Demultiply()
        {
            if (alphaValue < BASEMASK) {
                if (alphaValue == 0) {
                    redValue = 0;
                    greenValue = 0;
                    blueValue = 0;
                } else {
                    // 颜色值 * 255 / 透明度
                    CalcType red = (CalcType(redValue) * BASEMASK) / alphaValue;
                    CalcType green = (CalcType(greenValue) * BASEMASK) / alphaValue;
                    CalcType blue = (CalcType(blueValue) * BASEMASK) / alphaValue;
                    redValue = ValueType((red > CalcType(BASEMASK)) ? CalcType(BASEMASK) : red);
                    greenValue = ValueType((green > CalcType(BASEMASK)) ? CalcType(BASEMASK) : green);
                    blueValue = ValueType((blue > CalcType(BASEMASK)) ? CalcType(BASEMASK) : blue);
                }
            }
            return *this;
        }

        /**
         * @brief 渐变，根据变化系数计算出新的Rgba8T对象
         *
         * @param color为Rgba8T对象的引用，k为变化系数
         * @return 返回Rgba8T对象
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE SelfType Gradient(const SelfType& color, double k) const
        {
            SelfType ret;
            CalcType increaseK = Uround(k * BASEMASK);
            ret.redValue = Lerp(redValue, color.redValue, increaseK);
            ret.greenValue = Lerp(greenValue, color.greenValue, increaseK);
            ret.blueValue = Lerp(blueValue, color.blueValue, increaseK);
            ret.alphaValue = Lerp(alphaValue, color.alphaValue, increaseK);
            return ret;
        }

        /**
         * @brief 根据Rgba8T对象和掩码值计算颜色值
         *
         * @param color为Rgba8T对象的常引用，cover为掩码值
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void Add(const SelfType& color, unsigned cover)
        {
            CalcType calcRed;
            CalcType calcGreen;
            CalcType calcBlue;
            CalcType calcAlpha;
            if (cover == COVER_MASK) {
                if (color.alphaValue == BASEMASK) {
                    *this = color;
                    return;
                } else {
                    calcRed = redValue + color.redValue;
                    calcGreen = greenValue + color.greenValue;
                    calcBlue = blueValue + color.blueValue;
                    calcAlpha = alphaValue + color.alphaValue;
                }
            } else {
                calcRed = redValue + MultCover(color.redValue, cover);
                calcGreen = greenValue + MultCover(color.greenValue, cover);
                calcBlue = blueValue + MultCover(color.blueValue, cover);
                calcAlpha = alphaValue + MultCover(color.alphaValue, cover);
            }
            redValue = (ValueType)((calcRed > CalcType(BASEMASK)) ? CalcType(BASEMASK) : calcRed);
            greenValue = (ValueType)((calcGreen > CalcType(BASEMASK)) ? CalcType(BASEMASK) : calcGreen);
            blueValue = (ValueType)((calcBlue > CalcType(BASEMASK)) ? CalcType(BASEMASK) : calcBlue);
            alphaValue = (ValueType)((calcAlpha > CalcType(BASEMASK)) ? CalcType(BASEMASK) : calcAlpha);
        }

        template <class GammaLUT>
        GRAPHIC_GEOMETRY_INLINE void ApplyGammaDir(const GammaLUT& gamma)
        {
            redValue = gamma.GetDirTableValue(redValue);
            greenValue = gamma.GetDirTableValue(greenValue);
            blueValue = gamma.GetDirTableValue(blueValue);
        }

        template <class GammaLUT>
        GRAPHIC_GEOMETRY_INLINE void ApplyGammaInv(const GammaLUT& gamma)
        {
            redValue = gamma.GetInverseTableValue(redValue);
            greenValue = gamma.GetInverseTableValue(greenValue);
            blueValue = gamma.GetInverseTableValue(blueValue);
        }

        static SelfType FromWavelength(double waveLength, double gamma = 1.0)
        {
            return SelfType(Rgba::FromWavelength(waveLength, gamma));
        }

        static SelfType NoColor()
        {
            return SelfType(0, 0, 0, 0);
        }
    };

    using Rgba8 = Rgba8T<Linear>;
    using Srgba8 = Rgba8T<Srgb>;

    inline Rgba8 Rgb8Packed(unsigned value)
    {
        return Rgba8((value >> 0x10) & 0xFF, (value >> 0x08) & 0xFF, value & 0xFF);
    }

    inline Rgba8 Bgr8Packed(unsigned value)
    {
        return Rgba8(value & 0xFF, (value >> 0x08) & 0xFF, (value >> 0x10) & 0xFF);
    }

    inline Rgba8 Argb8Packed(unsigned value)
    {
        return Rgba8((value >> 0x10) & 0xFF, (value >> 0x08) & 0xFF, value & 0xFF, value >> 0x18);
    }

    template <class GammaLUT>
    Rgba8 Rgba8GammaDir(Rgba8 color, const GammaLUT& gamma)
    {
        return Rgba8(gamma.GetDirTableValue(color.redValue), gamma.GetDirTableValue(color.greenValue),
                     gamma.GetDirTableValue(color.blueValue), color.alphaValue);
    }

    template <class GammaLUT>
    Rgba8 Rgba8GammaInv(Rgba8 color, const GammaLUT& gamma)
    {
        return Rgba8(gamma.GetInverseTableValue(color.redValue), gamma.GetInverseTableValue(color.greenValue),
                     gamma.GetInverseTableValue(color.blueValue), color.alphaValue);
    }

    /**
     * @brief Rgba32颜色序列转化
     *
     * 颜色排列顺序：红、绿、蓝、透明度
     *
     * @see Rgba32
     * @since 1.0
     * @version 1.0
     */
    struct Rgba32 {
        using ValueType = float;
        using CalcType = double;
        using LongType = double;
        using SelfType = Rgba32;

        ValueType redValue;
        ValueType greenValue;
        ValueType blueValue;
        ValueType alphaValue;

        Rgba32()
        {}

        /**
         * @brief Rgba32构造函数
         *
         * @param red红色值、green绿色值、blue蓝色值、alpha透明度
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        Rgba32(ValueType red, ValueType green, ValueType blue, ValueType alpha = 1) :
            redValue(red), greenValue(green), blueValue(blue), alphaValue(alpha)
        {}

        /**
         * @brief Rgba32构造函数
         *
         * @param color为Rgba32对象的常引用、alpha透明度
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        Rgba32(const SelfType& color, float alpha) :
            redValue(color.redValue), greenValue(color.greenValue), blueValue(color.blueValue), alphaValue(alpha)
        {}

        /**
         * @brief Rgba32构造函数
         *
         * @param color为Rgba对象的常引用
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        Rgba32(const Rgba& color) :
            redValue(ValueType(color.redValue)),
            greenValue(ValueType(color.greenValue)),
            blueValue(ValueType(color.blueValue)),
            alphaValue(ValueType(color.alphaValue))
        {}

        /**
         * @brief Rgba32构造函数
         *
         * @param color为Rgba8对象的常引用
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        Rgba32(const Rgba8& color) :
            redValue(ValueType(color.redValue / 255.0)),
            greenValue(ValueType(color.greenValue / 255.0)),
            blueValue(ValueType(color.blueValue / 255.0)),
            alphaValue(ValueType(color.alphaValue / 255.0))
        {}

        /**
         * @brief Rgba32构造函数
         *
         * @param color为Srgba8对象的常引用
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        Rgba32(const Srgba8& color) :
            redValue(SrgbConv<ValueType>::RgbFromSrgb(color.redValue)),
            greenValue(SrgbConv<ValueType>::RgbFromSrgb(color.greenValue)),
            blueValue(SrgbConv<ValueType>::RgbFromSrgb(color.blueValue)),
            alphaValue(SrgbConv<ValueType>::AlphaFromSrgb(color.alphaValue))
        {}

        operator Rgba() const
        {
            return Rgba(redValue, greenValue, blueValue, alphaValue);
        }

        operator Rgba8() const
        {
            return Rgba8(
                Uround(redValue * 255.0),
                Uround(greenValue * 255.0),
                Uround(blueValue * 255.0),
                Uround(alphaValue * 255.0));
        }

        operator Srgba8() const
        {
            return Srgba8(
                SrgbConv<ValueType>::RgbToSrgb(redValue),
                SrgbConv<ValueType>::RgbToSrgb(greenValue),
                SrgbConv<ValueType>::RgbToSrgb(blueValue),
                SrgbConv<ValueType>::AlphaToSrgb(alphaValue));
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType FromDouble(double value)
        {
            return ValueType(value);
        }

        static GRAPHIC_GEOMETRY_INLINE double ToDouble(ValueType valueType)
        {
            return valueType;
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType FullValue()
        {
            return 1;
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType EmptyValue()
        {
            return 0;
        }

        GRAPHIC_GEOMETRY_INLINE bool IsOpaque() const
        {
            return alphaValue >= 1;
        }

        GRAPHIC_GEOMETRY_INLINE bool IsTransparent() const
        {
            return alphaValue <= 0;
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType Invert(ValueType valueType)
        {
            return 1 - valueType;
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType Multiply(ValueType valueA, ValueType valueB)
        {
            return ValueType(valueA * valueB);
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType Demultiply(ValueType valueA, ValueType valueB)
        {
            return (valueA == 0) ? 0 : ValueType(valueA / valueB);
        }

        template <typename T>
        static GRAPHIC_GEOMETRY_INLINE T Downscale(T value)
        {
            return value;
        }

        template <typename T>
        static GRAPHIC_GEOMETRY_INLINE T Downshift(T value, unsigned digit)
        {
            return digit > 0 ? value / (1 << digit) : value;
        }

        static GRAPHIC_GEOMETRY_INLINE ValueType MultCover(ValueType valueType, CoverType coverValue)
        {
            return ValueType(valueType * coverValue / COVER_MASK);
        }

        static GRAPHIC_GEOMETRY_INLINE CoverType ScaleCover(CoverType coverValue, ValueType valueType)
        {
            return CoverType(Uround(coverValue * valueType));
        }

        // valueP valueQ中插值，ratio为比例
        static GRAPHIC_GEOMETRY_INLINE ValueType Prelerp(ValueType valueP, ValueType valueQ, ValueType ratio)
        {
            return (1 - ratio) * valueP + valueQ;
        }

        // valueP valueQ中插值，ratio为比例
        static GRAPHIC_GEOMETRY_INLINE ValueType Lerp(ValueType valueP, ValueType valueQ, ValueType ratio)
        {
            return (1 - ratio) * valueP + ratio * valueQ;
        }

        SelfType& Clear()
        {
            redValue = 0;
            greenValue = 0;
            blueValue = 0;
            alphaValue = 0;
            return *this;
        }

        SelfType& Transparent()
        {
            alphaValue = 0;
            return *this;
        }

        GRAPHIC_GEOMETRY_INLINE SelfType& Opacity(double alpha)
        {
            if (alpha < 0) {
                alphaValue = 0;
            } else if (alpha > 1) {
                alphaValue = 1;
            } else {
                alphaValue = ValueType(alpha);
            }
            return *this;
        }

        double Opacity() const
        {
            return alphaValue;
        }

        GRAPHIC_GEOMETRY_INLINE SelfType& Premultiply()
        {
            if (alphaValue < 1) {
                if (alphaValue <= 0) {
                    redValue = 0;
                    greenValue = 0;
                    blueValue = 0;
                } else {
                    redValue *= alphaValue;
                    greenValue *= alphaValue;
                    blueValue *= alphaValue;
                }
            }
            return *this;
        }

        GRAPHIC_GEOMETRY_INLINE SelfType& Demultiply()
        {
            if (alphaValue < 1) {
                if (alphaValue <= 0) {
                    redValue = 0;
                    greenValue = 0;
                    blueValue = 0;
                } else {
                    redValue /= alphaValue;
                    greenValue /= alphaValue;
                    blueValue /= alphaValue;
                }
            }
            return *this;
        }

        /**
         * @brief 根据Rgba32对象和系数计算新的Rgba32对象
         *
         * @param color为Rgba32对象的常引用，k为比例
         * @return 返回Rgba32对象的引用
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE SelfType Gradient(const SelfType& color, double k) const
        {
            SelfType ret;
            ret.redValue = ValueType(redValue + (color.redValue - redValue) * k);
            ret.greenValue = ValueType(greenValue + (color.greenValue - greenValue) * k);
            ret.blueValue = ValueType(blueValue + (color.blueValue - blueValue) * k);
            ret.alphaValue = ValueType(alphaValue + (color.alphaValue - alphaValue) * k);
            return ret;
        }

        /**
         * @brief 根据Rgba32对象和掩码值计算颜色值
         *
         * @param color为Rgba32对象的常引用，cover为掩码值
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE void Add(const SelfType& color, unsigned cover)
        {
            if (cover == COVER_MASK) {
                if (color.IsOpaque()) {
                    *this = color;
                    return;
                } else {
                    redValue += color.redValue;
                    greenValue += color.greenValue;
                    blueValue += color.blueValue;
                    alphaValue += color.alphaValue;
                }
            } else {
                redValue += MultCover(color.redValue, cover);
                greenValue += MultCover(color.greenValue, cover);
                blueValue += MultCover(color.blueValue, cover);
                alphaValue += MultCover(color.alphaValue, cover);
            }
            if (alphaValue > 1) {
                alphaValue = 1;
            }
            if (redValue > alphaValue) {
                redValue = alphaValue;
            }
            if (greenValue > alphaValue) {
                greenValue = alphaValue;
            }
            if (blueValue > alphaValue) {
                blueValue = alphaValue;
            }
        }

        template <class GammaLUT>
        GRAPHIC_GEOMETRY_INLINE void ApplyGammaDir(const GammaLUT& gamma)
        {
            redValue = gamma.GetDirTableValue(redValue);
            greenValue = gamma.GetDirTableValue(greenValue);
            blueValue = gamma.GetDirTableValue(blueValue);
        }

        template <class GammaLUT>
        GRAPHIC_GEOMETRY_INLINE void ApplyGammaInv(const GammaLUT& gamma)
        {
            redValue = gamma.GetInverseTableValue(redValue);
            greenValue = gamma.GetInverseTableValue(greenValue);
            blueValue = gamma.GetInverseTableValue(blueValue);
        }

        static SelfType FromWavelength(double waveLength, double gamma = 1)
        {
            return SelfType(Rgba::FromWavelength(waveLength, gamma));
        }

        static SelfType NoColor()
        {
            return SelfType(0, 0, 0, 0);
        }
    };
} // namespace OHOS

#endif
