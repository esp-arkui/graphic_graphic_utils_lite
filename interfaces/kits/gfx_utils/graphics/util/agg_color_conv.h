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

#ifndef AGG_COLOR_CONV_INCLUDED
#define AGG_COLOR_CONV_INCLUDED

#include <cstring>
#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "render/agg_rendering_buffer.h"


namespace agg
{

    //--------------------------------------------------------------color_conv
    template<class RenBuf, class CopyRow> 
    void color_conv(RenBuf* dst, const RenBuf* src, CopyRow copy_row_functor)
    {
        unsigned width = src->width();
        unsigned height = src->height();

        if(dst->width()  < width)  width  = dst->width();
        if(dst->height() < height) height = dst->height();

        if(width)
        {
            unsigned y;
            for(y = 0; y < height; y++)
            {
                copy_row_functor(dst->row_ptr(0, y, width), 
                                 src->row_ptr(y), 
                                 width);
            }
        }
    }


    //---------------------------------------------------------color_conv_row
    template<class CopyRow> 
    void color_conv_row(int8u* dst, 
                        const int8u* src,
                        unsigned width,
                        CopyRow copy_row_functor)
    {
        copy_row_functor(dst, src, width);
    }


    //---------------------------------------------------------color_conv_same
    template<int BPP> class color_conv_same
    {
    public:
        void operator () (int8u* dst, 
                          const int8u* src,
                          unsigned width) const
        {
            std::memmove(dst, src, width*BPP);
        }
    };


    // Generic pixel converter.
    template<class DstFormat, class SrcFormat>
    struct conv_pixel
    {
        void operator()(void* dst, const void* src) const
        {
            // Read a pixel from the source format and write it to the destination format.
            DstFormat::write_plain_color(dst, SrcFormat::read_plain_color(src));
        }
    };

    // Generic row converter. Uses conv_pixel to convert individual pixels.
    template<class DstFormat, class SrcFormat>
    struct conv_row
    {
        void operator()(void* dst, const void* src, unsigned width) const
        {
            conv_pixel<DstFormat, SrcFormat> conv;
            do
            {
                conv(dst, src);
                dst = (int8u*)dst + DstFormat::pix_width;
                src = (int8u*)src + SrcFormat::pix_width;
            }
            while (--width);
        }
    };

    // Specialization for case where source and destination formats are identical.
    template<class Format>
    struct conv_row<Format, Format>
    {
        void operator()(void* dst, const void* src, unsigned width) const
        {
            std::memmove(dst, src, width * Format::pix_width);
        }
    };

    // Top-level conversion function, converts one pixel format to any other.
    template<class DstFormat, class SrcFormat, class RenBuf>
    void convert(RenBuf* dst, const RenBuf* src)
    {
        color_conv(dst, src, conv_row<DstFormat, SrcFormat>());
    }
}



#endif
