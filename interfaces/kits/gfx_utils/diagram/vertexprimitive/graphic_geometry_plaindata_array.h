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

#include "gfx_utils/diagram/common/graphic_common_basics.h"
#include "gfx_utils/heap_base.h"
#include "securec.h"
namespace OHOS {
/**
 * @file GraphicGeometryPlainDataArray.h
 *
 * @brief Defines GraphicGeometryPlainDataArray,Variable capacity.
 *
 * @since 1.0
 * @version 1.0
 */
template <class T>
class GraphicGeometryPlainDataArray : public HeapBase {
public:
    using SelfType = GraphicGeometryPlainDataArray<T>;

    ~GraphicGeometryPlainDataArray()
    {
        GeometryArrayAllocator<T>::Deallocate(data_, size_);
    }

    GraphicGeometryPlainDataArray() : data_(0), size_(0) {}
    /**
     *
     * @brief Construct definitions podarray array.
     * @param size Initial capacity.
     * @since 1.0
     * @version 1.0
     */
    GraphicGeometryPlainDataArray(unsigned size)
        : data_(GeometryArrayAllocator<T>::Allocate(size)), size_(size) {}

    GraphicGeometryPlainDataArray(const SelfType& podArray)
        : data_(GeometryArrayAllocator<T>::Allocate(podArray.size_)), size_(podArray.size_)
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
     * @brief Gets the element of the specified index.
     *
     * @since 1.0
     * @version 1.0
     */
    const T& operator[](unsigned index) const
    {
        return data_[index];
    }
    /**
     * @brief Gets the element of the specified index.
     *
     * @since 1.0
     * @version 1.0
     */
    T& operator[](unsigned index)
    {
        return data_[index];
    }
    /**
     * @brief Gets the element of the specified index.
     *
     * @since 1.0
     * @version 1.0
     */
    T ValueAt(unsigned index) const
    {
        return data_[index];
    }
    /**
     * @brief Gets the element of the specified index.
     *
     * @since 1.0
     * @version 1.0
     */
    const T& IndexAt(unsigned index) const
    {
        return data_[index];
    }
    /**
     * @brief Gets the element of the specified index.
     *
     * @since 1.0
     * @version 1.0
     */
    T& IndexAt(unsigned index)
    {
        return data_[index];
    }
    /**
     * @brief Get element header address.
     *
     * @since 1.0
     * @version 1.0
     */
    const T* Data() const
    {
        return data_;
    }
    /**
     * @brief Get element header address.
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
     * @brief Modify the capacity of the definitions podarray array.
     * @param size capacity
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
     * @brief Get the number of elements.
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
