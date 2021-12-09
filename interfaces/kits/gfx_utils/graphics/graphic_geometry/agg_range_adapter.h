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

#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "heap_base.h"
#include "securec.h"

namespace OHOS {

enum QuickSortThresholdEnum
{
    QUICK_SORT_THRESHOLD = 9
};

/**
*
* @brief ��һ�����Ƿ���ڵڶ�����.
* @since 1.0
* @version 1.0
*/
inline bool IntLess(int a, int b)
{
    return a < b;
}
/**
*
* @brief ��һ�����Ƿ���ڵڶ�����.
* @since 1.0
* @version 1.0
*/
inline bool IntGreater(int a, int b)
{
    return a > b;
}
/**
*
* @brief ��һ�����Ƿ�С�ڵڶ�����.
* @since 1.0
* @version 1.0
*/
inline bool UnsignedLess(unsigned a, unsigned b)
{
    return a < b;
}

/**
*
* @brief ��һ�����Ƿ���ڵڶ�����.
* @since 1.0
* @version 1.0
*/
inline bool UnsignedGreater(unsigned a, unsigned b)
{
    return a > b;
}
/**
*
* @brief ��������.
* @since 1.0
* @version 1.0
*/
template <class T>
inline void SwapElements(T& val1, T& val2)
{
    T t = val1;
    val1 = val2;
    val2 = t;
}
/**
*
* @brief ���������㷨.
* @param arr Ҫ���������,less ��������
* @since 1.0
* @version 1.0
*/
template <class Array, class Less>
void QuickSort(Array& arr, Less less);

/**
*
* @brief ����ָ������ָ������ɾ���ظ���Ԫ��.
* 
* @param arr ָ������,equal ָ������
* @since 1.0
* @version 1.0
*/
template <class Array, class Equal>
unsigned RemoveDuplicates(Array& arr, Equal equal);

/**
*
* @brief ��תһ������.
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
* @brief ���ַ������㷨.
*
* @since 1.0
* @version 1.0
*/
template <class Array, class Value, class Less>
unsigned BinarySearchPos(const Array& arrData, const Value& val, Less less);

/**
*
* @brief ��array��ָ��λ������뵽RangeAdaptor.
* @since 1.0
* @version 1.0
*/
template <class Array>
class RangeAdaptor : public HeapBase {
public:
    using ValueType = typename Array::ValueType;
    /**
    *
    * @brief ʹ��һ�����鹹��RangeAdaptor.
    * @param array ָ������,start ָ����ʼλ��, size ָ����С
    * @since 1.0
    * @version 1.0
    */
    RangeAdaptor(Array& array, unsigned start, unsigned size)
        : data_(array), start_(start), size_(size)
    {}
    /**
    *
    * @brief ��ȡָ���Ĵ�С.
    * @since 1.0
    * @version 1.0
    */
    unsigned Size() const
    {
        return size_;
    }

    /**
    *
    * @brief ��ȡָ����������Ԫ��.
    * @since 1.0
    * @version 1.0
    */
    const ValueType& operator[](unsigned i) const
    {
        return data_[start_ + i];
    }

    /**
    *
    * @brief ��ȡָ����������Ԫ��.
    * @since 1.0
    * @version 1.0
    */
    ValueType& operator[](unsigned i)
    {
        return data_[start_ + i];
    }

    /**
    *
    * @brief ��ȡָ����������Ԫ��.
    * @since 1.0
    * @version 1.0
    */
    const ValueType& IndexAt(unsigned i) const
    {
        return data_[start_ + i];
    }
    /**
    *
    * @brief ��ȡָ����������Ԫ��.
    * @since 1.0
    * @version 1.0
    */
    ValueType& IndexAt(unsigned i)
    {
        return data_[start_ + i];
    }
    /**
    *
    * @brief ��ȡָ����������Ԫ��.
    * @since 1.0
    * @version 1.0
    */
    ValueType ValueAt(unsigned i) const
    {
        return data_[start_ + i];
    }

private:
    Array& data_;   //ԭ����
    unsigned start_;//ԭ�������ʼλ��
    unsigned size_; //ָ���Ĵ�С
};

} // namespace OHOS
