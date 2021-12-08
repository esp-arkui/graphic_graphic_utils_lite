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

#include <gfx_utils/graphics/graphic_geometry/agg_pod_bvector.h>

#include <cmath>

namespace OHOS {

template <class T, unsigned S> PodBvector<T, S>::~PodBvector()
{
    if (numBlocks_) {
        T** blk = blocks_ + numBlocks_ - 1;
        while (numBlocks_--) {
            ArrAllocator<T>::Deallocate(*blk, BLOCK_SIZE);
            --blk;
        }
    }
    ArrAllocator<T*>::Deallocate(blocks_, maxBlocks_);
}

template <class T, unsigned S> void PodBvector<T, S>::FreeTail(unsigned size)
{
    if (size < size_) {
        unsigned nb = (size + BLOCK_MASK) >> BLOCK_SHIFT;
        while (numBlocks_ > nb) {
            ArrAllocator<T>::Deallocate(blocks_[--numBlocks_], BLOCK_SIZE);
        }
        if (numBlocks_ == 0) {
            ArrAllocator<T*>::Deallocate(blocks_, maxBlocks_);
            blocks_ = 0;
            maxBlocks_ = 0;
        }
        size_ = size;
    }
}

template <class T, unsigned S>
PodBvector<T, S>::PodBvector() : size_(0), numBlocks_(0), maxBlocks_(0), blocks_(0), blockPtrInc_(BLOCK_SIZE)
{
}

template <class T, unsigned S>
PodBvector<T, S>::PodBvector(unsigned blockPtrInc)
    : size_(0), numBlocks_(0), maxBlocks_(0), blocks_(0), blockPtrInc_(blockPtrInc)
{
}

template <class T, unsigned S>
PodBvector<T, S>::PodBvector(const PodBvector<T, S>& v)
    : size_(v.size_),
      numBlocks_(v.numBlocks_),
      maxBlocks_(v.maxBlocks_),
      blocks_(v.maxBlocks_ ? ArrAllocator<T*>::Allocate(v.maxBlocks_) : 0),
      blockPtrInc_(v.blockPtrInc_)
{
    unsigned i;
    for (i = 0; i < v.numBlocks_; ++i) {
        blocks_[i] = ArrAllocator<T>::Allocate(BLOCK_SIZE);
        std::memcpy_s(blocks_[i], BLOCK_SIZE * sizeof(T), v.blocks_[i], BLOCK_SIZE * sizeof(T));
    }
}

template <class T, unsigned S> const PodBvector<T, S>& PodBvector<T, S>::operator=(const PodBvector<T, S>& v)
{
    unsigned i;
    for (i = numBlocks_; i < v.numBlocks_; ++i) {
        AllocateBlock(i);
    }
    for (i = 0; i < v.numBlocks_; ++i) {
        std::memcpy_s(blocks_[i], BLOCK_SIZE * sizeof(T), v.blocks_[i], BLOCK_SIZE * sizeof(T));
    }
    size_ = v.size_;
    return *this;
}

template <class T, unsigned S> void PodBvector<T, S>::AllocateBlock(unsigned blockNum)
{
    if (blockNum >= maxBlocks_) {
        T** newBlocks = ArrAllocator<T*>::Allocate(maxBlocks_ + blockPtrInc_);

        if (blocks_) {
            std::memcpy_s(newBlocks, (maxBlocks_ + blockPtrInc_), blocks_, numBlocks_ * sizeof(T*));

            ArrAllocator<T*>::Deallocate(blocks_, maxBlocks_);
        }
        blocks_ = newBlocks;
        maxBlocks_ += blockPtrInc_;
    }
    blocks_[blockNum] = ArrAllocator<T>::Allocate(BLOCK_SIZE);
    numBlocks_++;
}

template <class T, unsigned S> inline T* PodBvector<T, S>::DataPtr()
{
    unsigned blockNum = size_ >> BLOCK_SHIFT;
    if (blockNum >= numBlocks_) {
        AllocateBlock(blockNum);
    }
    return blocks_[blockNum] + (size_ & BLOCK_MASK);
}

template <class T, unsigned S> inline void PodBvector<T, S>::Add(const T& val)
{
    *DataPtr() = val;
    ++size_;
}

template <class T, unsigned S> inline void PodBvector<T, S>::RemoveLast()
{
    if (size_) {
        --size_;
    }
}

template <class T, unsigned S> void PodBvector<T, S>::ModifyLast(const T& val)
{
    RemoveLast();
    Add(val);
}

template <class T, unsigned S> int PodBvector<T, S>::AllocateContinuousBlock(unsigned numElements)
{
    if (numElements < BLOCK_SIZE) {
        DataPtr();
        unsigned rest = BLOCK_SIZE - (size_ & BLOCK_MASK);
        unsigned index;
        if (numElements <= rest) {
            index = size_;
            size_ += numElements;
            return index;
        }

        size_ += rest;
        DataPtr();
        index = size_;
        size_ += numElements;
        return index;
    }
    return -1;
}

template <class T, unsigned S> unsigned PodBvector<T, S>::ByteSize() const
{
    return size_ * sizeof(T);
}

template <class T, unsigned S> void PodBvector<T, S>::Serialize(int8u* ptr) const
{
    unsigned i;
    for (i = 0; i < size_; i++) {
        std::memcpy_s(ptr, sizeof(T), &(*this)[i], sizeof(T));
        ptr += sizeof(T);
    }
}

template <class T, unsigned S> void PodBvector<T, S>::Deserialize(const int8u* data, unsigned byteSize)
{
    RemoveAll();
    byteSize /= sizeof(T);
    for (unsigned i = 0; i < byteSize; ++i) {
        T* ptr = DataPtr();
        std::memcpy_s(ptr, sizeof(T), data, sizeof(T));
        ++size_;
        data += sizeof(T);
    }
}

template <class T, unsigned S>
void PodBvector<T, S>::Deserialize(unsigned start, const T& emptyVal, const int8u* data, unsigned byteSize)
{
    while (size_ < start) {
        Add(emptyVal);
    }

    byteSize /= sizeof(T);
    for (unsigned i = 0; i < byteSize; ++i) {
        if (start + i < size_) {
            std::memcpy_s(&((*this)[start + i]), sizeof(T), data, sizeof(T));
        } else {
            T* ptr = DataPtr();
            std::memcpy_s(ptr, sizeof(T), data, sizeof(T));
            ++size_;
        }
        data += sizeof(T);
    }
}
} // namespace OHOS
