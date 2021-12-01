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

namespace agg
{

    template<class PixelFormat> class ImageAccessorClone
    {
    public:
        typedef PixelFormat   PixelFormatType;
        typedef typename PixelFormatType::colorType colorType;
        typedef typename PixelFormatType::orderType orderType;
        typedef typename PixelFormatType::valueType valueType;
        enum PixWidth { PIXWIDTH = PixelFormatType::pixWidth };

        ImageAccessorClone() {}
        explicit ImageAccessorClone(PixelFormatType& pixFormat) : 
            pixFormat_(&pixFormat) 
        {}

        void Attach(PixelFormatType& pixFormat)
        {
            pixFormat_ = &pixFormat;
        }

    private:
        GRAPTHIC_INLINE const int8u* Pixel() const
        {
            int x = x_;
            int y = y_;
            if(x < 0) {
                x = 0;
            }
            if (x >= (int)pixFormat_->Width()) {
                x = pixFormat_->Width() - 1;
            }
            if(y < 0) {
                y = 0;
            }
            if (y >= (int)pixFormat_->Height()) {
                y = pixFormat_->Height() - 1;
            }
            return pixFormat_->pixPtr(x, y);
        }

    public:
        GRAPTHIC_INLINE const int8u* Span(int x, int y, unsigned len)
        {
            x_ = x0_ = x;
            y_ = y;
            if(x >= 0 && x+len <= (int)pixFormat_->Width() &&
               y >= 0 && y < (int)pixFormat_->Height() ) {
                return pixPtr_ = pixFormat_->pixPtr(x, y);
            }
            pixPtr_ = 0;
            return Pixel();
        }

        GRAPTHIC_INLINE const int8u* NextX()
        {
            if(pixPtr_) {
                return pixPtr_ += PIXWIDTH;
            }
            ++x_;
            return Pixel();
        }

        GRAPTHIC_INLINE const int8u* NextY()
        {
            ++y_;
            x_ = x0_;
            if(pixPtr_ != 0 && 
               y_ >= 0 && y_ < (int)pixFormat_->Height()){
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

template<class PixelFormat> class ImageAccessorNoRepeat
{
public:
    typedef PixelFormat   PixelFormatType;
    typedef typename PixelFormatType::colorType colorType;
    typedef typename PixelFormatType::orderType orderType;
    typedef typename PixelFormatType::valueType valueType;
    enum PixWidth { 
        PIXWIDTH = PixelFormatType::pixWidth 
    };

    ImageAccessorNoRepeat() {}
    explicit ImageAccessorNoRepeat(PixelFormatType& pixFormat) :
        pixFormat_(&pixFormat)
    {}

    void Attach(PixelFormatType& pixFormat)
    {
        pixFormat_ = &pixFormat;
    }

private:
    GRAPTHIC_INLINE const int8u* Pixel() const
    {
        int x = x_;
        int y = y_;
        if(x < 0) {
            x = 0;
        }
        if (x >= (int)pixFormat_->Width()) {
            x = pixFormat_->Width() - 1;
            return NULL;
        }
        if(y < 0) {
            y = 0;
        }
        if (y >= (int)pixFormat_->Height()) {
            y = pixFormat_->Height() - 1;
            return NULL;
        }

        return pixFormat_->pixPtr(x, y);
    }

public:
    GRAPTHIC_INLINE const int8u* Span(int x, int y, unsigned len)
    {
        x_ = x0_ = x;
        y_ = y;
        if(x >= 0 && x+len <= (int)pixFormat_->Width() &&
           y >= 0 && y < (int)pixFormat_->Height()) {
            return pixPtr_ = pixFormat_->pixPtr(x, y);
        }
        pixPtr_ = 0;
        return Pixel();
    }

    GRAPTHIC_INLINE const int8u* NextX()
    {
        if(pixPtr_ != 0) {
            return pixPtr_ += PIXWIDTH;
        }
        ++x_;
        return Pixel();
    }

    GRAPTHIC_INLINE const int8u* NextY()
    {
        ++y_;
        x_ = x0_;
        if(pixPtr_ != 0 &&
           y_ >= 0 && y_ < (int)pixFormat_->Height()) {
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
        typedef typename PixelFormatType::colorType colorType;
        typedef typename PixelFormatType::orderType orderType;
        typedef typename PixelFormatType::valueType valueType;
        enum PixWidth { PIXWIDTH = PixelFormatType::pixWidth };

        ImageAccessorWrap() {}
        explicit ImageAccessorWrap(PixelFormatType& pixFormat) : 
            pixFormat_(&pixFormat), 
            wrapX_(pixFormat.Width()), 
            wrapY_(pixFormat.Height())
        {}

        void Attach(PixelFormatType& pixFormat)
        {
            pixFormat_ = &pixFormat;
        }

        GRAPTHIC_INLINE const int8u* Span(int x, int y, unsigned)
        {
            x_ = x;
            rowPtr_ = pixFormat_->pixPtr(0, wrapY_(y));
            return rowPtr_ + wrapX_(x) * PIXWIDTH;
        }

        GRAPTHIC_INLINE const int8u* NextX()
        {
            int x = ++wrapX_;
            return rowPtr_ + x * PIXWIDTH;
        }

        GRAPTHIC_INLINE const int8u* NextY()
        {
            rowPtr_ = pixFormat_->pixPtr(0, ++wrapY_);
            return rowPtr_ + wrapX_(x_) * PIXWIDTH;
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
    typedef typename PixelFormatType::colorType colorType;
    typedef typename PixelFormatType::orderType orderType;
    typedef typename PixelFormatType::valueType valueType;
    enum PixWidth { PIXWIDTH = PixelFormatType::pixWidth };

    ImageAccessorRepeatX() {}
    explicit ImageAccessorRepeatX(PixelFormatType& pixFormat) :
        pixFormat_(&pixFormat),
        wrapX_(pixFormat.Width())
    {}

    void Attach(PixelFormatType& pixFormat)
    {
        pixFormat_ = &pixFormat;
    }

    GRAPTHIC_INLINE const int8u* Span(int x, int y, unsigned len)
    {
        x_ = x;
        y_ = y;
        if (y >= (int)pixFormat_->Height()){
            y = pixFormat_->Height() - 1;
            return NULL;
        }
        rowPtr_ = pixFormat_->pixPtr(0, y);
        return rowPtr_ + wrapX_(x) * PIXWIDTH;
    }

    GRAPTHIC_INLINE const int8u* NextX()
    {
        if (y_ >= (int)pixFormat_->Height()) {
           return NULL;
       }
        int x = ++wrapX_;
        return rowPtr_ + x * PIXWIDTH;
    }

    GRAPTHIC_INLINE const int8u* NextY()
    {
        rowPtr_ = pixFormat_->pixPtr(0, y_);
        return rowPtr_ + wrapX_(x_) * PIXWIDTH;

    }

private:
    const PixelFormatType*  pixFormat_;
    const int8u*            rowPtr_;
    int                     x_;
    int                     y_;
    WrapX                   wrapX_;
};

template<class PixelFormat, class WrapY> class ImageAccessorRepeatY
{
public:
    typedef PixelFormat   PixelFormatType;
    typedef typename PixelFormatType::colorType colorType;
    typedef typename PixelFormatType::orderType orderType;
    typedef typename PixelFormatType::valueType valueType;
    enum PixWidth { PIXWIDTH = PixelFormatType::pixWidth };

    ImageAccessorRepeatY() {}
    explicit ImageAccessorRepeatY(PixelFormatType& pixFormat) :
        pixFormat_(&pixFormat),
        wrapY_(pixFormat.Height())
    {}

    void Attach(PixelFormatType& pixFormat)
    {
        pixFormat_ = &pixFormat;
    }

    GRAPTHIC_INLINE const int8u* Span(int x, int y, unsigned)
    {
        x_ = x;
        if (x >= (int)pixFormat_->Width()){
            x = pixFormat_->Width() - 1;
        return NULL;
        }
        rowPtr_ = pixFormat_->pixPtr(0, wrapY_(y));
        return rowPtr_ + x * PIXWIDTH;
    }

    GRAPTHIC_INLINE const int8u* NextX()
    {
        int x = ++x_;
        if (x >= (int)pixFormat_->Width()) {
            x = pixFormat_->Width() - 1;
            return NULL;
        }
        return rowPtr_ + x * PIXWIDTH;
    }

    GRAPTHIC_INLINE const int8u* NextY()
    {
        rowPtr_ = pixFormat_->pixPtr(0, ++wrapY_);
        return rowPtr_ + x_ * PIXWIDTH;
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
