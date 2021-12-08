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
#include "securec.h"
#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "heap_base.h"
namespace OHOS {

/**
 * @file graphic_geometry_arc.h
 *
 * @brief Defines PodArray，可以修改大小.
 *
 * @since 1.0
 * @version 1.0
 */
template <class T> class PodArray : public HeapBase {
public:
    using ValueType = T;
    using SelfType = PodArray<T>;

    ~PodArray()
    {
        ArrAllocator<T>::Deallocate(data_, size_);
    }

    PodArray() : data_(0), size_(0) {}

    PodArray(unsigned size) : data_(ArrAllocator<T>::Allocate(size)), size_(size) {}

    PodArray(const SelfType& v) : data_(ArrAllocator<T>::Allocate(v.size_)), size_(v.size_)
    {
        std::memcpy_s(data_, sizeof(T) * size_, v.data_, sizeof(T) * size_);
    }

    const SelfType& operator=(const SelfType& v)
    {
        Resize(v.size());
        std::memcpy(data_, sizeof(T) * size_, v.data_, sizeof(T) * size_);
        return *this;
    }

    const T& operator[](unsigned index) const
    {
        return data_[index];
    }

    T& operator[](unsigned index)
    {
        return data_[index];
    }

    T ValueAt(unsigned index) const
    {
        return data_[index];
    }

    const T& IndexAt(unsigned index) const
    {
        return data_[index];
    }

    T& IndexAt(unsigned index)
    {
        return data_[index];
    }

    const T* Data() const
    {
        return data_;
    }

    T* Data()
    {
        return data_;
    }

    void Resize(unsigned size)
    {
        if (size != size_) {
            pod_allocator<T>::Deallocate(data_, size_);
            data_ = ArrAllocator<T>::Allocate(size_ = size);
        }
    }

    unsigned Size() const
    {
        return size_;
    }

private:
    T* data_;
    unsigned size_;
};

}
