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

/**
 * @file graphic_geometry_arc.h
 *
 * @brief Defines ����������.
 *
 * @since 1.0
 * @version 1.0
 */
#ifndef GRAPHIC_GEOMETRY_ARC_INCLUDED
#define GRAPHIC_GEOMETRY_ARC_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"

namespace OHOS {
class Arc : public HeapBase {
public:
    Arc()
        : scale_(1.0), initialized_(false)
    {
    }

    Arc(double centerX, double centerY,
        double rx, double ry,
        double angle1, double angle2,
        bool ccw = true);
    /**
     * @brief �ص��ʼ����.
     *
     * @since 1.0
     * @version 1.0
     */
    void Rewind(unsigned);

    /**
     * @brief ��������ֵ.
     *
     * @since 1.0
     * @version 1.0
     */
    void ApproximationScale(double scale);

    /**
     * @brief ��ȡ����ֵ.
     *
     * @since 1.0
     * @version 1.0
     */
    double ApproximationScale() const
    {
        return scale_;
    }

    /**
     * @brief ȡ������Դ��������ת������ͨ��.
     *
     * @since 1.0
     * @version 1.0
     */
    unsigned Vertex(double* y, double* x);
    /**
     * @brief ��ʼ������.
     *
     * @since 1.0
     * @version 1.0
     */
    void Init(double centerX, double centerY, double rx, double ry, double angle1, double angle2, bool ccw = true);

private:
    /**
     * @brief �淶����������.
     *
     * @since 1.0
     * @version 1.0
     */
    void Normalize(double angle1, double angle2, bool ccw);
    bool initialized_;
    unsigned pathCmd_;
    double centerX_;
    double centerY_;
    double rx_;
    double ry_;
    double angle_;
    double start_;
    double end_;
    double scale_;
    double da_;
    bool ccw_; //���ߵķ���
};

} // namespace OHOS

#endif
