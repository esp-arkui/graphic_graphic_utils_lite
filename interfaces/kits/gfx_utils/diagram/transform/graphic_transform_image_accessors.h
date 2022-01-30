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

#include "gfx_utils/diagram/common/graphic_common_basics.h"

namespace OHOS {
/**
 * @brief Image accessor with copy
 *
 * @since 1.0
 * @version 1.0
 */
template <class PixelFormatType>
class ImageAccessorClone {
public:
    typedef typename PixelFormatType::ColorType color_type;
    typedef typename PixelFormatType::OrderType OrderType;
    enum PixWidth {
        PIX_WIDTH = PixelFormatType::PIX_WIDTH
    };

    ImageAccessorClone() {}
    explicit ImageAccessorClone(PixelFormatType& pixFormat) : pixFormat_(&pixFormat) {}
    /**
     * @brief Associated pixel set
     * @param pixFormat The associated pixel set is required
     * @since 1.0
     * @version 1.0
     */
    void Attach(PixelFormatType& pixFormat)
    {
        pixFormat_ = &pixFormat;
    }

    /**
     * @brief Get pixel address
     * @param x x-coordinate
     * @param y y-coordinate
     * @param len segment length
     * @return Coordinate pixel address
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* Span(int x, int y, unsigned len)
    {
        xPixel_ = xSpanPixel_ = x;
        yPixel_ = y;
        if (x >= 0 && x + len <= (int)pixFormat_->Width() &&
            y >= 0 && y < (int)pixFormat_->Height()) {
            return pixPtr_ = pixFormat_->PixPtr(x, y);
        }
        pixPtr_ = 0;
        return Pixel();
    }
    /**
     * @brief Pixel address increases by one pixel width
     * @return Coordinates are changed pixel addresses
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* NextX()
    {
        if (pixPtr_) {
            return pixPtr_ += PIX_WIDTH;
        }
        ++xPixel_;
        return Pixel();
    }
    /**
     * @brief Pixel address increases by one pixel width
     * @return Coordinates are changed pixel addresses
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* Nexty()
    {
        ++yPixel_;
        xPixel_ = xSpanPixel_;
        if (pixPtr_ != 0 &&
            yPixel_ >= 0 && yPixel_ < (int)pixFormat_->Height()) {
            return pixPtr_ = pixFormat_->PixPtr(xPixel_, yPixel_);
        }
        pixPtr_ = 0;
        return Pixel();
    }

private:
    /**
     * @brief The obtained coordinates are (xPixel_, yPixel_) Pixel address of
     * @return Coordinates are (xPixel_, yPixel_) Pixel address of
     * @since 1.0
     * @version 1.0
     */
    const uint8_t* Pixel() const
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
    const uint8_t* pixPtr_;
};

template <class PixFmt>
class ImageAccessorClip {
public:
    typedef PixFmt PixelFormatType;
    typedef typename PixelFormatType::ColorType color_type;
    typedef typename PixelFormatType::OrderType OrderType;
    typedef typename PixelFormatType::ValueType value_type;
    enum pix_width_e { pix_width = PixelFormatType::PIX_WIDTH };

    ImageAccessorClip()
    {}
    explicit ImageAccessorClip(PixelFormatType& pixf, const color_type& bk)
        : pixFormat_(&pixf)
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

    inline const uint8_t* Span(int x, int y, unsigned len)
    {
        m_x = m_x0 = x;
        m_y = y;
        if (y >= 0 && y < (int)pixFormat_->Height() &&
            x >= 0 && x + (int)len <= (int)pixFormat_->Width()) {
            return pixPtr_ = pixFormat_->PixPtr(x, y);
        }
        pixPtr_ = 0;
        return pixel();
    }

    inline const uint8_t* NextX()
    {
        if (pixPtr_)
            return pixPtr_ += pix_width;
        ++m_x;
        return pixel();
    }

    inline const uint8_t* Nexty()
    {
        ++m_y;
        m_x = m_x0;
        if (pixPtr_ &&
            m_y >= 0 && m_y < (int)pixFormat_->Height()) {
            return pixPtr_ = pixFormat_->PixPtr(m_x, m_y);
        }
        pixPtr_ = 0;
        return pixel();
    }

private:
    inline const uint8_t* pixel() const
    {
        if (m_y >= 0 && m_y < (int)pixFormat_->Height() &&
            m_x >= 0 && m_x < (int)pixFormat_->Width()) {
            return pixFormat_->PixPtr(m_x, m_y);
        }
        return m_bk_buf;
    }

    const PixelFormatType* pixFormat_;
    uint8_t m_bk_buf[pix_width];
    int m_x, m_x0, m_y;
    const uint8_t* pixPtr_;
};

/**
 * @brief Image accessor
 * @since 1.0
 * @version 1.0
 */
template <class PixelFormatType>
class ImageAccessorNoRepeat {
public:
    typedef typename PixelFormatType::ColorType color_type;
    typedef typename PixelFormatType::OrderType OrderType;
    enum PixWidth {
        PIX_WIDTH = PixelFormatType::PIX_WIDTH
    };

    ImageAccessorNoRepeat()
    {}
    explicit ImageAccessorNoRepeat(PixelFormatType& pixFormat)
        : pixFormat_(&pixFormat)
    {}
    /**
     * @brief Associated pixel set
     * @param pixFormat The associated pixel set is required
     * @since 1.0
     * @version 1.0
     */
    void Attach(PixelFormatType& pixFormat)
    {
        pixFormat_ = &pixFormat;
    }
    /**
     * @brief Get pixel address
     * @param x x-coordinate
     * @param y y-coordinate
     * @param len segment length
     * @return Coordinate pixel address
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* Span(int x, int y, unsigned len)
    {
        xPixel_ = xSpanPixel_ = x;
        yPixel_ = y;
        if (x >= 0 && x + len <= (int)pixFormat_->Width() &&
            y >= 0 && y < (int)pixFormat_->Height()) {
            return pixPtr_ = pixFormat_->PixPtr(x, y);
        }
        pixPtr_ = 0;
        return Pixel();
    }
    /**
     * @brief Pixel address increases by one pixel width
     * @return Coordinates are changed pixel addresses
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* NextX()
    {
        if (pixPtr_ != 0) {
            return pixPtr_ += PIX_WIDTH;
        }
        ++xPixel_;
        return Pixel();
    }
    /**
     * @brief Pixel address increases by one pixel width
     * @return Coordinates are changed pixel addresses
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* NextY()
    {
        ++yPixel_;
        xPixel_ = xSpanPixel_;
        if (pixPtr_ != 0 &&
            yPixel_ >= 0 && yPixel_ < (int)pixFormat_->GetHeight()) {
            return pixPtr_ = pixFormat_->pixPtr(xPixel_, yPixel_);
        }
        pixPtr_ = 0;
        return Pixel();
    }

private:
    /**
     * @brief The obtained coordinates are (xPixel_, yPixel_) Pixel address of
     * @return Coordinates are (xPixel_, yPixel_) Pixel address of
     * @since 1.0
     * @version 1.0
     */
    const uint8_t* Pixel() const
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
    const uint8_t* pixPtr_;
};

/**
 * @brief Image accessor with line feed function
 *
 * @since 1.0
 * @version 1.0
 */
template <class PixelFormatType, class WrapX, class WrapY>
class ImageAccessorWrap {
public:
    typedef typename PixelFormatType::ColorType color_type;
    typedef typename PixelFormatType::OrderType OrderType;
    enum PixWidth {
        PIX_WIDTH = PixelFormatType::PIX_WIDTH
    };

    ImageAccessorWrap() {}
    explicit ImageAccessorWrap(PixelFormatType& pixFormat)
        : pixFormat_(&pixFormat),
        wrapX_(pixFormat.Width()),
        wrapY_(pixFormat.Height()) {}

    /**
     * @brief Associated pixel set
     * @param pixFormat The associated pixel set is required
     * @since 1.0
     * @version 1.0
     */
    void Attach(PixelFormatType& pixFormat)
    {
        pixFormat_ = &pixFormat;
    }
    /**
     * @brief Get pixel address with line feed
     * @param x x-coordinate
     * @param y y-coordinate
     * @return Coordinate pixel address
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* Span(int x, int y, unsigned)
    {
        x_ = x;
        rowPtr_ = pixFormat_->PixPtr(0, wrapY_(y));
        return rowPtr_ + wrapX_(x) * PIX_WIDTH;
    }
    /**
     * @brief Add one pixel width to the pixel address, with line feed function
     * @return Add one pixel width to the pixel address, with line feed function
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* NextX()
    {
        int x = ++wrapX_;
        return rowPtr_ + x * PIX_WIDTH;
    }

    /**
     * @brief Add one pixel width to the pixel address, with line feed function
     * @return Add one pixel width to the pixel address, with line feed function
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* NextY()
    {
        rowPtr_ = pixFormat_->pixPtr(0, ++wrapY_);
        return rowPtr_ + wrapX_(x_) * PIX_WIDTH;
    }

private:
    const PixelFormatType* pixFormat_;
    const uint8_t* rowPtr_;
    int x_;
    WrapX wrapX_;
    WrapY wrapY_;
};

/**
 * @brief An image accessor in which images are repeated on the x-axis
 *
 * @since 1.0
 * @version 1.0
 */
template <class PixelFormatType, class WrapX>
class ImageAccessorRepeatX {
public:
    typedef typename PixelFormatType::ColorType color_type;
    typedef typename PixelFormatType::OrderType OrderType;
    enum PixWidth {
        PIX_WIDTH = PixelFormatType::PIX_WIDTH
    };

    ImageAccessorRepeatX() {}
    explicit ImageAccessorRepeatX(PixelFormatType& pixFormat)
        : pixFormat_(&pixFormat),
        wrapX_(pixFormat.Width()) {}
    /**
     * @brief Associated pixel set
     * @param pixFormat The associated pixel set is required
     * @since 1.0
     * @version 1.0
     */
    void Attach(PixelFormatType& pixFormat)
    {
        pixFormat_ = &pixFormat;
    }
    /**
     * @brief Get pixel address
     * @param x x-coordinate
     * @param y y-coordinate
     * @param len segment length
     * @return Coordinate pixel address
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* Span(int x, int y, unsigned len)
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
     * @brief Pixel address increases by one pixel width
     * @return Coordinates are changed pixel addresses
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* NextX()
    {
        if (yPixel_ >= (int)pixFormat_->Height()) {
            return NULL;
        }
        int x = ++wrapX_;
        return rowPtr_ + x * PIX_WIDTH;
    }
    /**
     * @brief Pixel address increases by one pixel width
     * @return Coordinates are changed pixel addresses
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* NextY()
    {
        rowPtr_ = pixFormat_->pixPtr(0, yPixel_);
        return rowPtr_ + wrapX_(xPixel_) * PIX_WIDTH;
    }

private:
    const PixelFormatType* pixFormat_;
    const uint8_t* rowPtr_;
    int xPixel_;
    int yPixel_;
    WrapX wrapX_;
};
/**
 * @brief Image accessor for image repetition on y-axis
 *
 * @since 1.0
 * @version 1.0
 */
template <class PixelFormatType, class WrapY>
class ImageAccessorRepeatY {
public:
    typedef typename PixelFormatType::ColorType color_type;
    typedef typename PixelFormatType::OrderType OrderType;
    enum PixWidth {
        PIX_WIDTH = PixelFormatType::PIX_WIDTH
    };

    ImageAccessorRepeatY() {}
    explicit ImageAccessorRepeatY(PixelFormatType& pixFormat)
        : pixFormat_(&pixFormat), wrapY_(pixFormat.Height()) {}
    /**
     * @brief Associated pixel set
     * @param pixFormat The associated pixel set is required
     * @since 1.0
     * @version 1.0
     */
    void Attach(PixelFormatType& pixFormat)
    {
        pixFormat_ = &pixFormat;
    }
    /**
     * @brief Get pixel address
     * @param x x-coordinate
     * @param y y-coordinate
     * @return Coordinate pixel address
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* Span(int x, int y, unsigned)
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
     * @brief Pixel address increases by one pixel width
     * @return Coordinates are changed pixel addresses
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* NextX()
    {
        int x = ++xPixel_;
        if (x >= (int)pixFormat_->Width()) {
            x = pixFormat_->Width() - 1;
            return NULL;
        }
        return rowPtr_ + x * PIX_WIDTH;
    }
    /**
     * @brief Pixel address increases by one pixel width
     * @return Coordinates are changed pixel addresses
     * @since 1.0
     * @version 1.0
     */
    inline const uint8_t* Nexty()
    {
        rowPtr_ = pixFormat_->pixPtr(0, ++wrapY_);
        return rowPtr_ + xPixel_ * PIX_WIDTH;
    }

private:
    const PixelFormatType* pixFormat_;
    const uint8_t* rowPtr_;
    int xPixel_;
    WrapY wrapY_;
};

class WrapModeRepeat {
public:
    WrapModeRepeat() {}
    WrapModeRepeat(unsigned size)
        : size_(size),
        add_(size == 0 ? 0 : (size * (0x3FFFFFFF / size))),
        value_(0)
    {
    }

    inline unsigned operator()(int v)
    {
        return value_ = (unsigned(v) + add_) % size_;
    }

    inline unsigned operator++()
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
