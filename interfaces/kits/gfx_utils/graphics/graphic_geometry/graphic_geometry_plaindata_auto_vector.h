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
 * @addtogroup GraphicGeometry
 * @brief Defines PodAutoVector.
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_AUTO_VECTOR_INCLUDED
#define GRAPHIC_GEOMETRY_AUTO_VECTOR_INCLUDED

#include <cstddef>
#include <cstring>

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"
#include "gfx_utils/heap_base.h"
#include "securec.h"

namespace OHOS {
    /**
 *
 * @brief Defines PodAutoVector,增加了可以增删改的方法，最大容量固定.
 *
 * @since 1.0
 * @version 1.0
 */
    template <class T, unsigned SizeN>
    class GeometryPlainDataAutoVector : public HeapBase {
    public:
        using ValueType = T;
        using SelfType = GeometryPlainDataAutoVector<T, SizeN>;
        /**
     * @brief 构造一个PodAutoVector.
     * 
     * @since 1.0
     * @version 1.0
     */
        GeometryPlainDataAutoVector() :
            size_(0)
        {}

        /**
     * @brief 清空元素.
     * @since 1.0
     * @version 1.0
     */
        void Clear()
        {
            size_ = 0;
        }
        /**
     * @brief 清空元素.
     * @since 1.0
     * @version 1.0
     */
        void RemoveAll()
        {
            size_ = 0;
        }
        /**
     * @brief 增加一个元素.
     * @since 1.0
     * @version 1.0
     */
        void Add(const T& val)
        {
            data_[size_++] = val;
        }
        /**
     * @brief 偏移数组首地址.
     * 
     * @since 1.0
     * @version 1.0
     */
        void IncSize(unsigned size)
        {
            data_ += size;
        }
        /**
     * @brief 增加一个元素.
     * 
     * @since 1.0
     * @version 1.0
     */
        void PushBack(const T& val)
        {
            data_[size_++] = val;
        }
        /**
     * @brief 获取指定索引的元素.
     * 
     * @since 1.0
     * @version 1.0
     */
        const T& operator[](unsigned index) const
        {
            return data_[index];
        }
        /**
     * @brief 获取指定索引的元素.
     * 
     * @since 1.0
     * @version 1.0
     */
        T& operator[](unsigned index)
        {
            return data_[index];
        }
        /**
     * @brief 获取指定索引的元素.
     * 
     * @since 1.0
     * @version 1.0
     */
        const T& IndexAt(unsigned index) const
        {
            return data_[index];
        }
        /**
     * @brief 获取指定索引的元素.
     * 
     * @since 1.0
     * @version 1.0
     */
        T& IndexAt(unsigned index)
        {
            return data_[index];
        }
        /**
     * @brief 获取指定索引的元素.
     * 
     * @since 1.0
     * @version 1.0
     */
        T ValueAt(unsigned index) const
        {
            return data_[index];
        }
        /**
     * @brief 获取元素个数.
     * 
     * @since 1.0
     * @version 1.0
     */
        unsigned Size() const
        {
            return size_;
        }

    private:
        T data_[SizeN]; // 保存元素的数组
        unsigned size_; // 元素数量
    };
} // namespace OHOS
#endif
