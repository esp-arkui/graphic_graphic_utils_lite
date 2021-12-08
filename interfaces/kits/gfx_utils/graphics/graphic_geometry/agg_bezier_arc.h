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
 * @brief Defines ArcToBezier BezierArc BezierArcSvg.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file graphic_geometry_bezier_arc.h
 *
 * @brief Defines ���������ṹ��.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef GRAPHIC_GEOMETRY_BEZIER_ARC_INCLUDED
#define GRAPHIC_GEOMETRY_BEZIER_ARC_INCLUDED

#include "gfx_utils/graphics/graphic_depict/agg_conv_transform.h"

namespace OHOS {
/**
 * @brief ����ת��Ϊ����������.
 *
 * @since 1.0
 * @version 1.0
 */
void ArcToBezier(double cx, double cy, double rx, double ry,
                 double startAngle, double sweepAngle,
                 double* curve);

/**
 * @file graphic_geometry_bezier_arc.h
 *
 * @brief Defines BezierArc.
 *
 * @since 1.0
 * @version 1.0
 */
class BezierArc : public HeapBase {
public:
    BezierArc()
        : vertex_(26), numVertices_(0), cmd_(PATH_CMD_LINE_TO)
    {}

    /**
     * @brief ʹ�û��߳�ʼ������������.
     * @Param centerX centerY Բ��Բ�ģ�rx ry Բ�����ݰ뾶��startAngle sweepAngle Բ����ֹ�Ƕ�
     * @since 1.0
     * @version 1.0
     */
    BezierArc(double centerX, double centerY,
              double rx, double ry,
              double startAngle,
              double sweepAngle)
    {
        Init(centerX, centerY, rx, ry, startAngle, sweepAngle);
    }

    /**
     * @brief ʹ�û��߳�ʼ������������.
     * @since 1.0
     * @version 1.0
     */
    void Init(double x, double y,
              double rx, double ry,
              double startAngle,
              double sweepAngle);

    /**
     * @brief �ص��ʼ����.
     *
     * @since 1.0
     * @version 1.0
     */
    void Rewind(unsigned)
    {
        vertex_ = 0;
    }

    /**
     * @brief ȡ������Դ��������ת������ͨ��. 
     * @return ��������
     * @since 1.0
     * @version 1.0
     */
    unsigned Vertex(double* x, double* y)
    {
        if (vertex_ >= numVertices_) {
            return PATH_CMD_STOP;
        }
        *x = vertices_[vertex_];
        *y = vertices_[vertex_ + 1];
        vertex_ = vertex_ + 2;
        if (vertex_ == 2) {
            return unsigned(PATH_CMD_MOVE_TO);
        } else {
            return cmd_;
        }
        
    }

    /**
     * @brief ��ȡ����Դ����.
     * @return �������
     * @since 1.0
     * @version 1.0
     */
    unsigned Nuvertices_() const
    {
        return numVertices_;
    }

    /**
     * @brief ��ȡ����Դ.
     * @return ����Դ����
     * @since 1.0
     * @version 1.0
     */
    const double* Vertices() const
    {
        return vertices_;
    }

    /**
     * @brief ��ȡ����Դ.
     * @return ����Դ����
     * @since 1.0
     * @version 1.0
     */
    double* Vertices()
    {
        return vertices_;
    }

private:
    unsigned vertex_;
    unsigned numVertices_;
    double vertices_[26];
    unsigned cmd_;
};

/**
 * @brief ����SVG��ʽ�ı�������.
 *
 * @since 1.0
 * @version 1.0
 */
class BezierArcSvg : public HeapBase {
public:
    BezierArcSvg()
        : arc_(), radiiOK_(false)
    {}

    /**
     * @brief ����ӣ�x1��y1������x2��y2������Բ��.
     * ��Բ�ķ����������뾶��rx��ry������
     * @since 1.0
     * @version 1.0
     */
    BezierArcSvg(double x1,
                 double y1,
                 double rx, double ry,
                 double angle,
                 bool largeArcFlag,
                 bool sweepFlag,
                 double x2, double y2)
        : arc_(), radiiOK_(false) 
    {
        Init(x1, y1, rx, ry, angle, largeArcFlag, sweepFlag, x2, y2);
    }

    bool RadiiOK() const
    {
        return radiiOK_;
    }

    void Init(double x1,
              double y1,
              double rx,
              double ry,
              double angle,
              bool largeArcFlag,
              bool sweepFlag,
              double x2,
              double y2);

    /**
    * @brief �ص��ʼ����.
    *
    * @since 1.0
    * @version 1.0
    */
    void Rewind(unsigned)
    {
        arc_.Rewind(0);
    }

    /**
     * @brief ȡ������Դ��������ת������ͨ��.
     * @return ������������
     * @since 1.0
     * @version 1.0
     */
    unsigned Vertex(double* x, double* y)
    {
        return arc_.Vertex(x, y);
    }

    /**
     * @brief ���ر��������Ķ������ݡ�
     * @return ���ض���Դ
     * @since 1.0
     * @version 1.0
     */
    const double* Vertices() const
    {
        return arc_.Vertices();
    }

    /**
     * @brief ���ر��������Ķ�������.
     * @return ���ر��������Ķ�������.
     * @since 1.0
     * @version 1.0
     */
    double* Vertices()
    {
        return arc_.Vertices();
    }

    /**
     * @brief ����������ֵ��������Ҳ����˵������1�����㣬������2��
     * @since 1.0
     * @version 1.0
     */
    unsigned NumVertices_() const
    {
        return arc_.NumVertices_();
    }

private:
    BezierArc arc_;
    bool radiiOK_;
};

} // namespace OHOS

#endif
