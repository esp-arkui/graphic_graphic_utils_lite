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
 * @addtogroup UI_Utils
 * @{
 *
 * @brief Defines basic UI utils.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file color.h
 *
 * @brief Defines color attributes for the graphics system and implements common color functions.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_LITE_COLOR_H
#define GRAPHIC_LITE_COLOR_H

#include <cmath>
#include <cstdint>

#include "gfx_utils/graphics/common/graphic_common_gamma_lut.h"
#include "gfx_utils/heap_base.h"
#include "graphic_config.h"

#if ENABLE_ARM_MATH
#    include "arm_math.h"
#endif
namespace OHOS {
    const float PURPLE_MIN = 380.0f;
    const float PURPLE_MIDDLE = 420.0f;
    const float PURPLE_MAX = 440.0f;
    const float BLUE_MAX = 490.0f;
    const float CYAN_MAX = 510.0f;
    const float GREEN_MAX = 580.0f;
    const float ORANGE_MAX = 645.0f;
    const float RED_MIN = 700.0f;
    const float RED_MAX = 780.0f;
    const float FIXED_VALUE = 0.3f;
    const float COLOR_CONVERT = 255.0f;
    template <class Colorspace>
    struct Rgba8T;
    struct Linear {
    };

    struct StandardRgb {
    };
    using OpacityType = uint8_t;
    using Rgba8 = Rgba8T<Linear>;
    using Srgba8 = Rgba8T<StandardRgb>;
    /**
     * @brief Enumerates opacity values.
     */
    enum {
        /** The opacity is 0. */
        OPA_TRANSPARENT = 0,
        /** The opacity is 100%. */
        OPA_OPAQUE = 255,
    };

    /**
     * @brief Defines the color attribute when the color depth is <b>16</b>.
     */
    typedef union {
        struct {
            /** Blue */
            uint16_t blue : 5;
            /** Green */
            uint16_t green : 6;
            /** Red */
            uint16_t red : 5;
        };
        /** Full RGB data */
        uint16_t full;
    } Color16;

    /**
     * @brief Defines the color attribute when the color depth is <b>24</b>.
     */
    struct Color24 {
        /** Blue */
        uint8_t blue;
        /** Green */
        uint8_t green;
        /** Red */
        uint8_t red;
    };

    /**
     * @brief Defines the color attribute when the color depth is <b>32</b>.
     */
    typedef union {
        struct {
            /** Blue */
            uint8_t blue;
            /** Green */
            uint8_t green;
            /** Red */
            uint8_t red;
            /** Alpha (how opaque each pixel is) */
            uint8_t alpha;
        };
        /** Full RGB data */
        uint32_t full;
    } Color32;

#if COLOR_DEPTH == 16
    typedef Color16 ColorType;
#elif COLOR_DEPTH == 32
    typedef Color32 ColorType;
#else
#    error "Invalid COLOR_DEPTH, Set it to 16 or 32!"
#endif

    struct OrderRgb {
        enum RgbEnum {
            RED = 0,
            GREEN = 1,
            BLUE = 2
        };
    };

    struct OrderBgr {
        enum BgrEnum {
            BLUE = 0,
            GREEN = 1,
            RED = 2
        };
    };

    struct OrderRgba {
        enum RgbaEnum {
            RED = 0,
            GREEN = 1,
            BLUE = 2,
            ALPHA = 3
        };
    };

    struct OrderArgb {
        enum ArgbEnum {
            ALPHA = 0,
            RED = 1,
            GREEN = 2,
            BLUE = 3
        };
    };
    struct OrderAbgr {
        enum AbgrEnum {
            ALPHA = 0,
            BLUE = 1,
            GREEN = 2,
            RED = 3
        };
    };
    struct OrderBgra {
        enum BgraEnum {
            BLUE = 0,
            GREEN = 1,
            RED = 2,
            ALPHA = 3
        };
    };

    /**
     * @brief Rgba
     *
     * 颜色排列顺序：红、绿、蓝、透明度
     * 注意这个里面的颜色支持浮点处理的
     * @see Rgba
     * @since 1.0
     * @version 1.0
     */
    struct Rgba {
        using uint8_t = float;

        float redValue;
        float greenValue;
        float blueValue;
        float alphaValue;

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
        Rgba(float red, float green, float blue, float alpha = 1.0) :
            redValue(red), greenValue(green), blueValue(blue), alphaValue(alpha)
        {}

        /**
         * @brief Rgba构造函数
         *
         * @param color是Rgba对象、alpha透明度
         * @return 无
         * @since 1.0
         * @version 1.0.
         */
        Rgba(const Rgba& color, float alpha)
            : redValue(color.redValue),
              greenValue(color.greenValue),
              blueValue(color.blueValue),
              alphaValue(alpha)
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
        Rgba& Opacity(float alpha)
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
        float Opacity() const
        {
            return alphaValue;
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
                float alpha = 1.0 / alphaValue;
                redValue *= alpha;
                greenValue *= alpha;
                blueValue *= alpha;
            }
            return *this;
        }
#if GRAPHIC_GEOMETYR_ENABLE_GRADIENT_FILLSTROKECOLOR
        /**
         * @brief 渐变
         *
         * @param rgba为Rgba对象，k为变化系数
         * @return 返回Rgba对象
         * @since 1.0
         * @version 1.0
         */
        Rgba Gradient(Rgba rgba, float k) const
        {
            Rgba ret;
            ret.redValue = redValue + (rgba.redValue - redValue) * k;
            ret.greenValue = greenValue + (rgba.greenValue - greenValue) * k;
            ret.blueValue = blueValue + (rgba.blueValue - blueValue) * k;
            ret.alphaValue = alphaValue + (rgba.alphaValue - alphaValue) * k;
            return ret;
        }
#endif
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
        Rgba& operator*=(float multiplyValue)
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
         * @brief 波长是否是紫色波段
         *
         * @param waveLength 波长
         * @return 返回Rgba对象
         * @since 1.0
         * @version 1.0
         */
        inline static Rgba IsPurpleWave(float waveLength)
        {
            if (waveLength >= PURPLE_MIN && waveLength <= PURPLE_MAX) {
                return Rgba(-1.0 * (waveLength - PURPLE_MAX) / (PURPLE_MAX - PURPLE_MIN), 0.0, 1.0);
            } else {
                return Rgba(0.0, 0.0, 0.0);
            }
        }
        /**
         * @brief 波长是否是蓝色波段
         *
         * @param waveLength 波长
         * @return 返回Rgba对象
         * @since 1.0
         * @version 1.0
         */
        inline static Rgba IsBlueWave(float waveLength)
        {
            if (waveLength >= PURPLE_MAX && waveLength <= BLUE_MAX) {
                return Rgba(0.0, (waveLength - PURPLE_MAX) / (BLUE_MAX - PURPLE_MAX), 1.0);
            } else {
                return Rgba(0.0, 0.0, 0.0);
            }
        }
        /**
         * @brief 波长是否是青色波段
         *
         * @param waveLength 波长
         * @return 返回Rgba对象
         * @since 1.0
         * @version 1.0
         */
        inline static Rgba IsCyanWave(float waveLength)
        {
            if (waveLength >= BLUE_MAX && waveLength <= CYAN_MAX) {
                return Rgba(0.0, 1.0, -1.0 * (waveLength - CYAN_MAX) / (CYAN_MAX - BLUE_MAX));
            } else {
                return Rgba(0.0, 0.0, 0.0);
            }
        }
        /**
         * @brief 波长是否是绿色波段
         *
         * @param waveLength 波长
         * @return 返回Rgba对象
         * @since 1.0
         * @version 1.0
         */
        inline static Rgba IsGreenWave(float waveLength)
        {
            if (waveLength >= CYAN_MAX && waveLength <= GREEN_MAX) {
                return Rgba((waveLength - CYAN_MAX) / (GREEN_MAX - CYAN_MAX), 1.0, 0.0);
            } else {
                return Rgba(0.0, 0.0, 0.0);
            }
        }
        /**
         * @brief 波长是否是橙色波段
         *
         * @param waveLength 波长
         * @return 返回Rgba对象
         * @since 1.0
         * @version 1.0
         */
        inline static Rgba IsOrangeWave(float waveLength)
        {
            if (waveLength >= GREEN_MAX && waveLength <= ORANGE_MAX) {
                return Rgba(1.0, -1.0 * (waveLength - ORANGE_MAX) / (ORANGE_MAX - GREEN_MAX), 0.0);
            } else {
                return Rgba(0.0, 0.0, 0.0);
            }
        }
        /**
         * @brief 波长是否是红色波段
         *
         * @param waveLength 波长
         * @return 返回Rgba对象
         * @since 1.0
         * @version 1.0
         */
        inline static Rgba IsRedWave(float waveLength)
        {
            if (waveLength >= ORANGE_MAX && waveLength <= RED_MAX) {
                return Rgba(1.0, 0.0, 0.0);
            } else {
                return Rgba(0.0, 0.0, 0.0);
            }
        }
        /**
         * @brief 根据波长初始化颜色
         *
         * @param waveLength波长
         * @return 返回Rgba对象
         * @since 1.0
         * @version 1.0
         */
        static Rgba InitColorByWaveLength(float waveLength);
        /**
         * @brief 根据波长以及gamma值计算颜色值
         *
         * @param waveLength波长，gamma伽马值
         * @return 返回Rgba对象
         * @since 1.0
         * @version 1.0
         */
        static Rgba FromWavelength(float waveLength, float gamma = 1.0);

        explicit Rgba(float wavelen, float gamma = 1.0)
        {
            *this = FromWavelength(wavelen, gamma);
        }
    };

    inline Rgba Rgba::InitColorByWaveLength(float waveLength)
    {
        Rgba rgba(0.0, 0.0, 0.0);
        rgba += IsPurpleWave(waveLength);
        rgba += IsBlueWave(waveLength);
        rgba += IsCyanWave(waveLength);
        rgba += IsGreenWave(waveLength);
        rgba += IsOrangeWave(waveLength);
        rgba += IsRedWave(waveLength);
        return rgba;
    }
    inline Rgba Rgba::FromWavelength(float waveLength, float gamma)
    {
        Rgba rgba(0.0, 0.0, 0.0);
        rgba += rgba.InitColorByWaveLength(waveLength);
        // 计算比系数
        float ratio = 1.0;
        if (waveLength > RED_MIN) {
            ratio = FIXED_VALUE + COEFFICIENT * (RED_MAX - waveLength) / (RED_MAX - RED_MIN);
        } else if (waveLength < PURPLE_MIDDLE) {
            ratio = FIXED_VALUE + COEFFICIENT * (waveLength - PURPLE_MIN) / (PURPLE_MIDDLE - PURPLE_MIN);
        }
        return Rgba(std::pow(rgba.redValue * ratio, gamma),
                    std::pow(rgba.greenValue * ratio, gamma),
                    std::pow(rgba.blueValue * ratio, gamma));
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
        using SelfType = Rgba8T;

        uint8_t redValue;
        uint8_t greenValue;
        uint8_t blueValue;
        uint8_t alphaValue;

        enum BaseScaleEnum {
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
        Rgba8T(unsigned red, unsigned green, unsigned blue, unsigned alpha = BASEMASK)
            : redValue(uint8_t(red)),
            greenValue(uint8_t(green)),
            blueValue(uint8_t(blue)),
            alphaValue(uint8_t(alpha))
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
        Rgba8T(const SelfType& color, unsigned alpha)
            : redValue(color.redValue),
            greenValue(color.greenValue),
            blueValue(color.blueValue),
            alphaValue(uint8_t(alpha))
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
         * @brief 将Rgba中的颜色值赋值到Rgba8T<Linear>中
         *
         * @param dst为Rgba8T<Linear>对象的引用，src为Rgba对象的常引用
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        static void Convert(Rgba8T<Linear>& dst, const Rgba& src)
        {
            dst.redValue = uint8_t(Uround(src.redValue * BASEMASK));
            dst.greenValue = uint8_t(Uround(src.greenValue * BASEMASK));
            dst.blueValue = uint8_t(Uround(src.blueValue * BASEMASK));
            dst.alphaValue = uint8_t(Uround(src.alphaValue * BASEMASK));
        }

        /**
         * @brief 将Rgba8T<Srgb>中的颜色值赋值到Rgba中
         *
         * @param dst为Rgba对象的引用，src为Rgba8T<Srgb>对象的常引用
         * @return 无
         * @since 1.0
         * @version 1.0.
         */
        static void Convert(Rgba& dst, const Rgba8T<StandardRgb>& src)
        {
            dst.redValue = StandardRgbConv<float>::RgbFromSrgb(src.redValue);
            dst.greenValue = StandardRgbConv<float>::RgbFromSrgb(src.greenValue);
            dst.blueValue = StandardRgbConv<float>::RgbFromSrgb(src.blueValue);
            dst.alphaValue = StandardRgbConv<float>::AlphaFromSrgb(src.alphaValue);
        }

        static GRAPHIC_GEOMETRY_INLINE float ToFloat(uint8_t uint8_t)
        {
            return float(uint8_t) / BASEMASK;
        }

        static GRAPHIC_GEOMETRY_INLINE uint8_t FromFloat(float value)
        {
            return uint8_t(Uround(value * BASEMASK));
        }

        static GRAPHIC_GEOMETRY_INLINE uint8_t EmptyValue()
        {
            return 0;
        }

        GRAPHIC_GEOMETRY_INLINE bool IsTransparent() const
        {
            return alphaValue == 0;
        }

        GRAPHIC_GEOMETRY_INLINE bool IsOpaque() const
        {
            return alphaValue == BASEMASK;
        }

        static GRAPHIC_GEOMETRY_INLINE uint8_t Invert(uint8_t uint8_t)
        {
            return BASEMASK - uint8_t;
        }

        static GRAPHIC_GEOMETRY_INLINE uint8_t Multiply(uint8_t valueA, uint8_t valueB)
        {
#if ENABLE_ARM_MATH
            uint32_t uint32_t = __SMUAD(valueA, valueB) + BASEMSB;
#else
            uint32_t uint32_t = valueA * valueB + BASEMSB;
#endif
            return uint8_t(((uint32_t >> BASESHIFT) + uint32_t) >> BASESHIFT);
        }

        static GRAPHIC_GEOMETRY_INLINE uint8_t DividMultiply(uint8_t valueA, uint8_t valueB)
        {
            if (valueA * valueB == 0) {
                return 0;
            } else if (valueA >= valueB) {
                return BASEMASK;
            } else {
#if ENABLE_ARM_MATH
                return uint8_t(__UDIV(__SMUAD(valueA, BASEMASK) + (valueB >> 1), valueB));
#else
                return uint8_t((valueA * BASEMASK + (valueB >> 1)) / valueB);
#endif
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

        static GRAPHIC_GEOMETRY_INLINE uint8_t MultCover(uint8_t valueA, CoverType coverValue)
        {
            return Multiply(valueA, coverValue);
        }

        static GRAPHIC_GEOMETRY_INLINE CoverType ScaleCover(CoverType coverValue, uint8_t value)
        {
            return Multiply(value, coverValue);
        }

        static GRAPHIC_GEOMETRY_INLINE uint8_t Prelerp(uint8_t valueP, uint8_t valueQ, uint8_t valueA)
        {
            return valueP + valueQ - Multiply(valueP, valueA);
        }

        static GRAPHIC_GEOMETRY_INLINE uint8_t Lerp(uint8_t valueP, uint8_t valueQ, uint8_t valueA)
        {
#if ENABLE_ARM_MATH
            int t = __SMUAD((valueQ - valueP), valueA) + BASEMSB - (valueP > valueQ);
#else
            int t = (valueQ - valueP) * valueA + BASEMSB - (valueP > valueQ);

#endif
            return uint8_t(valueP + (((t >> BASESHIFT) + t) >> BASESHIFT));
        }

        /**
         * @brief 设置透明度
         *
         * @param alpha为透明度
         * @return 返回Rgba8T对象的引用
         * @since 1.0
         * @version 1.0
         */
        SelfType& Opacity(float alpha)
        {
            if (alpha < 0) {
                alphaValue = 0;
            } else if (alpha > 1) {
                alphaValue = 1;
            } else {
                alphaValue = (uint8_t)Uround(alpha * float(BASEMASK));
            }
            return *this;
        }

        float Opacity() const
        {
            return float(alphaValue) / float(BASEMASK);
        }
#if GRAPHIC_GEOMETYR_ENABLE_GRADIENT_FILLSTROKECOLOR
        /**
         * @brief 渐变，根据变化系数计算出新的Rgba8T对象
         *
         * @param color为Rgba8T对象的引用，k为变化系数
         * @return 返回Rgba8T对象
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE SelfType Gradient(const SelfType& color, float k) const
        {
            SelfType ret;
            uint32_t increaseK = Uround(k * BASEMASK);
            ret.redValue = Lerp(redValue, color.redValue, increaseK);
            ret.greenValue = Lerp(greenValue, color.greenValue, increaseK);
            ret.blueValue = Lerp(blueValue, color.blueValue, increaseK);
            ret.alphaValue = Lerp(alphaValue, color.alphaValue, increaseK);
            return ret;
        }
#endif
        static SelfType NoColor()
        {
            return SelfType(0, 0, 0, 0);
        }
    };

    /**
     * @brief Converts colors in different formats and defines common colors.
     *
     * @since 1.0
     * @version 1.0
     */
    class Color : public HeapBase {
    public:
        /**
         * @brief Mixes two colors (color 1 and color 2) based on a specified opacity.
         *
         * @param c1 Indicates color 1.
         * @param c2 Indicates color 2.
         * @param mix Indicates the alpha, that is, how opaque each pixel is.

         * @return Returns the color data after mixing.
         * @since 1.0
         * @version 1.0
         */
        static ColorType GetMixColor(ColorType c1, ColorType c2, uint8_t mix);

        /**
         * @brief Obtains the color based on the RGB color value.
         *
         * @param r8 Indicates the intensity of red.
         * @param g8 Indicates the intensity of green.
         * @param b8 Indicates the intensity of blue.
         *
         * @return Returns the color data generated.
         * @since 1.0
         * @version 1.0
         */
        static ColorType GetColorFromRGB(uint8_t r8, uint8_t g8, uint8_t b8);

        /**
         * @brief Obtains the color based on the RGBA color value.
         *
         * @param r8 Indicates the intensity of red.
         * @param g8 Indicates the intensity of green.
         * @param b8 Indicates the intensity of blue.
         * @param alpha Indicates the alpha, that is, how opaque each pixel is.
         *
         * @return Returns the color data generated.
         * @since 1.0
         * @version 1.0
         */
        static ColorType GetColorFromRGBA(uint8_t r8, uint8_t g8, uint8_t b8, uint8_t alpha);

        /**
         * @brief Converts color data into the RGBA8888 format.
         *
         * The color data definition varies according to the color depth.
         *
         * @param color Indicates the color data, which is defined by {@link ColorType}.
         * @return Returns the RGBA8888 color data.
         * @since 1.0
         * @version 1.0
         */
        static uint32_t ColorTo32(ColorType color);

        /**
         * @brief Converts color data with the 16-bit color depth into the RGBA8888 format.
         *
         * @param color Indicates the color data with the 16-bit color depth, which is defined by {@link Color16}.
         * @param alpha Indicates the alpha, that is, how opaque each pixel is.
         * @return Returns the RGBA8888 color data.
         * @since 1.0
         * @version 1.0
         */
        static uint32_t ColorTo32(Color16 color, uint8_t alpha);

        /**
         * @brief Converts color data from the RGBA8888 format into the RGB565 format.
         *
         * @param color Indicates the color data with the 32-bit color depth, which is defined by {@link Color32}.
         * @return Returns the RGB565 color data.
         * @since 1.0
         * @version 1.0
         */
        static uint16_t ColorTo16(Color32 color);

        /**
         * @brief Obtains the color data of white.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType White();

        /**
         * @brief Obtains the color data of silver.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Silver();

        /**
         * @brief Obtains the color data of gray.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Gray();

        /**
         * @brief Obtains the color data of black.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Black();

        /**
         * @brief Obtains the color data of red.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Red();

        /**
         * @brief Obtains the color data of maroon.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Maroon();

        /**
         * @brief Obtains the color data of yellow.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Yellow();

        /**
         * @brief Obtains the color data of olive.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Olive();

        /**
         * @brief Obtains the color data of lime.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Lime();

        /**
         * @brief Obtains the color data of green.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Green();

        /**
         * @brief Obtains the color data of cyan.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Cyan();

        /**
         * @brief Obtains the color data of aqua.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Aqua();

        /**
         * @brief Obtains the color data of teal.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Teal();

        /**
         * @brief Obtains the color data of blue.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Blue();

        /**
         * @brief Obtains the color data of navy.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Navy();

        /**
         * @brief Obtains the color data of magenta.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Magenta();

        /**
         * @brief Obtains the color data of purple.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Purple();

        /**
         * @brief Obtains the color data of orange.
         *
         * @return Returns the color data.
         * @since 1.0
         * @version 1.0
         */
        static ColorType Orange();
    };
} // namespace OHOS

#endif
