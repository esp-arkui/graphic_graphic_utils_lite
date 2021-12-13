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
 * @brief Defines PodBvector.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_POD_BVECTOR_INCLUDED
#define GRAPHIC_GEOMETRY_POD_BVECTOR_INCLUDED

#include <cstddef>
#include <cstring>

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"
#include "gfx_utils/heap_base.h"
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
        enum BlockScaleEnum {
            BLOCK_SHIFT = S,
            BLOCK_SIZE = 1 << BLOCK_SHIFT,
            BLOCK_MASK = BLOCK_SIZE - 1
        };

        using ValueType = T;

        PodBvector();
        /**
        * @brief 构造一个 PodBvector.
        * @param blockPtrInc 每个块大小
        * @since 1.0
        * @version 1.0
        */
        PodBvector(unsigned blockPtrInc);

        ~PodBvector();

        PodBvector(const PodBvector<T, S>& v);

        const PodBvector<T, S>& operator=(const PodBvector<T, S>& v);
        /**
        * @brief 清空.
        * @since 1.0
        * @version 1.0
        */
        void RemoveAll()
        {
            size_ = 0;
        }
        /**
        * @brief 清空.
        * @since 1.0
        * @version 1.0
        */
        void Clear()
        {
            size_ = 0;
        }
        /**
        * @brief 释放所有元素占用的内存块.
        * @since 1.0
        * @version 1.0
        */
        void FreeAll()
        {
            FreeTail(0);
        }
        /**
        * @brief 修改最后一个元素.
        * @since 1.0
        * @version 1.0
        */
        void ModifyLast(const T& val);
        /**
        * @brief 删除最后一个元素.
        * @since 1.0
        * @version 1.0
        */
        void RemoveLast();
        /**
        * @brief 申请numElements大小的连续内存块.
        * @param numElements 要申请的元素个数
        * @since 1.0
        * @version 1.0
        */
        int AllocateContinuousBlock(unsigned numElements);
        void AllocateBlock(unsigned blockNum);

        /**
        * @brief 从末尾释放元素.
        * @param size 要释放的个数
        * @since 1.0
        * @version 1.0
        */
        void FreeTail(unsigned size);
        /**
        * @brief 增加一个元素.
        * @param val 增加的元素
        * @since 1.0
        * @version 1.0
        */
        void Add(const T& val);
        /**
        * @brief 增加一个元素.
        * @param val 增加的元素
        * @since 1.0
        * @version 1.0
        */
        void PushBack(const T& val)
        {
            Add(val);
        }

        /**
        * @brief 增加一个数组.
        * @param ptr 要增加的数组首地址，numElem 元素个数
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
        * @brief 缩小数组容量.
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
        * @brief 增加数据.
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
        * @brief 获取数组容量.
        * @since 1.0
        * @version 1.0
        */
        unsigned Size() const
        {
            return size_;
        }
        /**
        * @brief 获取指定索引数据块.
        * @since 1.0
        * @version 1.0
        */
        const T& operator[](unsigned index) const
        {
            return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
        }
        /**
        * @brief 获取指定索引数据块.
        * @since 1.0
        * @version 1.0
        */
        T& operator[](unsigned index)
        {
            return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
        }
        /**
        * @brief 获取指定索引数据块.
        * @since 1.0
        * @version 1.0
        */
        T& IndexAt(unsigned index)
        {
            return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
        }
        /**
        * @brief 获取指定索引数据块.
        * @since 1.0
        * @version 1.0
        */
        T ValueAt(unsigned index) const
        {
            return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
        }
        /**
        * @brief 获取当前索引数据块.
        * @since 1.0
        * @version 1.0
        */
        const T& IndexAt(unsigned index) const
        {
            return blocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
        }
        /**
        * @brief 获取当前索引数据块.
        * @since 1.0
        * @version 1.0
        */
        T& Curr(unsigned index)
        {
            return (*this)[index];
        }
        /**
        * @brief 获取当前索引上的前一个数据块.
        * @since 1.0
        * @version 1.0
        */
        const T& Prev(unsigned index) const
        {
            return (*this)[(index + size_ - 1) % size_];
        }
        /**
        * @brief 获取当前索引上的前一个数据块.
        * @since 1.0
        * @version 1.0
        */
        T& Prev(unsigned index)
        {
            return (*this)[(index + size_ - 1) % size_];
        }
        /**
        * @brief 获取当前索引上的数据块.
        * @since 1.0
        * @version 1.0
        */
        const T& Curr(unsigned index) const
        {
            return (*this)[index];
        }

        /**
        * @brief 获取当前索引上的下一个数据块.
        * @since 1.0
        * @version 1.0
        */
        const T& Next(unsigned index) const
        {
            return (*this)[(index + 1) % size_];
        }

        /**
        * @brief 获取当前索引上的下一个数据块.
        * @since 1.0
        * @version 1.0
        */
        T& Next(unsigned index)
        {
            return (*this)[(index + 1) % size_];
        }
        /**
         * @brief 获取末尾数据.
         * @since 1.0
         * @version 1.0
         */
        const T& Last() const
        {
            return (*this)[size_ - 1];
        }

        /**
         * @brief 获取末尾数据.
         * @since 1.0
         * @version 1.0
         */
        T& Last()
        {
            return (*this)[size_ - 1];
        }
        /**
         * @brief 反序列化数据到内存块.
         * @param data 要反序列的数据，byteSize 要序列化的字节数
         * @since 1.0
         * @version 1.0
         */
        void Deserialize(const int8u* data, unsigned byteSize);

        /**
         * @brief 反序列化数据到内存块.
         * @param start 指定位置, 指定位置外的填充数据，data 要反序列的数据，byteSize 要序列化的字节数
         * @since 1.0
         * @version 1.0
         */
        void Deserialize(unsigned start, const T& emptyVal, const int8u* data, unsigned byteSize);
        /**
         * @brief 获取字节数.
         *
         * @since 1.0
         * @version 1.0
         */
        unsigned ByteSize() const;

        /**
         * @brief 序列化数据到指定的地址.
         *
         * @since 1.0
         * @version 1.0
         */
        void Serialize(int8u* ptr) const;

        /**
         * @brief 反序列化数据到内存块.
         * @param start 指定位置, 指定位置外的填充数据，data 要反序列的数据
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
         * @brief 反序列化数据到内存块.
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
         * @brief 获取指定的块地址.
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
         * @brief 申请指定大小的内存块.
         *
         * @since 1.0
         * @version 1.0
         */
        void Allocatelock(unsigned nb);
        T* DataPtr();

        unsigned size_;        // 元素个数
        unsigned numBlocks_;   // 块数量
        unsigned maxBlocks_;   // 最大块数
        T** blocks_;           // 所有块的首地址
        unsigned blockPtrInc_; // 每个块的大小
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
        : size_(v.size_),
          numBlocks_(v.numBlocks_),
          maxBlocks_(v.maxBlocks_),
          blocks_(v.maxBlocks_ ? ArrAllocator<T*>::Allocate(v.maxBlocks_) : 0),
          blockPtrInc_(v.blockPtrInc_)
    {
        unsigned i;
        for (i = 0; i < v.numBlocks_; ++i) {
            blocks_[i] = ArrAllocator<T>::Allocate(BLOCK_SIZE);
            int status = memcpy_s(blocks_[i], BLOCK_SIZE * sizeof(T), v.blocks_[i], BLOCK_SIZE * sizeof(T));
            if (!status)
            {
                printf("%s", "内存出错");
                return;
            }
        }
    }

    template <class T, unsigned S>
    const PodBvector<T, S>& PodBvector<T, S>::operator=(const PodBvector<T, S>& v)
    {
        unsigned i;
        for (i = numBlocks_; i < v.numBlocks_; ++i) {
            AllocateBlock(i);
        }
        for (i = 0; i < v.numBlocks_; ++i) {
            int status = memcpy_s(blocks_[i], BLOCK_SIZE * sizeof(T), v.blocks_[i], BLOCK_SIZE * sizeof(T));
            if (!status)
            {
                printf("%s", "内存出错");
                return ;
            }
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
                int status = memcpy_s(newBlocks, (maxBlocks_ + blockPtrInc_), blocks_, numBlocks_ * sizeof(T*));
                if (!status)
                {
                    printf("%s", "内存出错");
                    return ;
                }
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
        unsigned blockNum = size_ >> BLOCK_SHIFT; // 解释含义
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
            int status = memcpy_s(ptr, sizeof(T), &(*this)[i], sizeof(T));
            if (!status)
            {
                printf("%s", "内存出错");
                return ;
            }
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
            int status = memcpy_s(ptr, sizeof(T), data, sizeof(T));
            if (!status)
            {
                printf("%s", "内存出错");
                return ;
            }
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
        int status;
        for (unsigned i = 0; i < byteSize; ++i) {
            if (start + i < size_) {
                status = memcpy_s(&((*this)[start + i]), sizeof(T), data, sizeof(T));
            } else {
                T* ptr = DataPtr();
                status = memcpy_s(ptr, sizeof(T), data, sizeof(T));
                ++size_;
            }
            data += sizeof(T);
            if (!status)
            {
                printf("%s", "内存出错");
                return ;
            }
        }
    }

} // namespace OHOS
#endif