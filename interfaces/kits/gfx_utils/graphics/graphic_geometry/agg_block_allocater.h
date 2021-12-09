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
#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "heap_base.h"

namespace OHOS {
/**
 * @file agg_block_allocater.h
 *
 * @brief Defines BlockAllocator,�����������ڴ��.
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
     * @brief Defines �ͷ������ڴ��.
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
     * @brief Defines ����һ��BlockAllocator.
     * @param blockSize �ܴ�С��blockPtrInc ÿһ���ڴ�ָ�������ֵ
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
     * @brief Defines �����ڴ��.
     * @param size ��Ҫ�Ĵ�С
     * @since 1.0
     * @version 1.0
     */
    int8u* Allocate(unsigned size, unsigned alignment = 1);

private:
    void AllocateBlock(unsigned size);

    unsigned blockSize_;    //�ܴ�С
    unsigned blockPtrInc_;  //����ֵ,ÿ����Ĵ�С
    unsigned numBlocks_;    //����
    unsigned maxBlocks_;    //������
    BlockType* blocks_;     //ÿһ����������ͼ���С
    int8u* bufPtr_;         //����ָ��
    unsigned rest_;         //���ñ�־
};

}

#include "agg_block_allocater.cpp"