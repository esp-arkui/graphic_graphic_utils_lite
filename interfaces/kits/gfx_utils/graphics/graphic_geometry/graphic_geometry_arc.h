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

#ifndef GRAPHIC_GEOMETRY_ARC_INCLUDED
#define GRAPHIC_GEOMETRY_ARC_INCLUDED

#include "gfx_utils/graphics/graphic_common/graphic_common_basics.h"
#include "gfx_utils/heap_base.h"
namespace OHOS {
    /**
     * @file graphic_geometry_arc.h
     *
     * @brief Defines 弧线类.
     *
     * @since 1.0
     * @version 1.0
     */
    class Arc : public HeapBase {
    public:
        Arc() :
            mInitialized(false), mExpansionRatio(1.0)
        {
        }

        /**
         * @brief 构造弧线.
         * @param centerX,centerY 弧线圆心, rx 椭圆弧横轴半径 ry 椭圆弧纵轴半径,
         * angle1,angle2起始角度,isClockwise 弧线是否顺时针方向
         * @since 1.0
         * @version 1.0
         */
        Arc(double centerX, double centerY,
            double rx, double ry,
            double angle1, double angle2,
            bool isClockwise = true);
        /**
         * @brief 设置为初始步骤.
         *
         * @since 1.0
         * @version 1.0
         */
        void Rewind(unsigned);

        /**
         * @brief 为正确调整近似级别
         * @param scale为视口坐标和逻辑坐标之间的比率
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        void ApproximationScale(double scale);

        /**
         * @brief 获取近似级别
         * @param scale为视口坐标和逻辑坐标之间的比率
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        double ApproximationScale() const
        {
            return mExpansionRatio;
        }

        /**
         * @brief 在采样阶段调用
         * @param x为点坐标x的值的指针，y为点坐标y的值的指针
         * @return 无
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(double* y, double* x);
        /**
         * @brief 初始化一个弧线.
         *
         * @since 1.0
         * @version 1.0
         */
        void Init(double centerX, double centerY, double rx, double ry,
                  double angle1, double angle2, bool isClockwise = true);

    private:
        /**
         * @brief 规范化弧线.
         *
         * @since 1.0
         * @version 1.0
         */
        void Normalize(double angle1, double angle2, bool isClockwise);
        bool mInitialized;      // 是否被Init
        unsigned mPathCommand;  // 连接命令
        double mCenterX;        // 中心X坐标
        double mCenterY;        // 中心Y坐标
        double mRadiusX;        // X短半轴
        double mRadiusY;        // Y长半轴
        double mCurrentAngle;   // 当前角度
        double mBeginAngle;     // 起始角度
        double mEndAngle;       // 结束角度
        double mExpansionRatio; // 伸缩比
        double mDeltaAngle;     // 弧度变化率
        bool mIsClockwise;      // 方向
    };
} // namespace OHOS

#endif
