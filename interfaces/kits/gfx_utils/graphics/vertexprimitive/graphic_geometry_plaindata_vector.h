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
 * @brief Defines PodVector.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_POD_VECTOR_INCLUDED
#define GRAPHIC_GEOMETRY_POD_VECTOR_INCLUDED

#include <cstddef>
#include <cstring>

#include "gfx_utils/graphics/common/graphic_common_basics.h"
#include "gfx_utils/heap_base.h"
#include "securec.h"

namespace OHOS {
    /**
     * @file GeometryPlainDataVector.h
     *
     * @brief Defines PodVector，提供了序列化与反序列化的方法.
     *
     * @since 1.0
     * @version 1.0
     */
    template <class T>
    class GeometryPlainDataVector : public HeapBase {
    public:
        using ValueType = T;

        GeometryPlainDataVector() :
            size_(0), capacity_(0), data_(0)
        {}

        /**
         *
         * @brief 构造一个PodVector.
         * @param cap 容量,extraTail
         * @since 1.0
         * @version 1.0
         */
        GeometryPlainDataVector(unsigned cap, unsigned extraTail = 0);

        GeometryPlainDataVector(const GeometryPlainDataVector<T>&);

        ~GeometryPlainDataVector()
        {
            GeometryArrayAllocator<T>::Deallocate(data_, capacity_);
        }

        const GeometryPlainDataVector<T>& operator=(const GeometryPlainDataVector<T>&);

        /**
         *
         * @brief 设置容量.
         * @since 1.0
         * @version 1.0
         */
        void Capacity(unsigned cap, unsigned extraTail = 0);

        /**
         *
         * @brief 获取当前容量.
         * @since 1.0
         * @version 1.0
         */
        unsigned Capacity() const
        {
            return capacity_;
        }
        /**
         *
         * @brief 申请内存.
         * @param size 容量,extraTail 扩展容量
         * @since 1.0
         * @version 1.0
         */
        void Allocate(unsigned size, unsigned extraTail = 0);
        /**
         *
         * @brief 数组扩容.
         * @param 指定新容量
         * @since 1.0
         * @version 1.0
         */
        void Resize(unsigned newSize);

        /**
         *
         * @brief 把数据全设置为0.
         *
         * @since 1.0
         * @version 1.0
         */
        void CleanData()
        {
            memset_s(data_, sizeof(T) * size_, 0, sizeof(T) * size_);
        }
        /**
         *
         * @brief 末尾增加元素.
         *
         * @since 1.0
         * @version 1.0
         */
        void Add(const T& val)
        {
            data_[size_++] = val;
        }
        /**
         *
         * @brief 末尾增加元素.
         *
         * @since 1.0
         * @version 1.0
         */
        void PushBack(const T& val)
        {
            data_[size_++] = val;
        }
        /**
         *
         * @brief 指定位置插入元素.
         *
         * @since 1.0
         * @version 1.0
         */
        void InsertAt(unsigned pos, const T& val);

        /**
         *
         * @brief 增加容量.
         *
         * @since 1.0
         * @version 1.0
         */
        void IncSize(unsigned size)
        {
            size_ += size;
        }
        /**
         *
         * @brief 获取容量.
         *
         * @since 1.0
         * @version 1.0
         */
        unsigned Size() const
        {
            return size_;
        }

        /**
         *
         * @brief 获取数据字节数.
         *
         * @since 1.0
         * @version 1.0
         */
        unsigned ByteSize() const
        {
            return size_ * sizeof(T);
        }

        /**
         *
         * @brief 把ptr指向的数据序列化到数组.
         *
         * @since 1.0
         * @version 1.0
         */
        void Serialize(int8u* ptr) const;

        /**
         *
         * @brief 把数组反序列化到data.
         *
         * @since 1.0
         * @version 1.0
         */
        void Deserialize(const int8u* data, unsigned byteSize);
        /**
         *
         * @brief 获取指定索引元素.
         *
         * @since 1.0
         * @version 1.0
         */
        const T& operator[](unsigned index) const
        {
            return data_[index];
        }
        /**
         *
         * @brief 获取指定索引元素.
         *
         * @since 1.0
         * @version 1.0
         */
        T& operator[](unsigned index)
        {
            return data_[index];
        }
        /**
         *
         * @brief 获取指定索引元素.
         *
         * @since 1.0
         * @version 1.0
         */
        const T& IndexAt(unsigned index) const
        {
            return data_[index];
        }
        /**
         *
         * @brief 获取指定索引元素.
         *
         * @since 1.0
         * @version 1.0
         */
        T& IndexAt(unsigned index)
        {
            return data_[index];
        }
        /**
         *
         * @brief 获取指定索引元素.
         *
         * @since 1.0
         * @version 1.0
         */
        T ValueAt(unsigned index) const
        {
            return data_[index];
        }
        /**
         *
         * @brief 获取数组指针.
         *
         * @since 1.0
         * @version 1.0
         */
        const T* Data() const
        {
            return data_;
        }

        /**
         *
         * @brief 获取数组指针.
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
         * @brief 重置容量.
         *
         * @since 1.0
         * @version 1.0
         */
        void CutAt(unsigned num)
        {
            if (num < size_) {
                size_ = num;
            }
        }
        /**
         *
         * @brief 删除所有元素.
         *
         * @since 1.0
         * @version 1.0
         */
        void RemoveAll()
        {
            size_ = 0;
        }
        /**
         *
         * @brief 删除所有元素.
         *
         * @since 1.0
         * @version 1.0
         */
        void Clear()
        {
            size_ = 0;
        }

    private:
        unsigned size_;
        unsigned capacity_;
        T* data_;
    };

    template <class T>
    void GeometryPlainDataVector<T>::Capacity(unsigned cap, unsigned extraTail)
    {
        size_ = 0;
        if (cap > capacity_) {
            GeometryArrayAllocator<T>::Deallocate(data_, capacity_);
            capacity_ = cap + extraTail;
            data_ = capacity_ ? GeometryArrayAllocator<T>::Allocate(capacity_) : 0;
        }
    }

    template <class T>
    void GeometryPlainDataVector<T>::Allocate(unsigned size, unsigned extraTail)
    {
        Capacity(size, extraTail);
        size_ = size;
    }

    template <class T>
    void GeometryPlainDataVector<T>::Resize(unsigned newSize)
    {
        if (newSize > size_) {
            if (newSize > capacity_) {
                T* data = GeometryArrayAllocator<T>::Allocate(newSize);
                memcpy_s(data, newSize, data_, size_ * sizeof(T));
                GeometryArrayAllocator<T>::Deallocate(data_, capacity_);
                data_ = data;
            }
        } else {
            size_ = newSize;
        }
    }

    template <class T>
    GeometryPlainDataVector<T>::GeometryPlainDataVector(unsigned cap, unsigned extraTail)
        : size_(0), capacity_(cap + extraTail),
          data_(GeometryArrayAllocator<T>::Allocate(capacity_))
    {
    }

    template <class T>
    GeometryPlainDataVector<T>::GeometryPlainDataVector(const GeometryPlainDataVector<T>& v)
        : size_(v.size_), capacity_(v.capacity_),
          data_(v.capacity_ ? GeometryArrayAllocator<T>::Allocate(v.capacity_) : 0)
    {
        memcpy_s(data_, sizeof(T) * v.size_, v.data_, sizeof(T) * v.size_);
    }

    template <class T>
    const GeometryPlainDataVector<T>& GeometryPlainDataVector<T>::operator=(const GeometryPlainDataVector<T>& val)
    {
        Allocate(val.size_);
        if (val.size_) {
            memcpy_s(data_, sizeof(T) * val.size_, val.data_, sizeof(T) * val.size_);
        }
        return *this;
    }

    template <class T>
    void GeometryPlainDataVector<T>::InsertAt(unsigned pos, const T& val)
    {
        if (pos >= size_) {
            data_[size_] = val;
        } else {
            memmove_s(data_ + pos + 1, (size_ - pos) * sizeof(T), data_ + pos, (size_ - pos) * sizeof(T));
            data_[pos] = val;
        }
        ++size_;
    }

} // namespace OHOS
#endif
