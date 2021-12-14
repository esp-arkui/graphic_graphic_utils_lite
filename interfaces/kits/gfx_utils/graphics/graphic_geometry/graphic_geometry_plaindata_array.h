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
 * @{
 *
 * @brief Defines Arc.
 *
 * @since 1.0
 * @version 1.0
 */
#ifndef GRAPHIC_GEOMETRY_POD_ARRAY_INCLUDED
#define GRAPHIC_GEOMETRY_POD_ARRAY_INCLUDED

#include <cstddef>
#include <cstring>

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"
#include "gfx_utils/heap_base.h"
#include "securec.h"
namespace OHOS {
    /**
     * @file GeometryPlainDataArray.h
     *
     * @brief Defines GeometryPlainDataArray数组,容量可变.
     *
     * @since 1.0
     * @version 1.0
     */
    template <class T>
    class GeometryPlainDataArray : public HeapBase {
    public:
        using ValueType = T;
        using SelfType = GeometryPlainDataArray<T>;

        ~GeometryPlainDataArray()
        {
            GeometryArrayAllocator<T>::Deallocate(data_, size_);
        }

        GeometryPlainDataArray() :
            data_(0), size_(0)
        {}
        /**
         *
         * @brief 构造Defines PodArray数组.
         * @param size 初始容量
         * @since 1.0
         * @version 1.0
         */
        GeometryPlainDataArray(unsigned size) :
            data_(GeometryArrayAllocator<T>::Allocate(size)), size_(size)
        {}

        GeometryPlainDataArray(const SelfType& podArray) :
            data_(GeometryArrayAllocator<T>::Allocate(podArray.size_)), size_(podArray.size_)
        {
            memcpy_s(data_, sizeof(T) * size_, podArray.data_, sizeof(T) * size_);
        }

        const SelfType& operator=(const SelfType& podArray)
        {
            Resize(podArray.Size());
            memcpy_s(data_, sizeof(T) * size_, podArray.data_, sizeof(T) * size_);
            return *this;
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
        T ValueAt(unsigned index) const
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
         * @brief 获取元素首地址.
         *
         * @since 1.0
         * @version 1.0
         */
        const T* Data() const
        {
            return data_;
        }
        /**
         * @brief 获取元素首地址.
         *
         * @since 1.0
         * @version 1.0
         */
        T* Data()
        {
            return data_;
        }

        /**
         *
         * @brief 修改Defines PodArray数组容量.
         * @param size 容量
         * @since 1.0
         * @version 1.0
         */
        void Resize(unsigned size)
        {
            if (size != size_) {
                GeometryArrayAllocator<T>::Deallocate(data_, size_);
                data_ = GeometryArrayAllocator<T>::Allocate(size_ = size);
            }
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
        T* data_;
        unsigned size_;
    };

} // namespace OHOS
#endif
