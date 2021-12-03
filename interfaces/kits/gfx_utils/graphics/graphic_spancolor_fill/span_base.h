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
* @file span_allocator_converter.h
* @brief Defines 扫描线分配器和转换器
* @since 1.0
* @version 1.0
*/


#ifndef SPAN_ALLOCATOR_CONVERTER_INCLUDED
#define SPAN_ALLOCATOR_CONVERTER_INCLUDED

#include "gfx_utils/graphics/graphic_geometry/agg_array.h"

namespace OHOS
{

    template<class ColorT> class span_allocator
    {
    public:
        typedef ColorT color_type;

        /**
         * @brief allocate 重新分配计算m_span长度
         * @param span_len 扫描线长度
         * @return m_span的首地址
         */
        AGG_INLINE color_type* allocate(unsigned span_len)
        {
            if(span_len > m_span.size())
            {
                m_span.resize(((span_len + 255) >> 8) << 8);
            }
            return &m_span[0];
        }

        /**
         * @brief span 返回扫描线数组
         * @return
         */
        AGG_INLINE color_type* span()               { return &m_span[0]; }

        /**
         * @brief max_span_len 返回扫描线数组长度
         */
        AGG_INLINE unsigned    max_span_len() const { return m_span.size(); }

    private:
        pod_array<color_type> m_span;
    };



    /**
     *准备和生成SpanGenerator 和 SpanConverter的span
     */
    template<class SpanGenerator, class SpanConverter> class span_converter
    {
    public:
        typedef typename SpanGenerator::color_type color_type;

        span_converter(SpanGenerator& span_gen, SpanConverter& span_cnv) :
            m_span_gen(&span_gen), m_span_cnv(&span_cnv) {}

        /**
         * @brief attach_generator 载入参数SpanGenerator
         * @param span_gen
         */
        void attach_generator(SpanGenerator& span_gen) { m_span_gen = &span_gen; }

        /**
         * @brief attach_converter载入参数SpanConverter
         * @param span_cnv
         */
        void attach_converter(SpanConverter& span_cnv) { m_span_cnv = &span_cnv; }

        /**
         * @brief prepare 调用SpanGenerator 和 SpanConverter的准备
         */
        void prepare()
        {
            m_span_gen->prepare();
            m_span_cnv->prepare();
        }

        /**
         * @brief generate 生成SpanGenerator 和 SpanConverter的span
         * @param span 扫描线
         * @param x 坐标-x
         * @param y 坐标-y
         * @param len 扫描线长度
         */
        void generate(color_type* span, int x, int y, unsigned len)
        {
            m_span_gen->generate(span, x, y, len);
            m_span_cnv->generate(span, x, y, len);
        }

    private:
        SpanGenerator* m_span_gen;
        SpanConverter* m_span_cnv;
    };
}

#endif


