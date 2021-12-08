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
#include "securec.h"
#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "heap_base.h"

namespace OHOS {

template <class T, unsigned S = 6> class PodBvector : public HeapBase {
public:
    enum BlockScaleEnum { BLOCK_SHIFT = S, BLOCK_SIZE = 1 << BLOCK_SHIFT, BLOCK_MASK = BLOCK_SIZE - 1 };

    using ValueType = T;

    PodBvector();

    PodBvector(unsigned blockPtrInc);

    ~PodBvector();

    PodBvector(const PodBvector<T, S>& v);

    const PodBvector<T, S>& operator=(const PodBvector<T, S>& v);

    void RemoveAll()
    {
        size_ = 0;
    }
    void Clear()
    {
        size_ = 0;
    }
    void FreeAll()
    {
        FreeTail(0);
    }

    void ModifyLast(const T& val);

    void RemoveLast();

    int AllocateContinuousBlock(unsigned numElements);

    void FreeTail(unsigned size);

    void Add(const T& val);

    void PushBack(const T& val)
    {
        Add(val);
    }

    void AddArray(const T* ptr, unsigned numElem)
    {
        while (numElem--) {
            Add(*ptr++);
        }
    }

    void CutAt(unsigned size)
    {
        if (size < size_) {
            size_ = size;
        }
    }

    template <class DataAccessor> void AddData(DataAccessor& data)
    {
        while (data.size()) {
            Add(*data);
            ++data;
        }
    }

    unsigned Size() const
    {
        return size_;
    }

    const T& operator[](unsigned index) const
    {
        return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
    }

    T& operator[](unsigned index)
    {
        return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
    }

    T& IndexAt(unsigned index)
    {
        return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
    }

    T ValueAt(unsigned index) const
    {
        return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
    }

    const T& IndexAt(unsigned index) const
    {
        return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
    }

    T& Curr(unsigned index)
    {
        return (*this)[index];
    }

    const T& Prev(unsigned index) const
    {
        return (*this)[(index + size_ - 1) % size_];
    }

    T& Prev(unsigned index)
    {
        return (*this)[(index + size_ - 1) % size_];
    }

    const T& Curr(unsigned index) const
    {
        return (*this)[index];
    }

    const T& Next(unsigned index) const
    {
        return (*this)[(index + 1) % size_];
    }

    T& Next(unsigned index)
    {
        return (*this)[(index + 1) % size_];
    }

    const T& Last() const
    {
        return (*this)[size_ - 1];
    }

    T& Last()
    {
        return (*this)[size_ - 1];
    }

    void Deserialize(const int8u* data, unsigned byteSize);

    void Deserialize(unsigned start, const T& emptyVal, const int8u* data, unsigned byteSize);

    unsigned ByteSize() const;

    void Serialize(int8u* ptr) const;

    template <class ByteAccessor> void Deserialize(unsigned start, const T& emptyVal, ByteAccessor data)
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

    template <class ByteAccessor> void Deserialize(ByteAccessor data)
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

    const T* Block(unsigned nb) const
    {
        return blocks_[nb];
    }

private:
    void Allocatelock(unsigned nb);
    T* DataPtr();

    unsigned size_;
    unsigned numBlocks_;
    unsigned maxBlocks_;
    T** blocks_;
    unsigned blockPtrInc_;
};




}
