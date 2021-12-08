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

#ifndef GRAPHIC_GEOMETRY_AUTO_VECTOR_INCLUDED
#define GRAPHIC_GEOMETRY_AUTO_VECTOR_INCLUDED

#include <cstddef>
#include <cstring>
#include "securec.h"
#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "heap_base.h"

namespace OHOS {
/**
 * @file agg_block_allocater.h
 *
 * @brief Defines PodAutoVector数组.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 *
 * @brief Defines PodAutoVector,增加了可以增删改的方法.
 *
 * @since 1.0
 * @version 1.0
 */
template <class T, unsigned Size> 
class PodAutoVector : public HeapBase {
public:
    using ValueType = T;
    using SelfType = PodAutoVector<T, Size>;

    PodAutoVector() : size_(0) {}

    void Clear()
    {
        size_ = 0;
    }

    void RemoveAll()
    {
        size_ = 0;
    }

    void Add(const T& val)
    {
        data_[size_++] = val;
    }

    void IncSize(unsigned size)
    {
        data_ += size;
    }

    void PushBack(const T& val)
    {
        data_[size_++] = val;
    }

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

    unsigned Size() const
    {
        return size_;
    }

private:
    T data_[Size];
    unsigned size_;
};
}
