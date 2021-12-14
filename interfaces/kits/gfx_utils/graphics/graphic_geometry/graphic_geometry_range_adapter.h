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

#ifndef GRAPHIC_GEOMETRY_RANGE_ADAPTER_INCLUDED
#define GRAPHIC_GEOMETRY_RANGE_ADAPTER_INCLUDED

#include <cstddef>
#include <cstring>

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"
#include "gfx_utils/heap_base.h"
#include "securec.h"

namespace OHOS {

    const int QUICK_SORT_THRESHOLD = 9;

    /**
    *
    * @brief 第一个数是否小于第二个数.
    * @since 1.0
    * @version 1.0
    */
    inline bool IntLess(int a, int b)
    {
        return a < b;
    }
    /**
    *
    * @brief 第一个数是否大于第二个数.
    * @since 1.0
    * @version 1.0
    */
    inline bool IntGreater(int a, int b)
    {
        return a > b;
    }
    /**
    *
    * @brief 第一个数是否小于第二个数.
    * @since 1.0
    * @version 1.0
    */
    inline bool UnsignedLess(unsigned a, unsigned b)
    {
        return a < b;
    }

    /**
    *
    * @brief 第一个数是否大于第二个数.
    * @since 1.0
    * @version 1.0
    */
    inline bool UnsignedGreater(unsigned a, unsigned b)
    {
        return a > b;
    }
    /**
    *
    * @brief 交换数据.
    * @since 1.0
    * @version 1.0
    */
    template <class T>
    inline void SwapElements(T& val1, T& val2)
    {
        T temp = val1;
        val1 = val2;
        val2 = temp;
    }
    /**
    *
    * @brief 快速排序算法.
    * @param arr 要排序的数组,less 排序条件
    * @since 1.0
    * @version 1.0
    */
    template <class Array, class Less>
    void QuickSort(Array& arr, Less less);

    /**
    *
    * @brief 根据指定条件指定条件删除重复的元素.
    *
    * @param arr 指定数组,equal 指定条件
    * @since 1.0
    * @version 1.0
    */
    template <class Array, class Equal>
    unsigned RemoveDuplicates(Array& arr, Equal equal);

    /**
    *
    * @brief 反转一个数组.
    *
    * @since 1.0
    * @version 1.0
    */
    template <class Array>
    void InvertContainer(Array& arr)
    {
        int i = 0;
        int j = arr.size() - 1;
        while (i < j) {
            SwapElements(arr[i++], arr[j--]);
        }
    }

    /**
    *
    * @brief 二分法查找算法.
    *
    * @since 1.0
    * @version 1.0
    */
    template <class Array, class Value, class Less>
    unsigned BinarySearchPos(const Array& arrData, const Value& val, Less less);

    /**
    *
    * @brief 从array的指定位置起放入到RangeAdaptor.
    * @since 1.0
    * @version 1.0
    */
    template <class Array>
    class RangeAdaptor : public HeapBase {
    public:
        using ValueType = typename Array::ValueType;
        /**
        *
        * @brief 使用一个数组构造RangeAdaptor.
        * @param array 指定数组,start 指定起始位置, size 指定大小
        * @since 1.0
        * @version 1.0
        */
        RangeAdaptor(Array& array, unsigned start, unsigned size) :
            data_(array), start_(start), size_(size)
        {}
        /**
        *
        * @brief 获取指定的大小.
        * @since 1.0
        * @version 1.0
        */
        unsigned Size() const
        {
            return size_;
        }

        /**
        *
        * @brief 获取指定的索引的元素.
        * @since 1.0
        * @version 1.0
        */
        const ValueType& operator[](unsigned index) const
        {
            return data_[start_ + index];
        }

        /**
        *
        * @brief 获取指定的索引的元素.
        * @since 1.0
        * @version 1.0
        */
        ValueType& operator[](unsigned index)
        {
            return data_[start_ + index];
        }

        /**
        *
        * @brief 获取指定的索引的元素.
        * @since 1.0
        * @version 1.0
        */
        const ValueType& IndexAt(unsigned index) const
        {
            return data_[start_ + index];
        }
        /**
        *
        * @brief 获取指定的索引的元素.
        * @since 1.0
        * @version 1.0
        */
        ValueType& IndexAt(unsigned index)
        {
            return data_[start_ + index];
        }
        /**
        *
        * @brief 获取指定的索引的元素.
        * @since 1.0
        * @version 1.0
        */
        ValueType ValueAt(unsigned index) const
        {
            return data_[start_ + index];
        }

    private:
        Array& data_;    //原数组
        unsigned start_; //原数组的起始位置
        unsigned size_;  //指定的大小
    };

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
                        SwapElements(*e1, *e2);
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

} // namespace OHOS
#endif
