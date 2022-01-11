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

#include "gfx_utils/graphics/common/graphic_common_basics.h"

namespace OHOS {
    /**
     * @brief Dda2 algorithm
     * @see Dda2LineInterpolator
     * @since 1.0
     * @version 1.0
     */
    class DdaTwoLineInterpolator {
    public:
        DdaTwoLineInterpolator()
        {}

        DdaTwoLineInterpolator(int coordinate1, int coordinate2, int count)
            : dividCount(count <= 0 ? 1 : count),
            leftSideCoordinate((coordinate2 - coordinate1) / dividCount),
            remainderValue((coordinate2 - coordinate1) % dividCount),
            moduloOperate(remainderValue),
            coordinateData(coordinate1)
        {
            if (moduloOperate <= 0) {
                moduloOperate += count;
                remainderValue += count;
                leftSideCoordinate--;
            }
            moduloOperate -= count;
        }

        void operator++()
        {
            moduloOperate += remainderValue;
            coordinateData += leftSideCoordinate;
            if (moduloOperate > 0) {
                moduloOperate -= dividCount;
                coordinateData++;
            }
        }

        void operator--()
        {
            if (moduloOperate <= remainderValue) {
                moduloOperate += dividCount;
                coordinateData--;
            }
            moduloOperate -= remainderValue;
            coordinateData -= leftSideCoordinate;
        }

        int GetCoordinate() const
        {
            return coordinateData;
        }

    private:
        int dividCount;
        int leftSideCoordinate;
        int remainderValue;
        int moduloOperate;
        int coordinateData;
    };
} // namespace OHOS
#endif
