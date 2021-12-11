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

namespace OHOS {
    template <class ColorT>
    class SpanAllocator {
    public:
        typedef ColorT color_type;

        /**
         * @brief Resize 重新分配计算m_span长度
         * @param span_len 扫描线长度
         * @return m_span的首地址
         */
        GRAPHIC_GEOMETRY_INLINE color_type* Resize(unsigned span_len)
        {
            if (span_len > spans.Size()) {
                spans.Resize(((span_len + 255) >> 8) << 8);
            }
            return &spans[0];
        }

        /**
         * @brief span 返回扫描线数组首地址
         * @return
         */
        GRAPHIC_GEOMETRY_INLINE color_type* span()
        {
            return &spans[0];
        }

        /**
         * @brief GetMaxSpansSize 返回扫描线数组长度
         */
        GRAPHIC_GEOMETRY_INLINE unsigned GetMaxSpansSize() const
        {
            return spans.size();
        }

    private:
        PodArray<color_type> spans;
    };

} // namespace OHOS

#endif
