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

#ifndef GRAPHIC_GEOMETRY_POD_AUTO_ARRAY_INCLUDED
#define GRAPHIC_GEOMETRY_POD_AUTO_ARRAY_INCLUDED

#include <cstddef>
#include <cstring>
#include "securec.h"
#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "heap_base.h"
namespace OHOS {
/**
 * @file graphic_geometry_arc.h
 *
 * @brief Defines PodAutoArray数组.
 *
 * @since 1.0
 * @version 1.0
 */
template <class T, unsigned Size> class PodAutoArray : public HeapBase {
public:
    using ValueType = T;
    using SelfType = PodAutoArray<T, Size>;

    PodAutoArray() {}

    explicit PodAutoArray(const T* data)
    {
        std::memcpy_s(data_, sizeof(T) * Size, data, sizeof(T) * Size);
        std::memcpy_s(data_, sizeof(T) * Size, sizeof(data_), data, sizeof(T) * Size);
    }

    const SelfType& operator=(const T* data)
    {
        std::memcpy(data_, sizeof(T) * Size, data, sizeof(T) * Size);
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

    static unsigned Size()
    {
        return Size;
    }

private:
    T data_[Size]; // data_ 名称修改
};

/**
 * @file graphic_geometry_arc.h
 *
 * @brief Defines PodAutoArray数组适配器.
 *
 * @since 1.0
 * @version 1.0
 */
template <class T> class PodArrayAdaptor : public HeapBase {
public:
    using ValueType = T;

    PodArrayAdaptor(T* array, unsigned size) : data_(array), size_(size) {}

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

    unsigned Size() const
    {
        return size_;
    }

private:
    T* data_;
    unsigned size_;
};

}
