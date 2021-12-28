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
#ifndef GRAPHIC_IMAGE_ACCESSORS_INCLUDED
#define GRAPHIC_IMAGE_ACCESSORS_INCLUDED

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"

namespace OHOS {
    /**
     * @brief 带拷贝的图像存取器
     *
     * @since 1.0
     * @version 1.0
     */
    template<class PixelFormatType> 
    class ImageAccessorClone {
    public:
        typedef typename PixelFormatType::ColorType color_type;
        typedef typename PixelFormatType::OrderType order_type;
        typedef typename PixelFormatType::ValueType value_type;
        enum PixWidth {
            PIX_WIDTH = PixelFormatType::PIX_WIDTH
        };

        ImageAccessorClone()
        {}
        explicit ImageAccessorClone(PixelFormatType& pixFormat) : pixFormat_(&pixFormat)
        {}
        /**
         * @brief 关联像素集
         * @param pixFormat 需要关联的像素集
         * @since 1.0
         * @version 1.0
         */
        void Attach(PixelFormatType& pixFormat)
        {
            pixFormat_ = &pixFormat;
        }

        /**
         * @brief 获取像素地址
         * @param x x轴坐标
         * @param y y轴坐标
         * @param len 线段长度
         * @return 坐标像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* Span(int x, int y, unsigned len)
        {
            xPixel_ = xSpanPixel_ = x;
            yPixel_ = y;
            if (x >= 0 && x+len <= (int)pixFormat_->Width() &&
                    y >= 0 && y < (int)pixFormat_->Height()) {
                return pixPtr_ = pixFormat_->PixPtr(x, y);
            }
            pixPtr_ = 0;
            return Pixel();
        }
        /**
         * @brief 像素地址增加一个像素宽度
         * @return 坐标为改变后的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* NextX()
        {
            if (pixPtr_) {
                return pixPtr_ += PIX_WIDTH;
            }
            ++xPixel_;
            return Pixel();
        }
        /**
         * @brief 像素地址增加一个像素高度
         * @return 坐标为改变后的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* Nexty()
        {
            ++yPixel_;
            xPixel_ = xSpanPixel_;
            if ( pixPtr_ != 0 &&
                 yPixel_ >= 0 && yPixel_ < (int)pixFormat_->Height()) {
                return pixPtr_ = pixFormat_->PixPtr(xPixel_, yPixel_);
            }
            pixPtr_ = 0;
            return Pixel();
        }

    private:

        /**
         * @brief 获取坐标为(xPixel_,yPixel_)的像素地址
         * @return 坐标为(xPixel_,yPixel_)的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* Pixel() const
        {
            int x = xPixel_;
            int y = yPixel_;
            if (x < 0) {
                x = 0;
            }
            if (x >= (int)pixFormat_->Width()) {
                x = pixFormat_->Width() - 1;
            }
            if (y < 0) {
                y = 0;
            }
            if (y >= (int)pixFormat_->Height()) {
                y = pixFormat_->Height() - 1;
            }
            return pixFormat_->PixPtr(x, y);
        }
        const PixelFormatType* pixFormat_;
        int xPixel_, xSpanPixel_, yPixel_;
        const int8u* pixPtr_;
    };


    //-----------------------------------------------------ImageAccessorClip
    template<class PixFmt> class ImageAccessorClip
    {
    public:
        typedef PixFmt   PixelFormatType;
        typedef typename PixelFormatType::ColorType color_type;
        typedef typename PixelFormatType::OrderType order_type;
        typedef typename PixelFormatType::ValueType value_type;
        enum pix_width_e { pix_width = PixelFormatType::PIX_WIDTH };

        ImageAccessorClip() {}
        explicit ImageAccessorClip(PixelFormatType& pixf,const color_type& bk) :
            pixFormat_(&pixf)
        {
            PixelFormatType::MakePix(m_bk_buf, bk);
        }

        void attach(PixelFormatType& pixf)
        {
            pixFormat_ = &pixf;
        }

        void background_color(const color_type& bk)
        {
            PixelFormatType::MakePix(m_bk_buf, bk);
        }

    private:
        GRAPHIC_GEOMETRY_INLINE const int8u* pixel() const
        {
            if (m_y >= 0 && m_y < (int)pixFormat_->Height() &&
                m_x >= 0 && m_x < (int)pixFormat_->Width())
            {
                return pixFormat_->PixPtr(m_x, m_y);
            }
            return m_bk_buf;
        }

    public:
        GRAPHIC_GEOMETRY_INLINE const int8u* Span(int x, int y, unsigned len)
        {
            m_x = m_x0 = x;
            m_y = y;
            if (y >= 0 && y < (int)pixFormat_->Height() &&
                x >= 0 && x + (int)len <= (int)pixFormat_->Width())
            {
                return pixPtr_ = pixFormat_->PixPtr(x, y);
            }
            pixPtr_ = 0;
            return pixel();
        }

        GRAPHIC_GEOMETRY_INLINE const int8u* NextX()
        {
            if (pixPtr_) return pixPtr_ += pix_width;
            ++m_x;
            return pixel();
        }

        GRAPHIC_GEOMETRY_INLINE const int8u* Nexty()
        {
            ++m_y;
            m_x = m_x0;
            if (pixPtr_ &&
                m_y >= 0 && m_y < (int)pixFormat_->Height())
            {
                return pixPtr_ = pixFormat_->PixPtr(m_x, m_y);
            }
            pixPtr_ = 0;
            return pixel();
        }

    private:
        const PixelFormatType* pixFormat_;
        int8u              m_bk_buf[pix_width];
        int                m_x, m_x0, m_y;
        const int8u* pixPtr_;
    };



    /**
     * @brief 图像存取器
     * @since 1.0
     * @version 1.0
     */
    template<class PixelFormatType> class ImageAccessorNoRepeat {
    public:
        typedef typename PixelFormatType::ColorType color_type;
        typedef typename PixelFormatType::OrderType order_type;
        typedef typename PixelFormatType::ValueType value_type;
        enum PixWidth {
            PIX_WIDTH = PixelFormatType::PIX_WIDTH
        };

        ImageAccessorNoRepeat()
        {}
        explicit ImageAccessorNoRepeat(PixelFormatType& pixFormat) : pixFormat_(&pixFormat)
        {}
        /**
         * @brief 关联像素集
         * @param pixFormat 需要关联的像素集
         * @since 1.0
         * @version 1.0
         */
        void Attach(PixelFormatType& pixFormat)
        {
            pixFormat_ = &pixFormat;
        }
        /**
         * @brief 获取像素地址
         * @param x x轴坐标
         * @param y y轴坐标
         * @param len 线段长度
         * @return 坐标像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* Span(int x, int y, unsigned len)
        {
            xPixel_ = xSpanPixel_ = x;
            yPixel_ = y;
            if (x >= 0 && x+len <= (int)pixFormat_->Width() &&
            y >= 0 && y < (int)pixFormat_->Height()) {
                return pixPtr_ = pixFormat_->PixPtr(x, y);
            }
            pixPtr_ = 0;
            return Pixel();
        }
        /**
         * @brief 像素地址增加一个像素宽度
         * @return 坐标为改变后的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* NextX()
        {
            if (pixPtr_ != 0) {
                return pixPtr_ += PIX_WIDTH;
            }
            ++xPixel_;
            return Pixel();
        }
        /**
         * @brief 像素地址增加一个像素高度
         * @return 坐标为改变后的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* NextY()
        {
            ++yPixel_;
            xPixel_ = xSpanPixel_;
            if ( pixPtr_ != 0 &&
                 yPixel_ >= 0 && yPixel_ < (int)pixFormat_->GetHeight()) {
                return pixPtr_ = pixFormat_->pixPtr(xPixel_, yPixel_);
            }
            pixPtr_ = 0;
            return Pixel();
        }

    private:
        /**
         * @brief 获取坐标为(xPixel_,yPixel_)的像素地址
         * @return 坐标为(xPixel_,yPixel_)的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* Pixel() const
        {
            int x = xPixel_;
            int y = yPixel_;
            if (x < 0) {
                x = 0;
            }
            if (x >= (int)pixFormat_->Width()) {
                x = pixFormat_->Width() - 1;
                return NULL;
            }
            if (y < 0) {
                y = 0;
            }
            if (y >= (int)pixFormat_->Height()) {
                y = pixFormat_->Height() - 1;
                return NULL;
            }

            return pixFormat_->PixPtr(x, y);
        }

        const PixelFormatType* pixFormat_;
        int xPixel_, xSpanPixel_, yPixel_;
        const int8u* pixPtr_;
    };

    /**
     * @brief 带换行功能的图像存取器
     *
     * @since 1.0
     * @version 1.0
     */
    template<class PixelFormatType, class WrapX, class WrapY> class ImageAccessorWrap {
    public:
        typedef typename PixelFormatType::ColorType color_type;
        typedef typename PixelFormatType::OrderType order_type;
        typedef typename PixelFormatType::ValueType value_type;
        enum PixWidth {
            PIX_WIDTH = PixelFormatType::PIX_WIDTH 
        };

        ImageAccessorWrap()
        {}
        explicit ImageAccessorWrap(PixelFormatType& pixFormat) : pixFormat_(&pixFormat),
            wrapX_(pixFormat.Width()),
            wrapY_(pixFormat.Height())
        {}

        /**
         * @brief 关联像素集
         * @param pixFormat 需要关联的像素集
         * @since 1.0
         * @version 1.0
         */
        void Attach(PixelFormatType& pixFormat)
        {
            pixFormat_ = &pixFormat;
        }
        /**
         * @brief 获取像素地址带换行
         * @param x x轴坐标
         * @param y y轴坐标
         * @return 坐标像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* Span(int x, int y, unsigned)
        {
            x_ = x;
            rowPtr_ = pixFormat_->PixPtr(0, wrapY_(y));
            return rowPtr_ + wrapX_(x) * PIX_WIDTH;
        }
        /**
         * @brief 像素地址增加一个像素宽度附带换行功能
         * @return 坐标为改变后的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* NextX()
        {
            int x = ++wrapX_;
            return rowPtr_ + x * PIX_WIDTH;
        }

        /**
         * @brief 像素地址增加一个像素高度附带换行功能
         * @return 坐标为改变后的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* NextY()
        {
            rowPtr_ = pixFormat_->pixPtr(0, ++wrapY_);
            return rowPtr_ + wrapX_(x_) * PIX_WIDTH;
        }

    private:
        const PixelFormatType* pixFormat_;
        const int8u* rowPtr_;
        int x_;
        WrapX wrapX_;
        WrapY wrapY_;
    };


    /**
     * @brief 图像在x轴重复的图像存取器
     *
     * @since 1.0
     * @version 1.0
     */
    template<class PixelFormatType, class WrapX> class ImageAccessorRepeatX {
    public:
        typedef typename PixelFormatType::ColorType color_type;
        typedef typename PixelFormatType::OrderType order_type;
        typedef typename PixelFormatType::ValueType value_type;
        enum PixWidth {
            PIX_WIDTH = PixelFormatType::PIX_WIDTH 
        };

        ImageAccessorRepeatX()
        {}
        explicit ImageAccessorRepeatX(PixelFormatType& pixFormat) : pixFormat_(&pixFormat),
            wrapX_(pixFormat.Width())
        {}
        /**
         * @brief 关联像素集
         * @param pixFormat 需要关联的像素集
         * @since 1.0
         * @version 1.0
         */
        void Attach(PixelFormatType& pixFormat)
        {
            pixFormat_ = &pixFormat;
        }
        /**
         * @brief 获取像素地址
         * @param x x轴坐标
         * @param y y轴坐标
         * @param len 线段长度
         * @return 坐标像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* Span(int x, int y, unsigned len)
        {
            xPixel_ = x;
            yPixel_ = y;
            if (y >= (int)pixFormat_->Height()) {
                y = pixFormat_->Height() - 1;
                return NULL;
            }
            rowPtr_ = pixFormat_->PixPtr(0, y);
            return rowPtr_ + wrapX_(x) * PIX_WIDTH;
        }
        /**
         * @brief 像素地址增加一个像素宽度
         * @return 坐标为改变后的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* NextX()
        {
            if (yPixel_ >= (int)pixFormat_->Height()) {
                return NULL;
            }
            int x = ++wrapX_;
            return rowPtr_ + x * PIX_WIDTH;
        }
        /**
         * @brief 像素地址增加一个像素高度
         * @return 坐标为改变后的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* NextY()
        {
            rowPtr_ = pixFormat_->pixPtr(0, yPixel_);
            return rowPtr_ + wrapX_(xPixel_) * PIX_WIDTH;
        }

    private:
        const PixelFormatType* pixFormat_;
        const int8u* rowPtr_;
        int xPixel_;
        int yPixel_;
        WrapX wrapX_;
    };
    /**
     * @brief 图像在y轴重复的图像存取器
     *
     * @since 1.0
     * @version 1.0
     */
    template<class PixelFormatType, class WrapY> class ImageAccessorRepeatY {
    public:
        typedef typename PixelFormatType::ColorType color_type;
        typedef typename PixelFormatType::OrderType order_type;
        typedef typename PixelFormatType::ValueType value_type;
        enum PixWidth
        {
            PIX_WIDTH = PixelFormatType::PIX_WIDTH
        };

        ImageAccessorRepeatY()
        {}
        explicit ImageAccessorRepeatY(PixelFormatType& pixFormat) : pixFormat_(&pixFormat),
            wrapY_(pixFormat.Height())
        {}
        /**
         * @brief 关联像素集
         * @param pixFormat 需要关联的像素集
         * @since 1.0
         * @version 1.0
         */
        void Attach(PixelFormatType& pixFormat)
        {
            pixFormat_ = &pixFormat;
        }
        /**
         * @brief 获取像素地址
         * @param x x轴坐标
         * @param y y轴坐标
         * @return 坐标像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* Span(int x, int y, unsigned)
        {
            xPixel_ = x;
            if (x >= (int)pixFormat_->Width()) {
                x = pixFormat_->Width() - 1;
                return NULL;
            }
            rowPtr_ = pixFormat_->PixPtr(0, wrapY_(y));
            return rowPtr_ + x * PIX_WIDTH;
        }
        /**
         * @brief 像素地址增加一个像素宽度
         * @return 坐标为改变后的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* NextX()
        {
            int x = ++xPixel_;
            if (x >= (int)pixFormat_->Width()) {
                x = pixFormat_->Width() - 1;
                return NULL;
            }
            return rowPtr_ + x * PIX_WIDTH;
        }
        /**
         * @brief 像素地址增加一个像素高度
         * @return 坐标为改变后的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPHIC_GEOMETRY_INLINE const int8u* Nexty()
        {
            rowPtr_ = pixFormat_->pixPtr(0, ++wrapY_);
            return rowPtr_ + xPixel_ * PIX_WIDTH;
        }

    private:
        const PixelFormatType* pixFormat_;
        const int8u* rowPtr_;
        int xPixel_;
        WrapY wrapY_;
    };

    class WrapModeRepeat {
    public:
        WrapModeRepeat()
        {}
        WrapModeRepeat(unsigned size)
            : size_(size),
            add_(size * (0x3FFFFFFF / size)),
            value_(0)
        {}

        GRAPHIC_GEOMETRY_INLINE unsigned operator()(int v)
        {
            return value_ = (unsigned(v) + add_) % size_;
        }

        GRAPHIC_GEOMETRY_INLINE unsigned operator++()
        {
            ++value_;
            if (value_ >= size_) {
                value_ = 0;
            }
            return value_;
        }

    private:
        unsigned size_;
        unsigned add_;
        unsigned value_;
    };
} // namespace OHOS

#endif
