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

#ifndef AGG_SPAN_CONVERTER_INCLUDED
#define AGG_SPAN_CONVERTER_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"

namespace OHOS
{
    //----------------------------------------------------------span_converter
    template<class SpanGenerator, class SpanConverter> class span_converter
    {
    public:
        typedef typename SpanGenerator::color_type color_type;

        span_converter(SpanGenerator& span_gen, SpanConverter& span_cnv) : 
            m_span_gen(&span_gen), m_span_cnv(&span_cnv) {}

        void attach_generator(SpanGenerator& span_gen) { m_span_gen = &span_gen; }
        void attach_converter(SpanConverter& span_cnv) { m_span_cnv = &span_cnv; }

        //--------------------------------------------------------------------
        void prepare() 
        { 
            m_span_gen->prepare(); 
            m_span_cnv->prepare();
        }

        //--------------------------------------------------------------------
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
