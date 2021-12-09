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
 * @brief Defines PodVector���ṩ�����л��뷴���л��ķ���.
 *
 * @since 1.0
 * @version 1.0
 */
template <class T> 
class PodVector : public HeapBase {
public:
    using ValueType = T;

    PodVector() : size_(0), capacity_(0), data_(nullptr) {}

    /**
     *
     * @brief ����һ��PodVector.
     * @param cap ����,extraTail 
     * @since 1.0
     * @version 1.0
     */
    PodVector(unsigned cap, unsigned extraTail = 0);

    PodVector(const PodVector<T>&);

    ~PodVector()
    {
        ArrAllocator<T>::Deallocate(data_, capacity_);
    }

    const PodVector<T>& operator=(const PodVector<T>&);

    /**
     *
     * @brief ��������.
     * @since 1.0
     * @version 1.0
     */
    void Capacity(unsigned cap, unsigned extraTail = 0);

    /**
     *
     * @brief ��ȡ��ǰ����.
     * @since 1.0
     * @version 1.0
     */
    unsigned Capacity() const
    {
        return capacity_;
    }
    /**
     *
     * @brief �����ڴ�.
     * @param size ����,extraTail ��չ����
     * @since 1.0
     * @version 1.0
     */
    void Allocate(unsigned size, unsigned extraTail = 0);
    /**
     *
     * @brief ��������.
     * @param ָ��������
     * @since 1.0
     * @version 1.0
     */
    void Resize(unsigned newSize);

    /**
     *
     * @brief ������ȫ����Ϊ0.
     *
     * @since 1.0
     * @version 1.0
     */
    void Zero()
    {
        std::memset_s(data_, sizeof(T) * size_, 0, sizeof(T) * size_);
    }
    /**
     *
     * @brief ĩβ����Ԫ��.
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
     * @brief ĩβ����Ԫ��.
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
     * @brief ָ��λ�ò���Ԫ��.
     *
     * @since 1.0
     * @version 1.0
     */
    void InsertAt(unsigned pos, const T& val);

    /**
     *
     * @brief ��������.
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
     * @brief ��ȡ����.
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
     * @brief ��ȡ�����ֽ���.
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
     * @brief ��ptrָ����������л�������.
     *
     * @since 1.0
     * @version 1.0
     */
    void Serialize(int8u* ptr) const;

    /**
     *
     * @brief �����鷴���л���data.
     *
     * @since 1.0
     * @version 1.0
     */
    void Deserialize(const int8u* data, unsigned byteSize);
    /**
     *
     * @brief ��ȡָ������Ԫ��.
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
     * @brief ��ȡָ������Ԫ��.
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
     * @brief ��ȡָ������Ԫ��.
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
     * @brief ��ȡָ������Ԫ��.
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
     * @brief ��ȡָ������Ԫ��.
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
     * @brief ɾ������Ԫ��.
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
     * @brief ��ȡ����ָ��.
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
     * @brief ��������.
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
     * @brief ɾ������Ԫ��.
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
     * @brief ɾ������Ԫ��.
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
}
