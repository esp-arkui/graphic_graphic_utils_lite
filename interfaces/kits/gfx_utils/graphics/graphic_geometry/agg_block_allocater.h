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

#ifndef GRAPHIC_GEOMETRY_BLOCK_ALLOCATER_INCLUDED
#define GRAPHIC_GEOMETRY_BLOCK_ALLOCATER_INCLUDED

#include <cstddef>
#include <cstring>
#include "securec.h"
#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"
#include "gfx_utils/heap_base.h"

namespace OHOS {
/**
 * @file agg_block_allocater.h
 *
 * @brief Defines BlockAllocator,申请连续的内存块.
 *
 * @since 1.0
 * @version 1.0
 */
class BlockAllocator : public HeapBase {
    struct BlockType {
        int8u* data;
        unsigned size;
    };

public:
    /**
     *
     * @brief Defines 释放所有内存块.
     *
     * @since 1.0
     * @version 1.0
     */
    void RemoveAll();

    ~BlockAllocator()
    {
        RemoveAll();
    }

    /**
     *
     * @brief Defines 构造一个BlockAllocator.
     * @param blockSize 总大小，blockPtrInc 每一块内存大小
     * @since 1.0
     * @version 1.0
     */
    BlockAllocator(unsigned blockSize, unsigned blockPtrInc = 256 - 8)
        : blockSize_(blockSize),
          blockPtrInc_(blockPtrInc),
          numBlocks_(0),
          maxBlocks_(0),
          blocks_(0),
          bufPtr_(0),
          rest_(0)
    {
    }

    /**
     *
     * @brief Defines 申请内存块.
     * @param size 需要的大小
     * @since 1.0
     * @version 1.0
     */
    int8u* Allocate(unsigned size, unsigned alignment = 1);

private:
    void AllocateBlock(unsigned size);

    unsigned blockSize_;    //总大小
    unsigned blockPtrInc_;  //增长值,每个块的大小
    unsigned numBlocks_;    //块数
    unsigned maxBlocks_;    //最大块数
    BlockType* blocks_;     //每一块的数据类型及大小
    int8u* bufPtr_;         //数据指针
    unsigned rest_;         //重置标志
};

}

#endif
