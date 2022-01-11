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

#include <gfx_utils/graphics/vertexprimitive/graphic_geometry_arc.h>

namespace OHOS {
#if GRAPHIC_GEOMETYR_ENABLE_ARC_VERTEX_SOURCE
    const int CURVERENUMSTEP = 4;
    Arc::Arc(float centerX, float centerY,
             float rx, float ry,
             float start_, float end_,
             bool isClockwise)
        : mCenterX(centerX),
        mCenterY(centerY), mRadiusX(rx), mRadiusY(ry), mExpansionRatio(1.0)
    {
        Normalize(start_, end_, isClockwise);
    }

    void Arc::ApproximationScale(float sale)
    {
        mExpansionRatio = sale;
        if (mInitialized) {
            Normalize(mBeginAngle, mEndAngle, mIsClockwise);
        }
    }

    unsigned Arc::Vertex(float* x, float* y)
    {
        // 当前命令是结束点没有顶点
        if (IsStop(mPathCommand)) {
            return PATH_CMD_STOP;
        }
        if ((mCurrentAngle < mEndAngle - mDeltaAngle / CURVERENUMSTEP) != mIsClockwise) {
            *x = mCenterX + std::cos(mEndAngle) * mRadiusX;
            *y = mCenterY + std::sin(mEndAngle) * mRadiusY;
            mPathCommand = PATH_CMD_STOP;
            return PATH_CMD_LINE_TO;
        }

        *x = mCenterX + std::cos(mCurrentAngle) * mRadiusX;
        *y = mCenterY + std::sin(mCurrentAngle) * mRadiusY;

        mCurrentAngle += mDeltaAngle;

        unsigned pf = mPathCommand;
        mPathCommand = PATH_CMD_LINE_TO;
        return pf;
    }

    void Arc::Rewind(unsigned)
    {
        mPathCommand = PATH_CMD_MOVE_TO;
        mCurrentAngle = mBeginAngle;
    }

    void Arc::Normalize(float startAngle, float endAngle, bool isClockwise)
    {
        float ra = (std::fabs(mRadiusX) + std::fabs(mRadiusY)) / FLOATNUM;
        mDeltaAngle = std::acos(ra / (ra + RADDALETAELPS / mExpansionRatio)) * FLOATNUM; // 计算出弧度变化率
        if (isClockwise) {
            while (endAngle < startAngle) {
                endAngle += PI * FLOATNUM;
            }
        } else {
            while (startAngle < endAngle) {
                startAngle += PI * FLOATNUM;
            }
            mDeltaAngle = -mDeltaAngle;
        }
        mIsClockwise = isClockwise;
        mBeginAngle = startAngle;
        mEndAngle = endAngle;
        mInitialized = true;
    }

    void Arc::Init(float centerX, float centerY, float rx, float ry,
                   float startAngle, float endAngle, bool isClockwise)
    {
        mCenterX = centerX;
        mCenterY = centerY;
        mRadiusX = rx;
        mRadiusY = ry;
        Normalize(startAngle, endAngle, isClockwise);
    }
#endif
} // namespace OHOS
