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
 * @brief Defines Arc
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
    * @brief 交换数据.
    * @since 1.0
    * @version 1.0
    */
    template <class T>
    inline void SwapElements(T& firstValue, T& secondValue)
    {
        T temp = firstValue;
        firstValue = secondValue;
        secondValue = temp;
    }

    template <class Array, class Less>
    void ToCompareLess(Array& arr, Less less, int iIndex, int jIndex, int base)
    {
        while (1) {
            do {
                iIndex++;
            } while (less(arr[iIndex], arr[base]));
            do {
                jIndex--;
            } while (less(arr[base], arr[jIndex]));

            if (iIndex > jIndex) {
                break;
            }
            SwapElements(arr[iIndex], arr[jIndex]);
        }
    }

    template <class Array, class Less>
    bool CompareLessThreshold(Array& arr, Less less, int iIndex, int jIndex, int base, int limit, int* top, int* stack)
    {
        typename Array::ValueType* e1;
        typename Array::ValueType* e2;
        jIndex = base;
        iIndex = jIndex + 1;
        for (; iIndex < limit; iIndex++) {
            for (; less(*(e1 = &(arr[jIndex + 1])), *(e2 = &(arr[jIndex]))); jIndex--) {
                SwapElements(*e1, *e2);
                if (jIndex == base) {
                    break;
                }
            }
            jIndex = iIndex;
        }
        if (top > stack) {
            top -= TWO_STEP;
            base = top[0];
            limit = top[1];
        } else {
            return true;
        }
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
        int iIndex = 0;
        int jIndex = arr.size() - 1;
        while (iIndex < jIndex) {
            SwapElements(arr[iIndex++], arr[jIndex--]);
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

    template <class Array, class Less>
    void QuickSort(Array& arr, Less less)
    {
        if (arr.Size() < INDEX_TWO) {
            return;
        }
        typename Array::ValueType* firstElement;
        typename Array::ValueType* secondElement;
        int stack[80];
        int* top = stack;
        int limit = arr.Size();
        int baseLimit = 0;
        while (1) {
            int len = limit - baseLimit;
            int iIndex;
            int jIndex;
            int pivot;
            if (len > QUICK_SORT_THRESHOLD) {
                pivot = baseLimit + len * HALFNUM;
                SwapElements(arr[baseLimit], arr[pivot]);
                iIndex = baseLimit + 1;
                jIndex = limit - 1;
                firstElement = &(arr[jIndex]);
                secondElement = &(arr[iIndex]);
                if (less(*firstElement, *secondElement)) {
                    SwapElements(*firstElement, *secondElement);
                }
                firstElement = &(arr[baseLimit]);
                secondElement = &(arr[iIndex]);
                if (less(*firstElement, *secondElement)) {
                    SwapElements(*firstElement, *secondElement);
                }
                firstElement = &(arr[jIndex]);
                secondElement = &(arr[baseLimit]);
                if (less(*firstElement, *secondElement))
                    SwapElements(*firstElement, *secondElement);
                ToCompareLess(arr, less, iIndex, jIndex, baseLimit);
                SwapElements(arr[baseLimit], arr[jIndex]);
                if (jIndex - baseLimit > limit - iIndex) {
                    top[0] = baseLimit;
                    top[1] = jIndex;
                    baseLimit = iIndex;
                } else {
                    top[0] = iIndex;
                    top[1] = limit;
                    limit = jIndex;
                }
                top += TWO_STEP;
            } else {
                if (CompareLessThreshold(arr, less, iIndex, jIndex, baseLimit, limit, top, stack)) {
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

        unsigned arrayBegin = 0;
        unsigned arrayEnd = arrData.size() - 1;

        if (less(val, arrData[0])) {
            return 0;
        }
        if (less(arrData[arrayEnd], val)) {
            return arrayEnd + 1;
        }

        while (arrayEnd - arrayBegin > 1) {
            unsigned mid = (arrayEnd + arrayBegin) >> 1;
            if (less(val, arrData[mid])) {
                arrayEnd = mid;
            } else {
                arrayBegin = mid;
            }
        }

        return arrayEnd;
    }

    template <class Array, class Equal>
    unsigned RemoveDuplicates(Array& arr, Equal equal)
    {
        if (arr.Size() < INDEX_TWO) {
            return arr.Size();
        }

        unsigned iIndex, jIndex;
        for (iIndex = 1, jIndex = 1; iIndex < arr.Size(); iIndex++) {
            typename Array::ValueType& element = arr[iIndex];
            if (!equal(element, arr[iIndex - 1])) {
                arr[jIndex++] = element;
            }
        }
        return jIndex;
    }
} // namespace OHOS
#endif
