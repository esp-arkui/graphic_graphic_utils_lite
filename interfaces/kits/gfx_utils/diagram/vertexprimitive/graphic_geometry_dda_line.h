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
 * @file graphic_geometryL_dda_line.h
 * @brief DDA2 Line Algorithm
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_DDA_LINE_INCLUDED
#define GRAPHIC_GEOMETRY_DDA_LINE_INCLUDED

#include <cstdlib>

#include "gfx_utils/diagram/common/graphic_common_basics.h"

namespace OHOS {
/**
 * @brief Dda2 algorithm
 * @see Dda2LineInterpolator
 * @since 1.0
 * @version 1.0
 */
class GraphicGeometryDdaLine {
public:
    GraphicGeometryDdaLine() {}

    GraphicGeometryDdaLine(int coordinate1, int coordinate2, int count)
        : dividCount_(count <= 0 ? 1 : count),
        leftSideCoordinate_((coordinate2 - coordinate1) / dividCount_),
        remainderValue_((coordinate2 - coordinate1) % dividCount_),
        moduloOperate_(remainderValue_),
        coordinateData_(coordinate1)
    {
        if (moduloOperate_ <= 0) {
            moduloOperate_ += count;
            remainderValue_ += count;
            leftSideCoordinate_--;
        }
        moduloOperate_ -= count;
    }

    void operator++()
    {
        moduloOperate_ += remainderValue_;
        coordinateData_ += leftSideCoordinate_;
        if (moduloOperate_ > 0) {
            moduloOperate_ -= dividCount_;
            coordinateData_++;
        }
    }

    void operator--()
    {
        if (moduloOperate_ <= remainderValue_) {
            moduloOperate_ += dividCount_;
            coordinateData_--;
        }
        moduloOperate_ -= remainderValue_;
        coordinateData_ -= leftSideCoordinate_;
    }

    int GetCoordinate() const
    {
        return coordinateData_;
    }

private:
    int dividCount_;
    int leftSideCoordinate_;
    int remainderValue_;
    int moduloOperate_;
    int coordinateData_;
};
} // namespace OHOS
#endif
