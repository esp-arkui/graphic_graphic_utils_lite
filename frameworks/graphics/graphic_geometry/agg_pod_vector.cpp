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

#include <gfx_utils/graphics/graphic_geometry/agg_pod_vector.h>

#include <cmath>

namespace OHOS {

template <class T> void PodVector<T>::Capacity(unsigned cap, unsigned extraTail)
{
    size_ = 0;
    if (cap > capacity_) {
        ArrAllocator<T>::Deallocate(data_, capacity_);
        capacity_ = cap + extraTail;
        data_ = capacity_ ? ArrAllocator<T>::Allocate(capacity_) : 0;
    }
}

template <class T> void PodVector<T>::Allocate(unsigned size, unsigned extraTail)
{
    capacity(size, extraTail);
    size_ = size;
}

template <class T> void PodVector<T>::Resize(unsigned newSize)
{
    if (newSize > size_) {
        if (newSize > capacity_) {
            T* data = ArrAllocator<T>::Allocate(newSize);
            std::memcpy_s(data, newSize, data_, size_ * sizeof(T));
            ArrAllocator<T>::Deallocate(data_, capacity_);
            data_ = data;
        }
    } else {
        size_ = newSize;
    }
}

template <class T>
PodVector<T>::PodVector(unsigned cap, unsigned extraTail)
    : size_(0), capacity_(cap + extraTail), data_(ArrAllocator<T>::Allocate(capacity_))
{
}

template <class T>
PodVector<T>::PodVector(const PodVector<T>& v)
    : size_(v.size_), capacity_(v.capacity_), data_(v.capacity_ ? ArrAllocator<T>::Allocate(v.capacity_) : 0)
{
    std::memcpy_s(data_, sizeof(T) * v.size_, v.data_, sizeof(T) * v.size_);
}

template <class T> const PodVector<T>& PodVector<T>::operator=(const PodVector<T>& val)
{
    Allocate(val.size_);
    if (val.size_) {
        std::memcpy_s(data_, sizeof(T) * val.size_, v.data_, sizeof(T) * val.size_);
    }
    return *this;
}

template <class T> void PodVector<T>::Deserialize(const int8u* data, unsigned byteSize)
{
    byteSize = byteSize / sizeof(T);
    Allocate(byteSize);
    if (byteSize) {
        std::memcpy_s(data_, byteSize * sizeof(T), data, byteSize * sizeof(T));
    }
}

template <class T> void PodVector<T>::Serialize(int8u* ptr) const
{
    if (size_) {
        std::memcpy_s(ptr, size_ * sizeof(T), data_, size_ * sizeof(T));
    }
}

template <class T> void PodVector<T>::InsertAt(unsigned pos, const T& val)
{
    if (pos >= size_) {
        data_[size_] = val;
    } else {
        std::memmove_s(data_ + pos + 1, (size_ - pos) * sizeof(T), data_ + pos, (size_ - pos) * sizeof(T));
        data_[pos] = val;
    }
    ++size_;
}

} // namespace OHOS
