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
* @file span_pattern_rgba.h
* @brief Defines pattern的扫描线
* @since 1.0
* @version 1.0
*/


#ifndef GRAPHIC_SPAN_PATTERN_RGBA_INCLUDED
#define GRAPHIC_SPAN_PATTERN_RGBA_INCLUDED

namespace OHOS
{
    template<class Source> class span_pattern_rgba
    {
    public:
        typedef Source source_type;
        typedef typename source_type::color_type color_type;
        typedef typename source_type::order_type order_type;
        typedef typename color_type::value_type value_type;

        //--------------------------------------------------------------------
        span_pattern_rgba() {}
        span_pattern_rgba(source_type& src, 
                          unsigned offset_x, unsigned offset_y) :
            m_src(&src),
            m_offset_x(offset_x),
            m_offset_y(offset_y)
        {}

        /**
         * @brief prepare 预备用给render_scanlines_aa中的
         */
        void prepare() {}
        /**
         * @brief generate 从m_src取出rgba赋予span中的rgba
         * @param span 扫描线
         * @param x 坐标-x
         * @param y 坐标-y
         * @param len 扫描线长度
         */
        void generate(color_type* span, int x, int y, unsigned len)
        {   
            //z坐标加上x，y的偏移量
            x += m_offset_x;
            y += m_offset_y;
            //从对应的image_accessors模板中取出对应像素
            const value_type* p = (const value_type*)m_src->span(x, y, len);
            for(;len;--len,p = (const value_type*)m_src->next_x(),++span)
            {
                if (p) {
                    //从source_type取出相应像素。
                    span->r = p[order_type::R];
                    span->g = p[order_type::G];
                    span->b = p[order_type::B];
                    span->a = p[order_type::A];
                } else {
                    //默认的颜色黑色不透明
                    span->r = 0;
                    span->g = 0;
                    span->b = 0;
                    span->a = 255;//0-透明255-不透明
                }
            }
        }

    private:
        source_type* m_src;
        unsigned     m_offset_x;
        unsigned     m_offset_y;

    };

}

#endif

