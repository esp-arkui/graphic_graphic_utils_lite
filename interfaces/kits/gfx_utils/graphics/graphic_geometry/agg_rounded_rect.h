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
 * @brief Defines RoundedRect.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file graphic_geometry_bounding_rect.h
 *
 * @brief Defines Բ�Ǿ�����.
 *
 * @since 1.0
 * @version 1.0
 */
#ifndef GRAPHIC_GEOMETRY_ROUNDED_RECT_INCLUDED
#define GRAPHIC_GEOMETRY_ROUNDED_RECT_INCLUDED

#include "gfx_utils/graphics/graphic_common/agg_basics.h"
#include "agg_arc.h"

namespace OHOS{
/**
 * @brief RoundedRect.
 *
 * @since 1.0
 * @version 1.0
 */
class RoundedRect : public HeapBase{
public:
    /**
     * @brief ����Բ�Ǿ���.
     *
     * @param x1,y1 �������ϽǶ���,x2,y2 �������½Ƕ���,r Բ�ǰ뾶.
     * @since 1.0
     * @version 1.0
     */
    RoundedRect(double x1, double y1, double x2, double y2, double r);

    RoundedRect() {}

    /**
     * @brief ����Բ�Ǿ��ζ���Բ���ĺ���뾶������뾶.
     *
     * @param rx����뾶,����뾶.
     * @since 1.0
     * @version 1.0
     */
    void Radius(double rx, double ry);

    /**
     * @brief ����Բ�Ǿ����ĸ�����Բ���ĺ���뾶������뾶.
     *
     * @param rxBottom�·����������뾶,ryBottom�·�����������뾶,rxTop�Ϸ����������뾶,ryTop�Ϸ�����������뾶.
     * @since 1.0
     * @version 1.0
     */
    void Radius(double rxBottom, double ryBottom, double rxTop, double ryTop);

     /**
     * @brief �ֱ�����Բ�Ǿ����ĸ�����Բ���ĺ���뾶������뾶.
     *
     * @param �ֱ��Ӧ�ĸ�����,rx ����뾶,ry ����뾶.
     * @since 1.0
     * @version 1.0
     */
    void Radius(double rx1, double ry1, double rx2, double ry2, 
                double rx3, double ry3, double rx4, double ry4);

    /**
     * @brief ���ö��㻡�ε�������
     *
     * @param ������.
     * @since 1.0
     * @version 1.0
     */
    void ApproximationScale(double scale)
    {
        arc_.ApproximationScale(scale);
    }

    /**
     * @brief ���þ��ε����Ͻ������½���������.
     *
     * @param x1��y1���ϽǶ���,x2��y2���½Ƕ���.
     * @since 1.0
     * @version 1.0
     */
    void Rect(double x1, double y1, double x2, double y2);

    /**
     * @brief ����Բ�Ǿ��ζ���Բ���İ뾶.
     *
     * @param r �뾶.
     * @since 1.0
     * @version 1.0
     */
    void Radius(double r);

    /**
     * @brief �淶��Բ�Ǿ��εĻ��ΰ뾶.
     *
     * @since 1.0
     * @version 1.0
     */
    void NormalizeRadius();
    
     /**
     * @brief �ص��ʼ����.
     *
     * @since 1.0
     * @version 1.0
     */   
    void Rewind(unsigned);

    /**
     * @brief ��ȡ���㻡�ε�������
     * @return ������.
     * @since 1.0
     * @version 1.0
     */
    double ApproximationScale() const
    {
        return arc_.ApproximationScale();
    }

    /**
     * @brief ȡ������Դ��������ת������ͨ��.
     * @param x,y ����Դ
     * @return ��������
     * @since 1.0
     * @version 1.0
     */
    unsigned Vertex(double* x, double* y);

private:
    unsigned status_;
    Arc arc_;
    double x1_;
    double y1_;
    double x2_;
    double y2_;
    double rx1_;
    double ry1_;
    double rx2_;
    double ry2_;
    double rx3_;
    double ry3_;
    double rx4_;
    double ry4_;

};

}

#endif

