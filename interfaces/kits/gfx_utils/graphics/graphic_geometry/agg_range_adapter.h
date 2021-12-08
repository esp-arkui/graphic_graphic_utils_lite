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
#include "securec.h"
#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "heap_base.h"

namespace OHOS {

enum QuickSortThresholdEnum { QUICK_SORT_THRESHOLD = 9 };

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

template <class T> inline void SwapElements(T& val1, T& val2)
{
    T t = val1;
    val1 = val2;
    val2 = t;
}

template <class Array, class Less> void QuickSort(Array& arr, Less less)
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

template <class Array, class Equal> unsigned RemoveDuplicates(Array& arr, Equal equal)
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

template <class Array> void InvertContainer(Array& arr)
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

template <class Array> class RangeAdaptor : public HeapBase {
public:
    using ValueType = typename Array::ValueType;

    RangeAdaptor(Array& array, unsigned start, unsigned size) : data_(array), start_(start), size_(size) {}

    unsigned Size() const
    {
        return size_;
    }

    const ValueType& operator[](unsigned i) const
    {
        return data_[start_ + i];
    }

    ValueType& operator[](unsigned i)
    {
        return data_[start_ + i];
    }

    const ValueType& IndexAt(unsigned i) const
    {
        return data_[start_ + i];
    }

    ValueType& IndexAt(unsigned i)
    {
        return data_[start_ + i];
    }

    ValueType ValueAt(unsigned i) const
    {
        return data_[start_ + i];
    }

private:
    Array& data_;
    unsigned start_;
    unsigned size_;
};


}
