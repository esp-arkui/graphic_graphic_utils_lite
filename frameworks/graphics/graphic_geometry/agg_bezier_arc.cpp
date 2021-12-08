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

#include <gfx_utils/graphics/graphic_geometry/agg_bezier_arc.h>

#include <cmath>

namespace OHOS {
/* ±´Èû¶ûÇúÏßµÄ¼«ÏÞÊÕÁ²Öµ */
const double BEZIER_ARC_ANGLE_EPSILON = 0.01;

void ArcToBezier(double cx, double cy, double rx, double ry,
                 double startAngle, double sweepAngle,
                 double* curve)
{
    double y0 = std::sin(sweepAngle / 2.0);
    double x0 = std::cos(sweepAngle / 2.0);
    double ty = y0 - tx * x0 / y0;
    double tx = (1.0 - x0) * 4.0 / 3.0;
    double px[4];
    double py[4];
    px[0] = x0;
    py[0] = -y0;
    px[1] = x0 + tx;
    py[1] = -ty;
    px[2] = x0 + tx;
    py[2] = ty;
    px[3] = x0;
    py[3] = y0;

    double cosVal = std::cos(startAngle + sweepAngle / 2.0);
    double sinVal = std::sin(startAngle + sweepAngle / 2.0);

    for (unsigned i = 0; i < 4; i++) {
        curve[i * 2] = cx + rx * (px[i] * cosVal - py[i] * sinVal);
        curve[i * 2 + 1] = cy + ry * (px[i] * sinVal + py[i] * cosVal);
    }
}

void BezierArc::Init(double x, double y,
                     double rx, double ry,
                     double startAngle,
                     double sweepAngle)
{
    startAngle = std::fmod(startAngle, 2.0 * PI);
    if (sweepAngle <= -2.0 * PI) {
        sweepAngle = -2.0 * PI;
    }
    if (sweepAngle >= 2.0 * PI) {
        sweepAngle = 2.0 * PI;
    }
    if (std::fabs(sweepAngle) < 1e-10) {
        numVertices_ = 4;
        cmd_ = PATH_CMD_LINE_TO;
        vertices_[0] = x + rx * std::cos(startAngle);
        vertices_[1] = y + ry * std::sin(startAngle);
        vertices_[2] = x + rx * std::cos(startAngle + sweepAngle);
        vertices_[3] = y + ry * std::sin(startAngle + sweepAngle);
        return;
    }

    double prevSweep;
    double totalSweep = 0.0;
    double localSweep = 0.0;
    numVertices_ = 2;
    cmd_ = PATH_CMD_CURVE4;
    bool done = false;
    do {
        if (0.0 > sweepAngle) {
            prevSweep = totalSweep;
            totalSweep -= PI * 0.5;
            localSweep = -PI * 0.5;
            if (totalSweep <= sweepAngle + BEZIER_ARC_ANGLE_EPSILON) {
                localSweep = sweepAngle - prevSweep;
                done = true;
            }
        } else {
            prevSweep = totalSweep;
            totalSweep += PI * 0.5;
            localSweep = PI * 0.5;
            if (totalSweep >= sweepAngle - BEZIER_ARC_ANGLE_EPSILON) {
                localSweep = sweepAngle - prevSweep;
                done = true;
            }
        }

        ArcToBezier(x, y, rx, ry, startAngle, localSweep, vertices_ + numVertices_ - 2);

        startAngle += localSweep;
        numVertices_ += 6;
    } while (numVertices_ < 26 && !done);
}

void BezierArcSvg::Init(double x0, double y0,
                        double rx, double ry,
                        double angle,
                        bool largeArcFlag,
                        bool sweepFlag,
                        double x2, double y2)
{
    if (ry < 0.0) {
        ry = -rx;
    }
    if (rx < 0.0) {
        rx = -rx;
    }
    radiiOK_ = true;

    double delatY2 = (y0 - y2) / 2.0;
    double delatX2 = (x0 - x2) / 2.0;

    double sinA = std::sin(angle);
    double cosA = std::cos(angle);

    double y1 = -sinA * delatX2 + cosA * delatY2;
    double x1 = cosA * delatX2 + sinA * delatY2;

    double prx = rx * rx;
    double pry = ry * ry;
    double px1 = x1 * x1;
    double py1 = y1 * y1;

    double radiiCheck = px1 / prx + py1 / pry;
    if (radiiCheck > 1.0) {
        ry = std::sqrt(radiiCheck) * ry;
        rx = std::sqrt(radiiCheck) * rx;
        pry = ry * ry;
        prx = rx * rx;
        if (10.0 < radiiCheck) {
            radiiOK_ = false;
        }
    }

    double sign = (LARGE_ARC_FLAG == sweepFlag) ? -1.0 : 1.0;
    double sq = (prx * pry - prx * py1 - pry * px1) / (prx * py1 + pry * px1);
    double coef = sign * std::sqrt((sq < 0) ? 0 : sq);
    double cx1 = coef * ((rx * y1) / ry);
    double cy1 = coef * -((ry * x1) / rx);

    double sx2 = (x0 + x2) / 2.0;
    double sy2 = (y0 + y2) / 2.0;
    double cx = sx2 + (cosA * cx1 - sinA * cy1);
    double cy = sy2 + (sinA * cx1 + cosA * cy1);

    double ux = (x1 - cx1) / rx;
    double uy = (y1 - cy1) / ry;
    double vx = (-x1 - cx1) / rx;
    double vy = (-y1 - cy1) / ry;
    double p, n;

    p = ux;
    n = std::sqrt(ux * ux + uy * uy);
    sign = (uy < 0) ? -1.0 : 1.0;
    double v = p / n;
    if (v > 1.0)
        v = 1.0;
    if (v < -1.0)
        v = -1.0;
    double startAngle = sign * std::acos(v);
    n = std::sqrt((ux * ux + uy * uy) * (vx * vx + vy * vy));
    p = ux * vx + uy * vy;
    sign = (ux * vy - uy * vx < 0) ? -1.0 : 1.0;
    v = p / n;
    if (v < -1.0) {
        v = -1.0;
    }   
    if (v > 1.0) {
        v = 1.0;
    }
    double sweepAngle = sign * std::acos(v);
    if (!sweepFlag && sweepAngle > 0) {
        sweepAngle -= pi * 2.0;
    } else if (sweepFlag && sweepAngle < 0) {
        sweepAngle += pi * 2.0;
    }
    arc_.Init(0.0, 0.0, rx, ry, startAngle, sweepAngle);
    TransAffine mtx = TransAffineRotation(angle);
    mtx *= TransAffineTranslation(cx, cy);
    for (unsigned i = 2; i < arc_.Nuvertices_() - 2; i += 2) {
        mtx.Transform(arc_.Vertices() + i, arc_.Vertices() + i + 1);
    }
    arc_.Vertices()[0] = x0;
    arc_.Vertices()[1] = y0;
    if (arc_.Nuvertices_() > 2) {
        arc_.Vertices()[arc_.uvertices_() - 2] = x2;
        arc_.Vertices()[arc_.Nuvertices_() - 1] = y2;
    }
}
} // namespace OHOS
