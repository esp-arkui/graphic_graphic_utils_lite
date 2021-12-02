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

#ifndef AGG_SPAN_ALLOCATOR_INCLUDED
#define AGG_SPAN_ALLOCATOR_INCLUDED

#include "gfx_utils/graphics/graphic_geometry/agg_array.h"

namespace OHOS
{
    //----------------------------------------------------------span_allocator
    template<class ColorT> class span_allocator
    {
    public:
        typedef ColorT color_type;

        //--------------------------------------------------------------------
        GRAPTHIC_INLINE color_type* allocate(unsigned span_len)
        {
            if(span_len > m_span.size())
            {
                // To reduce the number of reallocs we align the 
                // span_len to 256 color elements. 
                // Well, I just like this number and it looks reasonable.
                //-----------------------
                m_span.resize(((span_len + 255) >> 8) << 8);
            }
            return &m_span[0];
        }

        GRAPTHIC_INLINE color_type* span()               { return &m_span[0]; }
        GRAPTHIC_INLINE unsigned    max_span_len() const { return m_span.size(); }

    private:
        pod_array<color_type> m_span;
    };
}


#endif


