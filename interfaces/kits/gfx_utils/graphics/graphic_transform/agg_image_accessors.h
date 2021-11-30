//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------

#ifndef AGG_IMAGE_ACCESSORS_INCLUDED
#define AGG_IMAGE_ACCESSORS_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"

namespace agg
{

    //----------------------------------------------------ImageAccessorClone
    template<class PixelFormat> class ImageAccessorClone
    {
    public:
        typedef PixelFormat   PixelFormatType;
        typedef typename PixelFormatType::color_type color_type;
        typedef typename PixelFormatType::order_type order_type;
        typedef typename PixelFormatType::value_type value_type;
        enum pix_width_e { pix_width = PixelFormatType::pix_width };

        ImageAccessorClone() {}
        explicit ImageAccessorClone(PixelFormatType& pixf) : 
            m_pixf(&pixf) 
        {}

        void Attach(PixelFormatType& pixf)
        {
            m_pixf = &pixf;
        }

    private:
        AGG_INLINE const int8u* Pixel() const
        {
            int x = m_x;
            int y = m_y;
            if(x < 0) x = 0;
            if(y < 0) y = 0;

            if (x >= (int)m_pixf->width()) {
                x = m_pixf->width() - 1;
            }
            if (y >= (int)m_pixf->height()) {
                y = m_pixf->height() - 1;
            }

            return m_pixf->pix_ptr(x, y);
        }

    public:
        AGG_INLINE const int8u* Span(int x, int y, unsigned len)
        {
            m_x = m_x0 = x;
            m_y = y;
            if(y >= 0 && y < (int)m_pixf->height() &&
               x >= 0 && x+len <= (int)m_pixf->width())
            {
                return m_pix_ptr = m_pixf->pix_ptr(x, y);
            }
            m_pix_ptr = 0;
            return Pixel();
        }

        AGG_INLINE const int8u* NextX()
        {
            if(m_pix_ptr) return m_pix_ptr += pix_width;
            ++m_x;
            return Pixel();
        }

        AGG_INLINE const int8u* NextY()
        {
            ++m_y;
            m_x = m_x0;
            if(m_pix_ptr && 
               m_y >= 0 && m_y < (int)m_pixf->height())
            {
                return m_pix_ptr = m_pixf->pix_ptr(m_x, m_y);
            }
            m_pix_ptr = 0;
            return Pixel();
        }

    private:
        const PixelFormatType* m_pixf;
        int                m_x, m_x0, m_y;
        const int8u*       m_pix_ptr;
    };



//----------------------------------------------------ImageAccessorClone
template<class PixelFormat> class image_accessor_norepeat
{
public:
    typedef PixelFormat   PixelFormatType;
    typedef typename PixelFormatType::color_type color_type;
    typedef typename PixelFormatType::order_type order_type;
    typedef typename PixelFormatType::value_type value_type;
    enum pix_width_e { pix_width = PixelFormatType::pix_width };

    image_accessor_norepeat() {}
    explicit image_accessor_norepeat(PixelFormatType& pixf) :
        m_pixf(&pixf)
    {}

    void Attach(PixelFormatType& pixf)
    {
        m_pixf = &pixf;
    }

private:
    AGG_INLINE const int8u* Pixel() const
    {
        int x = m_x;
        int y = m_y;
        if(x < 0) x = 0;
        if(y < 0) y = 0;

        if (x >= (int)m_pixf->width()) {
            x = m_pixf->width() - 1;
            return NULL;
        }
        if (y >= (int)m_pixf->height()) {
            y = m_pixf->height() - 1;
            return NULL;
        }

        return m_pixf->pix_ptr(x, y);
    }

public:
    AGG_INLINE const int8u* Span(int x, int y, unsigned len)
    {
        m_x = m_x0 = x;
        m_y = y;
        if(y >= 0 && y < (int)m_pixf->height() &&
           x >= 0 && x+len <= (int)m_pixf->width())
        {
            return m_pix_ptr = m_pixf->pix_ptr(x, y);
        }
        m_pix_ptr = 0;
        return Pixel();
    }

    AGG_INLINE const int8u* NextX()
    {
        if(m_pix_ptr) return m_pix_ptr += pix_width;
        ++m_x;
        return Pixel();
    }

    AGG_INLINE const int8u* NextY()
    {
        ++m_y;
        m_x = m_x0;
        if(m_pix_ptr &&
           m_y >= 0 && m_y < (int)m_pixf->height())
        {
            return m_pix_ptr = m_pixf->pix_ptr(m_x, m_y);
        }
        m_pix_ptr = 0;
        return Pixel();
    }

private:
    const PixelFormatType* m_pixf;
    int                m_x, m_x0, m_y;
    const int8u*       m_pix_ptr;
};



    //-----------------------------------------------------image_accessor_wrap
    template<class PixelFormat, class WrapX, class WrapY> class image_accessor_wrap
    {
    public:
        typedef PixelFormat   PixelFormatType;
        typedef typename PixelFormatType::color_type color_type;
        typedef typename PixelFormatType::order_type order_type;
        typedef typename PixelFormatType::value_type value_type;
        enum pix_width_e { pix_width = PixelFormatType::pix_width };

        image_accessor_wrap() {}
        explicit image_accessor_wrap(PixelFormatType& pixf) : 
            m_pixf(&pixf), 
            m_wrap_x(pixf.width()), 
            m_wrap_y(pixf.height())
        {}

        void Attach(PixelFormatType& pixf)
        {
            m_pixf = &pixf;
        }

        AGG_INLINE const int8u* Span(int x, int y, unsigned)
        {
            m_x = x;
            m_row_ptr = m_pixf->pix_ptr(0, m_wrap_y(y));
            return m_row_ptr + m_wrap_x(x) * pix_width;
        }

        AGG_INLINE const int8u* NextX()
        {
            int x = ++m_wrap_x;
            return m_row_ptr + x * pix_width;
        }

        AGG_INLINE const int8u* NextY()
        {
            m_row_ptr = m_pixf->pix_ptr(0, ++m_wrap_y);
            return m_row_ptr + m_wrap_x(m_x) * pix_width;
        }

    private:
        const PixelFormatType* m_pixf;
        const int8u*       m_row_ptr;
        int                m_x;
        WrapX              m_wrap_x;
        WrapY              m_wrap_y;
    };




//----------------------------------------------------image_accessor_repeat_x
template<class PixelFormat, class WrapX> class image_accessor_repeat_x
{
public:
    typedef PixelFormat   PixelFormatType;
    typedef typename PixelFormatType::color_type color_type;
    typedef typename PixelFormatType::order_type order_type;
    typedef typename PixelFormatType::value_type value_type;
    enum pix_width_e { pix_width = PixelFormatType::pix_width };

    image_accessor_repeat_x() {}
    explicit image_accessor_repeat_x(PixelFormatType& pixf) :
        m_pixf(&pixf),
        m_wrap_x(pixf.width())
    {}

    void Attach(PixelFormatType& pixf)
    {
        m_pixf = &pixf;
    }

    AGG_INLINE const int8u* Span(int x, int y, unsigned len)
    {
        m_x = x;
        m_y = y;
        if (y >= (int)m_pixf->height()){
            y = m_pixf->height() - 1;
            return NULL;
        }
        m_row_ptr = m_pixf->pix_ptr(0, y);
        return m_row_ptr + m_wrap_x(x) * pix_width;
    }

    AGG_INLINE const int8u* NextX()
    {
        if (m_y >= (int)m_pixf->height()) {
           return NULL;
       }
        int x = ++m_wrap_x;
        return m_row_ptr + x * pix_width;
    }

    AGG_INLINE const int8u* NextY()
    {
        m_row_ptr = m_pixf->pix_ptr(0, m_y);
        return m_row_ptr + m_wrap_x(m_x) * pix_width;

    }

private:
    const PixelFormatType* m_pixf;
    const int8u* m_row_ptr;
    int                m_x;
    int                m_y;
    WrapX              m_wrap_x;
};

////-----------------------------------------------------image_accessor_wrap_y
template<class PixelFormat, class WrapY> class image_accessor_repeat_y
{
public:
    typedef PixelFormat   PixelFormatType;
    typedef typename PixelFormatType::color_type color_type;
    typedef typename PixelFormatType::order_type order_type;
    typedef typename PixelFormatType::value_type value_type;
    enum pix_width_e { pix_width = PixelFormatType::pix_width };

    image_accessor_repeat_y() {}
    explicit image_accessor_repeat_y(PixelFormatType& pixf) :
        m_pixf(&pixf),
        m_wrap_y(pixf.height())
    {}

    void Attach(PixelFormatType& pixf)
    {
        m_pixf = &pixf;
    }

    AGG_INLINE const int8u* Span(int x, int y, unsigned)
    {
        m_x = x;
        if (x >= (int)m_pixf->width()){
            x = m_pixf->width() - 1;
        return NULL;
        }
        m_row_ptr = m_pixf->pix_ptr(0, m_wrap_y(y));
        return m_row_ptr + x * pix_width;
    }

    AGG_INLINE const int8u* NextX()
    {
        int x = ++m_x;
        if (x >= (int)m_pixf->width()) {
            x = m_pixf->width() - 1;
            return NULL;
        }
        return m_row_ptr + x * pix_width;
    }

    AGG_INLINE const int8u* NextY()
    {
        m_row_ptr = m_pixf->pix_ptr(0, ++m_wrap_y);
        return m_row_ptr + m_x * pix_width;
    }

private:
    const PixelFormatType* m_pixf;
    const int8u* m_row_ptr;
    int                m_x;
    WrapY              m_wrap_y;
};


    //--------------------------------------------------------wrap_mode_repeat
    class wrap_mode_repeat
    {
    public:
        wrap_mode_repeat() {}
        wrap_mode_repeat(unsigned size) : 
            m_size(size), 
            m_add(size * (0x3FFFFFFF / size)),
            m_value(0)
        {}

        AGG_INLINE unsigned operator() (int v)
        { 
            return m_value = (unsigned(v) + m_add) % m_size; 
        }

        AGG_INLINE unsigned operator++ ()
        {
            ++m_value;
            if(m_value >= m_size) m_value = 0;
            return m_value;
        }
    private:
        unsigned m_size;
        unsigned m_add;
        unsigned m_value;
    };


}


#endif
