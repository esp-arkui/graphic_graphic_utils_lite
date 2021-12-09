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
 * @brief Defines PodAutoVector����.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 *
 * @brief Defines PodAutoVector,�����˿�����ɾ�ĵķ�������������̶�.
 *
 * @since 1.0
 * @version 1.0
 */
template <class T, unsigned Size> 
class PodAutoVector : public HeapBase {
public:
    using ValueType = T;
    using SelfType = PodAutoVector<T, Size>;
    /**
     * @brief ����һ��PodAutoVector.
     * 
     * @since 1.0
     * @version 1.0
     */
    PodAutoVector() : size_(0) {}

     /**
     * @brief ���Ԫ��.
     * 
     * @since 1.0
     * @version 1.0
     */
    void Clear()
    {
        size_ = 0;
    }
    /**
     * @brief ���Ԫ��.
     * 
     * @since 1.0
     * @version 1.0
     */
    void RemoveAll()
    {
        size_ = 0;
    }
    /**
     * @brief ����һ��Ԫ��.
     * 
     * @since 1.0
     * @version 1.0
     */
    void Add(const T& val)
    {
        data_[size_++] = val;
    }
    /**
     * @brief ƫ�������׵�ַ.
     * 
     * @since 1.0
     * @version 1.0
     */
    void IncSize(unsigned size)
    {
        data_ += size;
    }
    /**
     * @brief ����һ��Ԫ��.
     * 
     * @since 1.0
     * @version 1.0
     */
    void PushBack(const T& val)
    {
        data_[size_++] = val;
    }
    /**
     * @brief ��ȡָ��������Ԫ��.
     * 
     * @since 1.0
     * @version 1.0
     */
    const T& operator[](unsigned index) const
    {
        return data_[index];
    }
    /**
     * @brief ��ȡָ��������Ԫ��.
     * 
     * @since 1.0
     * @version 1.0
     */
    T& operator[](unsigned index)
    {
        return data_[index];
    }
    /**
     * @brief ��ȡָ��������Ԫ��.
     * 
     * @since 1.0
     * @version 1.0
     */
    const T& IndexAt(unsigned index) const
    {
        return data_[index];
    }
    /**
     * @brief ��ȡָ��������Ԫ��.
     * 
     * @since 1.0
     * @version 1.0
     */
    T& IndexAt(unsigned index)
    {
        return data_[index];
    }
    /**
     * @brief ��ȡָ��������Ԫ��.
     * 
     * @since 1.0
     * @version 1.0
     */
    T ValueAt(unsigned index) const
    {
        return data_[index];
    }
    /**
     * @brief ��ȡԪ�ظ���.
     * 
     * @since 1.0
     * @version 1.0
     */
    unsigned Size() const
    {
        return size_;
    }

private:
    T data_[Size];  //����Ԫ�ص�����
    unsigned size_; //Ԫ������
};
}
