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

#ifndef GRAPHIC_GEOMETRY_POS_VECTOR_INCLUDED
#define GRAPHIC_GEOMETRY_POS_VECTOR_INCLUDED

#include <cstddef>
#include <cstring>
#include "securec.h"
#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "heap_base.h"

namespace OHOS {
/**
 * @file graphic_geometry_arc.h
 *
 * @brief Defines PodVector，提供了序列化与反序列化的方法.
 *
 * @since 1.0
 * @version 1.0
 */
template <class T> class PodVector : public HeapBase {
public:
    using ValueType = T;

    PodVector() : size_(0), capacity_(0), data_(0) {}

    PodVector(unsigned cap, unsigned extraTail = 0);

    PodVector(const PodVector<T>&);

    ~PodVector()
    {
        ArrAllocator<T>::Deallocate(data_, capacity_);
    }

    const PodVector<T>& operator=(const PodVector<T>&);

    void Capacity(unsigned cap, unsigned extraTail = 0);

    unsigned Capacity() const
    {
        return capacity_;
    }

    void Allocate(unsigned size, unsigned extraTail = 0);

    void Resize(unsigned newSize);

    void Zero()
    {
        std::memset(data_, 0, sizeof(T) * size_);
    }

    void Add(const T& val)
    {
        data_[size_++] = val;
    }

    void PushBack(const T& val)
    {
        data_[size_++] = val;
    }

    void InsertAt(unsigned pos, const T& val);

    void IncSize(unsigned size)
    {
        size_ += size;
    }

    unsigned Size() const
    {
        return size_;
    }

    unsigned ByteSize() const
    {
        return size_ * sizeof(T);
    }

    void Serialize(int8u* ptr) const;

    void Deserialize(const int8u* data, unsigned byteSize);

    const T& operator[](unsigned index) const
    {
        return data_[index];
    }

    T& operator[](unsigned index)
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

    T ValueAt(unsigned index) const
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

    void CutAt(unsigned num)
    {
        if (num < size_) {
            size_ = num;
        }
    }

    void RemoveAll()
    {
        size_ = 0;
    }

    void Clear()
    {
        size_ = 0;
    }

private:
    unsigned size_;
    unsigned capacity_;
    T* data_;
};
}
