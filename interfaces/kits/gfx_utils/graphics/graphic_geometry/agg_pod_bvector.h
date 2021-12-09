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

#ifndef GRAPHIC_GEOMETRY_POS_BVECTOR_INCLUDED
#define GRAPHIC_GEOMETRY_POS_BVECTOR_INCLUDED

#include <cstddef>
#include <cstring>

#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "heap_base.h"
#include "securec.h"

namespace OHOS {

/**
 * @brief Defines PodBvector.
 *
 * @since 1.0
 * @version 1.0
 */
template <class T, unsigned S = 6>
class PodBvector : public HeapBase {
public:
    enum BlockScaleEnum
    {
        BLOCK_SHIFT = S,
        BLOCK_SIZE = 1 << BLOCK_SHIFT,
        BLOCK_MASK = BLOCK_SIZE - 1
    };

    using ValueType = T;

    PodBvector();
    /**
    * @brief ����һ�� PodBvector.
    * @param blockPtrInc ÿ�����С
    * @since 1.0
    * @version 1.0
    */
    PodBvector(unsigned blockPtrInc);

    ~PodBvector();

    PodBvector(const PodBvector<T, S>& v);

    const PodBvector<T, S>& operator=(const PodBvector<T, S>& v);
    /**
    * @brief ���.
    * @since 1.0
    * @version 1.0
    */
    void RemoveAll()
    {
        size_ = 0;
    }
    /**
    * @brief ���.
    * @since 1.0
    * @version 1.0
    */
    void Clear()
    {
        size_ = 0;
    }
    /**
    * @brief �ͷ���ͷԪ��ռ�õ��ڴ��.
    * @since 1.0
    * @version 1.0
    */
    void FreeAll()
    {
        FreeTail(0);
    }
    /**
    * @brief �޸����һ��Ԫ��.
    * @since 1.0
    * @version 1.0
    */
    void ModifyLast(const T& val);
    /**
    * @brief ɾ�����һ��Ԫ��.
    * @since 1.0
    * @version 1.0
    */
    void RemoveLast();
    /**
    * @brief ����numElements��С�������ڴ��.
    * @param numElements Ҫ�����Ԫ�ظ���
    * @since 1.0
    * @version 1.0
    */
    int AllocateContinuousBlock(unsigned numElements);
    /**
    * @brief ��ĩβ�ͷ�Ԫ��.
    * @param size Ҫ�ͷŵĸ���
    * @since 1.0
    * @version 1.0
    */
    void FreeTail(unsigned size);
    /**
    * @brief ����һ��Ԫ��.
    * @param val ���ӵ�Ԫ��
    * @since 1.0
    * @version 1.0
    */
    void Add(const T& val);
    /**
    * @brief ����һ��Ԫ��.
    * @param val ���ӵ�Ԫ��
    * @since 1.0
    * @version 1.0
    */
    void PushBack(const T& val)
    {
        Add(val);
    }

    /**
    * @brief ����һ������.
    * @param ptr Ҫ���ӵ������׵�ַ��numElem Ԫ�ظ���
    * @since 1.0
    * @version 1.0
    */
    void AddArray(const T* ptr, unsigned numElem)
    {
        while (numElem--) {
            Add(*ptr++);
        }
    }

    /**
    * @brief ��С��������.
    * @since 1.0
    * @version 1.0
    */
    void CutAt(unsigned size)
    {
        if (size < size_) {
            size_ = size;
        }
    }
    /**
    * @brief ��������.
    * @since 1.0
    * @version 1.0
    */
    template <class DataAccessor>
    void AddData(DataAccessor& data)
    {
        while (data.size()) {
            Add(*data);
            ++data;
        }
    }
    /**
    * @brief ��ȡ��������.
    * @since 1.0
    * @version 1.0
    */
    unsigned Size() const
    {
        return size_;
    }
    /**
    * @brief ��ȡָ���������ݿ�.
    * @since 1.0
    * @version 1.0
    */
    const T& operator[](unsigned index) const
    {
        return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
    }
    /**
    * @brief ��ȡָ���������ݿ�.
    * @since 1.0
    * @version 1.0
    */
    T& operator[](unsigned index)
    {
        return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
    }
    /**
    * @brief ��ȡָ���������ݿ�.
    * @since 1.0
    * @version 1.0
    */
    T& IndexAt(unsigned index)
    {
        return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
    }
    /**
    * @brief ��ȡָ���������ݿ�.
    * @since 1.0
    * @version 1.0
    */
    T ValueAt(unsigned index) const
    {
        return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
    }
    /**
    * @brief ��ȡ��ǰ�������ݿ�.
    * @since 1.0
    * @version 1.0
    */
    const T& IndexAt(unsigned index) const
    {
        return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
    }
    /**
    * @brief ��ȡ��ǰ�������ݿ�.
    * @since 1.0
    * @version 1.0
    */
    T& Curr(unsigned index)
    {
        return (*this)[index];
    }
    /**
    * @brief ��ȡ��ǰ�����ϵ�ǰһ�����ݿ�.
    * @since 1.0
    * @version 1.0
    */
    const T& Prev(unsigned index) const
    {
        return (*this)[(index + size_ - 1) % size_];
    }
    /**
    * @brief ��ȡ��ǰ�����ϵ�ǰһ�����ݿ�.
    * @since 1.0
    * @version 1.0
    */
    T& Prev(unsigned index)
    {
        return (*this)[(index + size_ - 1) % size_];
    }
    /**
    * @brief ��ȡ��ǰ�����ϵ����ݿ�.
    * @since 1.0
    * @version 1.0
    */
    const T& Curr(unsigned index) const
    {
        return (*this)[index];
    }

    /**
    * @brief ��ȡ��ǰ�����ϵ���һ�����ݿ�.
    * @since 1.0
    * @version 1.0
    */
    const T& Next(unsigned index) const
    {
        return (*this)[(index + 1) % size_];
    }

    /**
    * @brief ��ȡ��ǰ�����ϵ���һ�����ݿ�.
    * @since 1.0
    * @version 1.0
    */
    T& Next(unsigned index)
    {
        return (*this)[(index + 1) % size_];
    }
    /**
     * @brief ��ȡĩβ����.
     * @since 1.0
     * @version 1.0
     */
    const T& Last() const
    {
        return (*this)[size_ - 1];
    }

    /**
     * @brief ��ȡĩβ����.
     * @since 1.0
     * @version 1.0
     */
    T& Last()
    {
        return (*this)[size_ - 1];
    }
    /**
     * @brief �����л����ݵ��ڴ��.
     * @param data Ҫ�����е����ݣ�byteSize Ҫ���л����ֽ���
     * @since 1.0
     * @version 1.0
     */
    void Deserialize(const int8u* data, unsigned byteSize);

     /**
     * @brief �����л����ݵ��ڴ��.
     * @param start ָ��λ��, ָ��λ�����������ݣ�data Ҫ�����е����ݣ�byteSize Ҫ���л����ֽ���
     * @since 1.0
     * @version 1.0
     */
    void Deserialize(unsigned start, const T& emptyVal, const int8u* data, unsigned byteSize);
    /**
     * @brief ��ȡ�ֽ���.
     * 
     * @since 1.0
     * @version 1.0
     */
    unsigned ByteSize() const;

    /**
     * @brief ���л����ݵ�ָ���ĵ�ַ.
     * 
     * @since 1.0
     * @version 1.0
     */
    void Serialize(int8u* ptr) const;

     /**
     * @brief �����л����ݵ��ڴ��.
     * @param start ָ��λ��, ָ��λ�����������ݣ�data Ҫ�����е�����
     * @since 1.0
     * @version 1.0
     */
    template <class ByteAccessor>
    void Deserialize(unsigned start, const T& emptyVal, ByteAccessor data)
    {
        while (size_ < start) {
            Add(emptyVal);
        }

        unsigned elesize_ = data.size() / sizeof(T);
        for (unsigned i = 0; i < elesize_; ++i) {
            int8u* ptr;
            if (start + i < size_) {
                ptr = (int8u*)(&((*this)[start + i]));
            } else {
                ptr = (int8u*)DataPtr();
                ++size_;
            }
            for (unsigned j = 0; j < sizeof(T); ++j) {
                *ptr++ = *data;
                ++data;
            }
        }
    }
    /**
     * @brief �����л����ݵ��ڴ��.
     * 
     * @since 1.0
     * @version 1.0
     */
    template <class ByteAccessor>
    void Deserialize(ByteAccessor data)
    {
        RemoveAll();
        unsigned elesize_ = data.size() / sizeof(T);

        for (unsigned i = 0; i < elesize_; ++i) {
            int8u* ptr = (int8u*)DataPtr();
            for (unsigned j = 0; j < sizeof(T); ++j) {
                *ptr++ = *data;
                ++data;
            }
            ++size_;
        }
    }

    /**
     * @brief ��ȡָ���Ŀ��ַ.
     * 
     * @since 1.0
     * @version 1.0
     */
    const T* Block(unsigned nb) const
    {
        return blocks_[nb];
    }

private:
    /**
     * @brief ����ָ����С���ڴ��.
     * 
     * @since 1.0
     * @version 1.0
     */
    void Allocatelock(unsigned nb); 
    T* DataPtr();

    unsigned size_;         //Ԫ�ظ���
    unsigned numBlocks_;    //������
    unsigned maxBlocks_;    //������
    T** blocks_;            //���п���׵�ַ
    unsigned blockPtrInc_;  //ÿ����Ĵ�С
};

} // namespace OHOS
