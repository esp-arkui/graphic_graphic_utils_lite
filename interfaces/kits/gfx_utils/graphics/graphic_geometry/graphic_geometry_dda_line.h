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
 * @file graphic_geometry_dda_line.h
 * @brief DDA2直线算法
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_DDA_LINE_INCLUDED
#define GRAPHIC_GEOMETRY_DDA_LINE_INCLUDED

#include <cstdlib>

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"

namespace OHOS {
    /**
     * @brief DDA2算法
     * @see Dda2LineInterpolator
     * @since 1.0
     * @version 1.0
     */
    class Dda2LineInterpolator {
    public:
        Dda2LineInterpolator()
        {}

        Dda2LineInterpolator(int coordinate1, int coordinate2, int count)
            :count_(count <= 0 ? 1 : count),
            left_((coordinate2 - coordinate1) / count_),
            remainder_((coordinate2 - coordinate1) % count_),
            modulo_(remainder_),
            coordinate_(coordinate1)
        {
            if (modulo_ <= 0) {
                modulo_ += count;
                remainder_ += count;
                left_--;
            }
            modulo_ -= count;
        }

        void operator++()
        {
            modulo_ += remainder_;
            coordinate_ += left_;
            if (modulo_ > 0) {
                modulo_ -= count_;
                coordinate_++;
            }
        }

        void operator--()
        {
            if (modulo_ <= remainder_) {
                modulo_ += count_;
                coordinate_--;
            }
            modulo_ -= remainder_;
            coordinate_ -= left_;
        }

        int GetCoordinate() const
        {
            return coordinate_;
        }

    private:
        int count_;
        int left_;
        int remainder_;
        int modulo_;
        int coordinate_;
    };
} // namespace OHOS

#endif
