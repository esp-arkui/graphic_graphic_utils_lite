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

#include <gfx_utils/graphics/graphic_geometry/agg_block_allocater.h>

#include <cmath>

namespace OHOS {
void BlockAllocator::RemoveAll()
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

int8u* BlockAllocator::Allocate(unsigned size, unsigned alignment = 1)
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

void BlockAllocator::AllocateBlock(unsigned size)
{
    if (size < blockSize_) {
        size = blockSize_;
    }
    if (numBlocks_ >= maxBlocks_) {
        BlockType* newBlocks = ArrAllocator<BlockType>::Allocate(maxBlocks_ + blockPtrInc_);

        if (blocks_) {
            std::memcpy_s(newBlocks, (maxBlocks_ + blockPtrInc_), blocks_, numBlocks_ * sizeof(BlockType));
            ArrAllocator<BlockType>::Deallocate(blocks_, maxBlocks_);
        }
        blocks_ = newBlocks;
        maxBlocks_ += blockPtrInc_;
    }

    blocks_[numBlocks_].size = size;
    blocks_[numBlocks_].data = bufPtr_ = ArrAllocator<int8u>::Allocate(size);

    numBlocks_++;
    rest_ = size;
}
} // namespace OHOS
