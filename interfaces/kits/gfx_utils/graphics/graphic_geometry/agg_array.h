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

#ifndef GRAPHIC_GEOMETRY_ARRAY_INCLUDED
#define GRAPHIC_GEOMETRY_ARRAY_INCLUDED

#include <cstddef>
#include <cstring>

#include "gfx_utils/graphics/graphic_common/agg_basics.h"

namespace OHOS {
/**
 * @file graphic_geometry_arc.h
 *
 * @brief Defines 简单数组容器类.
 *
 * @since 1.0
 * @version 1.0
 */
template <class T, unsigned Size>
class PodAutoArray : public HeapBase {
public:
    using ValueType = T;
    using SelfType = PodAutoArray<T, Size>;

    PodAutoArray()
    {}

    explicit PodAutoArray(const T* data)
    {
        std::memcpy(array_, data, sizeof(T) * Size);
    }

    const SelfType& operator=(const T* data)
    {
        std::memcpy(array_, data, sizeof(T) * Size);
        return *this;
    }

    const T& operator[](unsigned index) const
    {
        return array_[index];
    }

    T& operator[](unsigned index)
    {
        return array_[index];
    }

    const T& At(unsigned index) const
    {
        return array_[index];
    }

    T& At(unsigned index)
    {
        return array_[index];
    }

    T ValueAt(unsigned index) const
    {
        return array_[index];
    }

    static unsigned Size()
    {
        return Size;
    }

private:
    T array_[Size];
};

/**
 * @file graphic_geometry_arc.h
 *
 * @brief Defines 简单数组容器适配类.
 *
 * @since 1.0
 * @version 1.0
 */
template <class T>
class PodArrayAdaptor : public HeapBase {
public:
    using ValueType = T;

    PodArrayAdaptor(T* array, unsigned size)
        : array_(array), size_(size)
    {}

    const T& operator[](unsigned index) const
    {
        return array_[index];
    }

    T& operator[](unsigned index)
    {
        return array_[index];
    }

    T ValueAt(unsigned index) const
    {
        return array_[index];
    }

    const T& At(unsigned index) const
    {
        return array_[index];
    }

    T& At(unsigned index)
    {
        return array_[index];
    }

    unsigned Size() const
    {
        return size_;
    }

private:
    T* array_;
    unsigned size_;
};

/**
 * @file graphic_geometry_arc.h
 *
 * @brief Defines 向量容器类,提供了清除,增加了增加，插入等方法.
 *
 * @since 1.0
 * @version 1.0
 */
template <class T, unsigned Size>
class PodAutoVector : public HeapBase {
public:
    using ValueType = T;
    using SelfType = PodAutoVector<T, Size>;

    PodAutoVector()
        : size_(0)
    {}

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
        array_[size_++] = val;
    }

    void IncSize(unsigned size)
    {
        array_ += size;
    }

    void PushBack(const T& val)
    {
        array_[size_++] = val;
    }

    const T& operator[](unsigned index) const
    {
        return array_[index];
    }

    T& operator[](unsigned index)
    {
        return array_[index];
    }

    const T& At(unsigned index) const
    {
        return array_[index];
    }

    T& At(unsigned index)
    {
        return array_[index];
    }

    T ValueAt(unsigned index) const
    {
        return array_[index];
    }

    unsigned Size() const
    {
        return size_;
    }

private:
    T array_[Size];
    unsigned size_;
};

/**
 * @file graphic_geometry_arc.h
 *
 * @brief Defines 简单数组容器类.
 *
 * @since 1.0
 * @version 1.0
 */
template <class T>
class PodArray : public HeapBase {
public:
    using ValueType = T;
    using SelfType = PodArray<T>;

    ~PodArray()
    {
        ArrAllocator<T>::Deallocate(array_, size_);
    }

    PodArray()
        : array_(0), size_(0)
    {}

    PodArray(unsigned size)
        : array_(ArrAllocator<T>::Allocate(size)), size_(size)
    {}

    PodArray(const SelfType& v)
        : array_(ArrAllocator<T>::Allocate(v.size_)), size_(v.size_)
    {
        std::memcpy(array_, v.array_, sizeof(T) * size_);
    }

    const SelfType& operator=(const SelfType& v)
    {
        Resize(v.size());
        std::memcpy(array_, v.array_, sizeof(T) * size_);
        return *this;
    }

    const T& operator[](unsigned index) const
    {
        return array_[index];
    }

    T& operator[](unsigned index)
    {
        return array_[index];
    }

    T ValueAt(unsigned index) const
    {
        return array_[index];
    }

    const T& At(unsigned index) const
    {
        return array_[index];
    }

    T& At(unsigned index)
    {
        return array_[index];
    }

    const T* Data() const
    {
        return array_;
    }

    T* Data()
    {
        return array_;
    }

    void Resize(unsigned size)
    {
        if (size != size_) {
            pod_allocator<T>::Deallocate(array_, size_);
            array_ = ArrAllocator<T>::Allocate(size_ = size);
        }
    }

    unsigned Size() const
    {
        return size_;
    }

private:
    T* array_;
    unsigned size_;
};

template <class T>
class PodVector : public HeapBase {
public:
    using ValueType = T;

    PodVector()
        : size_(0), capacity_(0), array_(0)
    {}

    PodVector(unsigned cap, unsigned extraTail = 0);

    PodVector(const PodVector<T>&);

    ~PodVector()
    {
        ArrAllocator<T>::Deallocate(array_, capacity_);
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
        std::memset(array_, 0, sizeof(T) * size_);
    }

    void Add(const T& val)
    {
        array_[size_++] = val;
    }

    void PushBack(const T& val)
    {
        array_[size_++] = val;
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
        return array_[index];
    }

    T& operator[](unsigned index)
    {
        return array_[index];
    }

    const T& At(unsigned index) const
    {
        return array_[index];
    }

    T& At(unsigned index)
    {
        return array_[index];
    }

    T ValueAt(unsigned index) const
    {
        return array_[index];
    }

    const T* Data() const
    {
        return array_;
    }

    T* Data()
    {
        return array_;
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
    T* array_;
};

template <class T>
void PodVector<T>::Capacity(unsigned cap, unsigned extraTail)
{
    size_ = 0;
    if (cap > capacity_) {
        ArrAllocator<T>::Deallocate(array_, capacity_);
        capacity_ = cap + extraTail;
        array_ = capacity_ ? ArrAllocator<T>::Allocate(capacity_) : 0;
    }
}

template <class T>
void PodVector<T>::Allocate(unsigned size, unsigned extraTail)
{
    capacity(size, extraTail);
    size_ = size;
}

template <class T>
void PodVector<T>::Resize(unsigned newSize)
{
    if (newSize > size_) {
        if (newSize > capacity_) {
            T* data = ArrAllocator<T>::Allocate(newSize);
            std::memcpy(data, array_, size_ * sizeof(T));
            ArrAllocator<T>::Deallocate(array_, capacity_);
            array_ = data;
        }
    } else {
        size_ = newSize;
    }
}

template <class T>
PodVector<T>::PodVector(unsigned cap, unsigned extraTail)
    : size_(0), capacity_(cap + extraTail), array_(ArrAllocator<T>::Allocate(capacity_))
{}

template <class T>
PodVector<T>::PodVector(const PodVector<T>& v)
    : size_(v.size_), capacity_(v.capacity_), array_(v.capacity_ ? ArrAllocator<T>::Allocate(v.capacity_) : 0)
{
    std::memcpy(array_, v.array_, sizeof(T) * v.size_);
}

template <class T>
const PodVector<T>&
PodVector<T>::operator=(const PodVector<T>& val)
{
    Allocate(val.size_);
    if (val.size_) {
        std::memcpy(array_, v.array_, sizeof(T) * val.size_);
    }
    return *this;
}

template <class T>
void PodVector<T>::Deserialize(const int8u* data, unsigned byteSize)
{
    byteSize = byteSize / sizeof(T);
    Allocate(byteSize);
    if (byteSize) {
        std::memcpy(array_, data, byteSize * sizeof(T));
    }
}

template <class T>
void PodVector<T>::Serialize(int8u* ptr) const
{
    if (size_) {
        std::memcpy(ptr, array_, size_ * sizeof(T));
    }
}

template <class T>
void PodVector<T>::InsertAt(unsigned pos, const T& val)
{
    if (pos >= size_) {
        array_[size_] = val;
    } else {
        std::memmove(array_ + pos + 1, array_ + pos, (size_ - pos) * sizeof(T));
        array_[pos] = val;
    }
    ++size_;
}

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

    template <class DataAccessor>
    void AddData(DataAccessor& data)
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

    T& At(unsigned index)
    {
        return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
    }

    T ValueAt(unsigned index) const
    {
        return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
    }

    const T& At(unsigned index) const
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

    void Deserialize(unsigned start, const T& emptyVal,
                     const int8u* data, unsigned byteSize);

    unsigned ByteSize() const;

    void Serialize(int8u* ptr) const;

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

template <class T, unsigned S>
PodBvector<T, S>::~PodBvector()
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

template <class T, unsigned S>
void PodBvector<T, S>::FreeTail(unsigned size)
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
PodBvector<T, S>::PodBvector()
    : size_(0), numBlocks_(0), maxBlocks_(0), blocks_(0), blockPtrInc_(BLOCK_SIZE)
{
}

template <class T, unsigned S>
PodBvector<T, S>::PodBvector(unsigned blockPtrInc)
    : size_(0), numBlocks_(0), maxBlocks_(0), blocks_(0), blockPtrInc_(blockPtrInc)
{
}

template <class T, unsigned S>
PodBvector<T, S>::PodBvector(const PodBvector<T, S>& v)
    : size_(v.size_), numBlocks_(v.numBlocks_), maxBlocks_(v.maxBlocks_), blocks_(v.maxBlocks_ ? ArrAllocator<T*>::Allocate(v.maxBlocks_) : 0), blockPtrInc_(v.blockPtrInc_)
{
    unsigned i;
    for (i = 0; i < v.numBlocks_; ++i) {
        blocks_[i] = ArrAllocator<T>::Allocate(BLOCK_SIZE);
        std::memcpy(blocks_[i], v.blocks_[i], BLOCK_SIZE * sizeof(T));
    }
}

template <class T, unsigned S>
const PodBvector<T, S>&
PodBvector<T, S>::operator=(const PodBvector<T, S>& v)
{
    unsigned i;
    for (i = numBlocks_; i < v.numBlocks_; ++i) {
        AllocateBlock(i);
    }
    for (i = 0; i < v.numBlocks_; ++i) {
        std::memcpy(blocks_[i], v.blocks_[i], BLOCK_SIZE * sizeof(T));
    }
    size_ = v.size_;
    return *this;
}

template <class T, unsigned S>
void PodBvector<T, S>::AllocateBlock(unsigned blockNum)
{
    if (blockNum >= maxBlocks_) {
        T** newBlocks = ArrAllocator<T*>::Allocate(maxBlocks_ + blockPtrInc_);

        if (blocks_) {
            std::memcpy(newBlocks, blocks_, numBlocks_ * sizeof(T*));

            ArrAllocator<T*>::Deallocate(blocks_, maxBlocks_);
        }
        blocks_ = newBlocks;
        maxBlocks_ += blockPtrInc_;
    }
    blocks_[blockNum] = ArrAllocator<T>::Allocate(BLOCK_SIZE);
    numBlocks_++;
}

template <class T, unsigned S>
inline T* PodBvector<T, S>::DataPtr()
{
    unsigned blockNum = size_ >> BLOCK_SHIFT;
    if (blockNum >= numBlocks_) {
        AllocateBlock(blockNum);
    }
    return blocks_[blockNum] + (size_ & BLOCK_MASK);
}

template <class T, unsigned S>
inline void PodBvector<T, S>::Add(const T& val)
{
    *DataPtr() = val;
    ++size_;
}

template <class T, unsigned S>
inline void PodBvector<T, S>::RemoveLast()
{
    if (size_) {
        --size_;
    }
}

template <class T, unsigned S>
void PodBvector<T, S>::ModifyLast(const T& val)
{
    RemoveLast();
    Add(val);
}

template <class T, unsigned S>
int PodBvector<T, S>::AllocateContinuousBlock(unsigned numElements)
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

template <class T, unsigned S>
unsigned PodBvector<T, S>::ByteSize() const
{
    return size_ * sizeof(T);
}

template <class T, unsigned S>
void PodBvector<T, S>::Serialize(int8u* ptr) const
{
    unsigned i;
    for (i = 0; i < size_; i++) {
        std::memcpy(ptr, &(*this)[i], sizeof(T));
        ptr += sizeof(T);
    }
}

template <class T, unsigned S>
void PodBvector<T, S>::Deserialize(const int8u* data, unsigned byteSize)
{
    RemoveAll();
    byteSize /= sizeof(T);
    for (unsigned i = 0; i < byteSize; ++i) {
        T* ptr = DataPtr();
        std::memcpy(ptr, data, sizeof(T));
        ++size_;
        data += sizeof(T);
    }
}

template <class T, unsigned S>
void PodBvector<T, S>::Deserialize(unsigned start, const T& emptyVal,
                                   const int8u* data, unsigned byteSize)
{
    while (size_ < start) {
        Add(emptyVal);
    }

    byteSize /= sizeof(T);
    for (unsigned i = 0; i < byteSize; ++i) {
        if (start + i < size_) {
            std::memcpy(&((*this)[start + i]), data, sizeof(T));
        } else {
            T* ptr = DataPtr();
            std::memcpy(ptr, data, sizeof(T));
            ++size_;
        }
        data += sizeof(T);
    }
}

class BlockAllocator : public HeapBase {
    struct BlockType {
        int8u* data;
        unsigned size;
    };

public:
    void RemoveAll()
    {
        if (numBlocks_) {
            BlockType* blk = blocks_ + numBlocks_ - 1;
            while (numBlocks_--) {
                ArrAllocator<int8u>::Deallocate(blk->data, blk->size);
                --blk;
            }
            ArrAllocator<BlockType>::Deallocate(blocks_, maxBlocks_);
        }
        numBlocks_ = 0;
        maxBlocks_ = 0;
        blocks_ = 0;
        bufPtr_ = 0;
        rest_ = 0;
    }

    ~BlockAllocator()
    {
        RemoveAll();
    }

    BlockAllocator(unsigned blockSize, unsigned blockPtrInc = 256 - 8)
        : blockSize_(blockSize), blockPtrInc_(blockPtrInc), numBlocks_(0), maxBlocks_(0), blocks_(0), bufPtr_(0), rest_(0)
    {
    }

    int8u* Allocate(unsigned size, unsigned alignment = 1)
    {
        if (size == 0) {
            return 0;
        }
        if (size <= rest_) {
            int8u* ptr = bufPtr_;
            if (alignment > 1) {
                unsigned align = (alignment - unsigned((std::size_t)ptr) % alignment) % alignment;

                size += align;
                ptr += align;
                if (size <= rest_) {
                    rest_ -= size;
                    bufPtr_ += size;
                    return ptr;
                }
                AllocateBlock(size);
                return Allocate(size - align, alignment);
            }
            rest_ -= size;
            bufPtr_ += size;
            return ptr;
        }
        AllocateBlock(size + alignment - 1);
        return Allocate(size, alignment);
    }

private:
    void AllocateBlock(unsigned size)
    {
        if (size < blockSize_) {
            size = blockSize_;
        }
        if (numBlocks_ >= maxBlocks_) {
            BlockType* newBlocks = ArrAllocator<BlockType>::Allocate(maxBlocks_ + blockPtrInc_);

            if (blocks_) {
                std::memcpy(newBlocks, blocks_, numBlocks_ * sizeof(BlockType));
                ArrAllocator<BlockType>::Deallocate(blocks_, maxBlocks_);
            }
            blocks_ = newBlocks;
            maxBlocks_ += blockPtrInc_;
        }

        blocks_[numBlocks_].size = size;
        blocks_[numBlocks_].data =
            bufPtr_ =
                ArrAllocator<int8u>::Allocate(size);

        numBlocks_++;
        rest_ = size;
    }

    unsigned blockSize_;
    unsigned blockPtrInc_;
    unsigned numBlocks_;
    unsigned maxBlocks_;
    BlockType* blocks_;
    int8u* bufPtr_;
    unsigned rest_;
};

enum QuickSortThresholdEnum
{
    QUICK_SORT_THRESHOLD = 9
};

template <class T>
inline void SwapElements(T& val1, T& val2)
{
    T t = val1;
    val1 = val2;
    val2 = t;
}

template <class Array, class Less>
void QuickSort(Array& arr, Less less)
{
    if (arr.Size() < 2) {
        return;
    }
    typename Array::ValueType* e1;
    typename Array::ValueType* e2;
    int stack[80];
    int* top = stack;
    int limit = arr.Size();
    int base = 0;
    for (;;) {
        int len = limit - base;
        int i;
        int j;
        int pivot;
        if (len > QUICK_SORT_THRESHOLD) {
            pivot = base + len / 2;
            SwapElements(arr[base], arr[pivot]);
            i = base + 1;
            j = limit - 1;
            e1 = &(arr[j]);
            e2 = &(arr[i]);
            if (less(*e1, *e2)) {
                SwapElements(*e1, *e2);
            }
            e1 = &(arr[base]);
            e2 = &(arr[i]);
            if (less(*e1, *e2)) {
                SwapElements(*e1, *e2);
            }
            e1 = &(arr[j]);
            e2 = &(arr[base]);
            if (less(*e1, *e2))
                SwapElements(*e1, *e2);
            for (;;) {
                do
                    i++;
                while (less(arr[i], arr[base]));
                do
                    j--;
                while (less(arr[base], arr[j]));

                if (i > j) {
                    break;
                }
                SwapElements(arr[i], arr[j]);
            }
            SwapElements(arr[base], arr[j]);
            if (j - base > limit - i) {
                top[0] = base;
                top[1] = j;
                base = i;
            } else {
                top[0] = i;
                top[1] = limit;
                limit = j;
            }
            top += 2;
        } else {
            j = base;
            i = j + 1;
            for (; i < limit; j = i, i++) {
                for (; less(*(e1 = &(arr[j + 1])), *(e2 = &(arr[j]))); j--) {
                    Swapelements(*e1, *e2);
                    if (j == base) {
                        break;
                    }
                }
            }
            if (top > stack) {
                top -= 2;
                base = top[0];
                limit = top[1];
            } else {
                break;
            }
        }
    }
}

template <class Array, class Equal>
unsigned RemoveDuplicates(Array& arr, Equal equal)
{
    if (arr.Size() < 2) {
        return arr.Size();
    }

    unsigned i, j;
    for (i = 1, j = 1; i < arr.Size(); i++) {
        typename Array::ValueType& e = arr[i];
        if (!equal(e, arr[i - 1])) {
            arr[j++] = e;
        }
    }
    return j;
}

template <class Array>
void InvertContainer(Array& arr)
{
    int i = 0;
    int j = arr.size() - 1;
    while (i < j) {
        SwapElements(arr[i++], arr[j--]);
    }
}

template <class Array, class Value, class Less>
unsigned BinarySearchPos(const Array& arrData, const Value& val, Less less)
{
    if (arrData.size() == 0) {
        return 0;
    }

    unsigned begin = 0;
    unsigned end = arrData.size() - 1;

    if (less(val, arrData[0])) {
        return 0;
    }
    if (less(arrData[end], val)) {
        return end + 1;
    }

    while (end - begin > 1) {
        unsigned mid = (end + begin) >> 1;
        if (less(val, arrData[mid])) {
            end = mid;
        } else {
            begin = mid;
        }
    }

    return end;
}

template <class Array>
class RangeAdaptor : public HeapBase {
public:
    using ValueType = typename Array::ValueType;

    range_adaptor(Array& array, unsigned start, unsigned size)
        : array_(array), start_(start), size_(size)
    {}

    unsigned Size() const
    {
        return size_;
    }

    const ValueType& operator[](unsigned i) const
    {
        return array_[start_ + i];
    }

    ValueType& operator[](unsigned i)
    {
        return array_[start_ + i];
    }

    const ValueType& At(unsigned i) const
    {
        return array_[start_ + i];
    }

    ValueType& At(unsigned i)
    {
        return array_[start_ + i];
    }

    ValueType ValueAt(unsigned i) const
    {
        return array_[start_ + i];
    }

private:
    Array& array_;
    unsigned start_;
    unsigned size_;
};

inline bool IntLess(int a, int b)
{
    return a < b;
}

inline bool IntGreater(int a, int b)
{
    return a > b;
}

inline bool UnsignedLess(unsigned a, unsigned b)
{
    return a < b;
}

inline bool UnsignedGreater(unsigned a, unsigned b)
{
    return a > b;
}
} // namespace OHOS

#endif
