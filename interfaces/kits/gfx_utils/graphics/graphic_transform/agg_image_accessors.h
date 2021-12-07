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

#include "gfx_utils/graphics/graphic_common/agg_basics.h"

namespace OHOS
{

    template<class PixelFormatType> class ImageAccessorClone
    {
    public:
        typedef typename PixelFormatType::color_type color_type;
        typedef typename PixelFormatType::order_type order_type;
        typedef typename PixelFormatType::value_type value_type;
        enum PixWidth { 
            PIX_WIDTH = PixelFormatType::pix_width 
        };

        ImageAccessorClone() {}
        explicit ImageAccessorClone(PixelFormatType& pixFormat) : 
            pixFormat_(&pixFormat) 
        {}
        /**
         * @brief 关联像素集
         * 
         * @param pixFormat 需要关联的像素集
         * @since 1.0
         * @version 1.0
         */
        void Attach(PixelFormatType& pixFormat)
        {
            pixFormat_ = &pixFormat;
        }

    private:


        /**
         * @brief 获取坐标为(x_,y_)的像素地址
         * 
         * @return 坐标为(x_,y_)的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPTHIC_INLINE const int8u* Pixel() const
        {
            int x = x_;
            int y = y_;
            if(x < 0) {
                x = 0;
            }
            if (x >= (int)pixFormat_->width()) {
                x = pixFormat_->width() - 1;
            }
            if(y < 0) {
                y = 0;
            }
            if (y >= (int)pixFormat_->height()) {
                y = pixFormat_->height() - 1;
            }
            return pixFormat_->pix_ptr(x, y);
        }

    public:
        /**
         * @brief 获取像素地址
         * 
         * @param x x轴坐标
         * @param y y轴坐标
         * @param len 线段长度
         * @return 坐标像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPTHIC_INLINE const int8u* Span(int x, int y, unsigned len)
        {
            x_ = x0_ = x;
            y_ = y;
            if(x >= 0 && x+len <= (int)pixFormat_->width() &&
               y >= 0 && y < (int)pixFormat_->height() ) {
                return pixPtr_ = pixFormat_->pix_ptr(x, y);
            }
            pixPtr_ = 0;
            return Pixel();
        }
        /**
         * @brief 像素地址增加一个像素宽度
         * 
         * @return 坐标为改变后的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPTHIC_INLINE const int8u* NextX()
        {
            if(pixPtr_) {
                return pixPtr_ += PIX_WIDTH;
            }
            ++x_;
            return Pixel();
        }
        /**
         * @brief 像素地址增加一个像素高度
         * 
         * @return 坐标为改变后的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPTHIC_INLINE const int8u* NextY()
        {
            ++y_;
            x_ = x0_;
            if(pixPtr_ != 0 && 
               y_ >= 0 && y_ < (int)pixFormat_->height()){
                return pixPtr_ = pixFormat_->pix_ptr(x_, y_);
            }
            pixPtr_ = 0;
            return Pixel();
        }

    private:
        const PixelFormatType* pixFormat_;
        int                x_, x0_, y_;
        const int8u*       pixPtr_;
    };

template<class PixelFormatType> class ImageAccessorNoRepeat
{
public:
    typedef typename PixelFormatType::color_type color_type;
    typedef typename PixelFormatType::order_type order_type;
    typedef typename PixelFormatType::value_type value_type;
    enum PixWidth { 
        PIX_WIDTH = PixelFormatType::pix_width 
    };

    ImageAccessorNoRepeat() {}
    explicit ImageAccessorNoRepeat(PixelFormatType& pixFormat) :
        pixFormat_(&pixFormat)
    {}
    /**
     * @brief 关联像素集
     * 
     * @param pixFormat 需要关联的像素集
     * @since 1.0
     * @version 1.0
     */
    void Attach(PixelFormatType& pixFormat)
    {
        pixFormat_ = &pixFormat;
    }

private:

    /**
     * @brief 获取坐标为(x_,y_)的像素地址
     * 
     * @return 坐标为(x_,y_)的像素地址
     * @since 1.0
     * @version 1.0
     */
    GRAPTHIC_INLINE const int8u* Pixel() const
    {
        int x = x_;
        int y = y_;
        if(x < 0) {
            x = 0;
        }
        if (x >= (int)pixFormat_->width()) {
            x = pixFormat_->width() - 1;
            return NULL;
        }
        if(y < 0) {
            y = 0;
        }
        if (y >= (int)pixFormat_->height()) {
            y = pixFormat_->height() - 1;
            return NULL;
        }

        return pixFormat_->pix_ptr(x, y);
    }

public:
    /**
     * @brief 获取像素地址
     * 
     * @param x x轴坐标
     * @param y y轴坐标
     * @param len 线段长度
     * @return 坐标像素地址
     * @since 1.0
     * @version 1.0
     */
    GRAPTHIC_INLINE const int8u* Span(int x, int y, unsigned len)
    {
        x_ = x0_ = x;
        y_ = y;
        if(x >= 0 && x+len <= (int)pixFormat_->width() &&
           y >= 0 && y < (int)pixFormat_->height()) {
            return pixPtr_ = pixFormat_->pix_ptr(x, y);
        }
        pixPtr_ = 0;
        return Pixel();
    }
    /**
     * @brief 像素地址增加一个像素宽度
     * 
     * @return 坐标为改变后的像素地址
     * @since 1.0
     * @version 1.0
     */
    GRAPTHIC_INLINE const int8u* NextX()
    {
        if(pixPtr_ != 0) {
            return pixPtr_ += PIX_WIDTH;
        }
        ++x_;
        return Pixel();
    }
    /**
     * @brief 像素地址增加一个像素高度
     * 
     * @return 坐标为改变后的像素地址
     * @since 1.0
     * @version 1.0
     */
    GRAPTHIC_INLINE const int8u* NextY()
    {
        ++y_;
        x_ = x0_;
        if(pixPtr_ != 0 &&
           y_ >= 0 && y_ < (int)pixFormat_->height()) {
            return pixPtr_ = pixFormat_->pixPtr(x_, y_);
        }
        pixPtr_ = 0;
        return Pixel();
    }

private:
    const PixelFormatType* pixFormat_;
    int                x_, x0_, y_;
    const int8u*       pixPtr_;
};


template<class PixelFormat, class WrapX, class WrapY> class ImageAccessorWrap
{
public:
    typedef PixelFormat   PixelFormatType;
    typedef typename PixelFormatType::color_type color_type;
    typedef typename PixelFormatType::order_type order_type;
    typedef typename PixelFormatType::value_type value_type;
    enum PixWidth { 
        PIX_WIDTH = PixelFormatType::pix_width 
    };

    ImageAccessorWrap() {}
    explicit ImageAccessorWrap(PixelFormatType& pixFormat) : 
        pixFormat_(&pixFormat), 
        wrapX_(pixFormat.width()), 
        wrapY_(pixFormat.height())
    {}

    /**
     * @brief 关联像素集
     * 
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
     * 
     * @param x x轴坐标
     * @param y y轴坐标
     * @return 坐标像素地址
     * @since 1.0
     * @version 1.0
     */
    GRAPTHIC_INLINE const int8u* Span(int x, int y, unsigned)
    {
        x_ = x;
        rowPtr_ = pixFormat_->pix_ptr(0, wrapY_(y));
        return rowPtr_ + wrapX_(x) * PIX_WIDTH;
    }
    /**
     * @brief 像素地址增加一个像素宽度附带换行功能
     * 
     * @return 坐标为改变后的像素地址
     * @since 1.0
     * @version 1.0
     */
    GRAPTHIC_INLINE const int8u* NextX()
    {
        int x = ++wrapX_;
        return rowPtr_ + x * PIX_WIDTH;
    }

    /**
     * @brief 像素地址增加一个像素高度附带换行功能
     * 
     * @return 坐标为改变后的像素地址
     * @since 1.0
     * @version 1.0
     */
    GRAPTHIC_INLINE const int8u* NextY()
    {
        rowPtr_ = pixFormat_->pixPtr(0, ++wrapY_);
        return rowPtr_ + wrapX_(x_) * PIX_WIDTH;
    }

private:
    const PixelFormatType* pixFormat_;
    const int8u*       rowPtr_;
    int                x_;
    WrapX              wrapX_;
    WrapY              wrapY_;
};



template<class PixelFormat, class WrapX> class ImageAccessorRepeatX
{
public:
    typedef PixelFormat   PixelFormatType;
    typedef typename PixelFormatType::color_type color_type;
    typedef typename PixelFormatType::order_type order_type;
    typedef typename PixelFormatType::value_type value_type;
    enum PixWidth { 
        PIX_WIDTH = PixelFormatType::pix_width 
    };

    ImageAccessorRepeatX() {}
    explicit ImageAccessorRepeatX(PixelFormatType& pixFormat) :
        pixFormat_(&pixFormat),
        wrapX_(pixFormat.width())
    {}
    /**
     * @brief 关联像素集
     * 
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
     * 
     * @param x x轴坐标
     * @param y y轴坐标
     * @param len 线段长度
     * @return 坐标像素地址
     * @since 1.0
     * @version 1.0
     */
    GRAPTHIC_INLINE const int8u* Span(int x, int y, unsigned len)
    {
        x_ = x;
        y_ = y;
        if (y >= (int)pixFormat_->height()){
            y = pixFormat_->height() - 1;
            return NULL;
        }
        rowPtr_ = pixFormat_->pix_ptr(0, y);
        return rowPtr_ + wrapX_(x) * PIX_WIDTH;
    }
    /**
     * @brief 像素地址增加一个像素宽度
     * 
     * @return 坐标为改变后的像素地址
     * @since 1.0
     * @version 1.0
     */
    GRAPTHIC_INLINE const int8u* NextX()
    {
        if (y_ >= (int)pixFormat_->height()) {
           return NULL;
       }
        int x = ++wrapX_;
        return rowPtr_ + x * PIX_WIDTH;
    }
    /**
     * @brief 像素地址增加一个像素高度
     * 
     * @return 坐标为改变后的像素地址
     * @since 1.0
     * @version 1.0
     */
    GRAPTHIC_INLINE const int8u* NextY()
    {
        rowPtr_ = pixFormat_->pixPtr(0, y_);
        return rowPtr_ + wrapX_(x_) * PIX_WIDTH;

    }

private:
    const PixelFormatType*  pixFormat_;
    const int8u*            rowPtr_;
    int                     x_;
    int                     y_;
    WrapX                   wrapX_;
};

    template<class PixelFormatType, class WrapY> class ImageAccessorRepeatY
    {
    public:
        typedef typename PixelFormatType::color_type color_type;
        typedef typename PixelFormatType::order_type order_type;
        typedef typename PixelFormatType::value_type value_type;
        enum PixWidth { 
            PIX_WIDTH = PixelFormatType::pix_width 
        };

        ImageAccessorRepeatY() {}
        explicit ImageAccessorRepeatY(PixelFormatType& pixFormat) :
            pixFormat_(&pixFormat),
            wrapY_(pixFormat.height())
        {}
        /**
         * @brief 关联像素集
         * 
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
         * 
         * @param x x轴坐标
         * @param y y轴坐标
         * @return 坐标像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPTHIC_INLINE const int8u* Span(int x, int y, unsigned)
        {
            x_ = x;
            if (x >= (int)pixFormat_->width()){
                x = pixFormat_->width() - 1;
            return NULL;
            }
            rowPtr_ = pixFormat_->pix_ptr(0, wrapY_(y));
            return rowPtr_ + x * PIX_WIDTH;
        }
        /**
         * @brief 像素地址增加一个像素宽度
         * 
         * @return 坐标为改变后的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPTHIC_INLINE const int8u* NextX()
        {
            int x = ++x_;
            if (x >= (int)pixFormat_->width()) {
                x = pixFormat_->width() - 1;
                return NULL;
            }
            return rowPtr_ + x * PIX_WIDTH;
        }
        /**
         * @brief 像素地址增加一个像素高度
         * 
         * @return 坐标为改变后的像素地址
         * @since 1.0
         * @version 1.0
         */
        GRAPTHIC_INLINE const int8u* NextY()
        {
            rowPtr_ = pixFormat_->pixPtr(0, ++wrapY_);
            return rowPtr_ + x_ * PIX_WIDTH;
        }

    private:
        const PixelFormatType*  pixFormat_;
        const int8u*            rowPtr_;
        int                     x_;
        WrapY                   wrapY_;
    };


    class WrapModeRepeat
    {
    public:
        WrapModeRepeat() {}
        WrapModeRepeat(unsigned size) : 
            size_(size), 
            add_(size * (0x3FFFFFFF / size)),
            value_(0)
        {}

        GRAPTHIC_INLINE unsigned operator() (int v)
        { 
            return value_ = (unsigned(v) + add_) % size_; 
        }

        GRAPTHIC_INLINE unsigned operator++ ()
        {
            ++value_;
            if(value_ >= size_) {
                value_ = 0;
            }
            return value_;
        }
    private:
        unsigned size_;
        unsigned add_;
        unsigned value_;
    };


}


#endif
