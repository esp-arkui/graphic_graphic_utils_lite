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

#include <gfx_utils/graphics/graphic_geometry/graphic_geometry_bezier_arc.h>

#include <cmath>

namespace OHOS {
    void ArcToBezier(double cx, double cy, double rx, double ry,
                     double startAngle, double sweepAngle,
                     double* curve)
    {
        double y0 = std::sin(sweepAngle / DOUBLENUM);
        double x0 = std::cos(sweepAngle / DOUBLENUM);
        double tx = (1.0 - x0) * BEZIER_ARC_DELTAX / BEZIER_ARC_EQUAL_DIVISION;
        double ty = y0 - tx * x0 / y0;
        double px[BEZIER_ARC_POINTS];
        double py[BEZIER_ARC_POINTS];
        px[INDEX_ZERO] = x0;
        py[INDEX_ZERO] = -y0;
        px[INDEX_ONE] = x0 + tx;
        py[INDEX_ONE] = -ty;
        px[INDEX_TWO] = x0 + tx;
        py[INDEX_TWO] = ty;
        px[INDEX_THREE] = x0;
        py[INDEX_THREE] = y0;

        double cosVal = std::cos(startAngle + sweepAngle / DOUBLENUM);
        double sinVal = std::sin(startAngle + sweepAngle / DOUBLENUM);

        for (unsigned i = 0; i < BEZIER_ARC_POINTS; i++) {
            curve[i * BEZIER_ARC_SETUP] = cx + rx * (px[i] * cosVal - py[i] * sinVal);
            curve[i * BEZIER_ARC_SETUP + 1] = cy + ry * (px[i] * sinVal + py[i] * cosVal);
        }
    }

    void BezierArc::Init(double x, double y,
                         double rx, double ry,
                         double startAngle,
                         double sweepAngle)
    {
        startAngle = std::fmod(startAngle, DOUBLENUM * PI);
        if (sweepAngle <= -DOUBLENUM * PI) {
            sweepAngle = -DOUBLENUM * PI;
        }
        if (sweepAngle >= DOUBLENUM * PI) {
            sweepAngle = DOUBLENUM * PI;
        }
        if (std::fabs(sweepAngle) < 1e-10) {
            numVertices_ = BEZIER_ARC_POINTS;
            cmd_ = PATH_CMD_LINE_TO;
            vertices_[INDEX_ZERO] = x + rx * std::cos(startAngle);
            vertices_[INDEX_ONE] = y + ry * std::sin(startAngle);
            vertices_[INDEX_TWO] = x + rx * std::cos(startAngle + sweepAngle);
            vertices_[INDEX_THREE] = y + ry * std::sin(startAngle + sweepAngle);
            return;
        }

        double prevSweep;
        double totalSweep = 0.0;
        double localSweep = 0.0;
        numVertices_ = BEZIER_ARC_SETUP;
        cmd_ = PATH_CMD_CURVE4;
        bool done = false;
        do {
            if (0.0 > sweepAngle) {
                prevSweep = totalSweep;
                totalSweep -= PI * HALFNUM;
                localSweep = -PI * HALFNUM;
                if (totalSweep <= sweepAngle + BEZIER_ARC_ANGLE_EPSILON) {
                    localSweep = sweepAngle - prevSweep;
                    done = true;
                }
            } else {
                prevSweep = totalSweep;
                totalSweep += PI * HALFNUM;
                localSweep = PI * HALFNUM;
                if (totalSweep >= sweepAngle - BEZIER_ARC_ANGLE_EPSILON) {
                    localSweep = sweepAngle - prevSweep;
                    done = true;
                }
            }

            ArcToBezier(x, y, rx, ry, startAngle, localSweep, vertices_ + numVertices_ - BEZIER_ARC_SETUP);

            startAngle += localSweep;
            numVertices_ += BEZIER_ARC_VERTICES_SIZE_STEP;
        } while (numVertices_ < BEZIER_ARC_VERTEX_NUM && !done);
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

        double delatY2 = (y0 - y2) / DOUBLENUM;
        double delatX2 = (x0 - x2) / DOUBLENUM;

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
            if (radiiCheck > BEZIER_ARC_RADIICHECK) {
                radiiOK_ = false;
            }
        }
        double sign = (largeArcFlag == sweepFlag) ? -1.0 : 1.0;
        double sq = (prx * pry - prx * py1 - pry * px1) / (prx * py1 + pry * px1);
        double coef = sign * std::sqrt((sq < 0) ? 0 : sq);
        double cx1 = coef * ((rx * y1) / ry);
        double cy1 = coef * -((ry * x1) / rx);

        double sx2 = (x0 + x2) / DOUBLENUM;
        double sy2 = (y0 + y2) / DOUBLENUM;
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
            sweepAngle -= PI * DOUBLENUM;
        } else if (sweepFlag && sweepAngle < 0) {
            sweepAngle += PI * DOUBLENUM;
        }
        arc_.Init(0.0, 0.0, rx, ry, startAngle, sweepAngle);
        TransAffine mtx = TransAffineRotation(angle);
        mtx *= TransAffineTranslation(cx, cy);
        for (unsigned i = BEZIER_ARC_SETUP; i < arc_.Nuvertices() - BEZIER_ARC_SETUP; i += BEZIER_ARC_SETUP) {
            mtx.Transform(arc_.Vertices() + i, arc_.Vertices() + i + 1);
        }
        arc_.Vertices()[0] = x0;
        arc_.Vertices()[1] = y0;
        if (arc_.Nuvertices() > BEZIER_ARC_SETUP) {
            arc_.Vertices()[arc_.Nuvertices() - BEZIER_ARC_SETUP] = x2;
            arc_.Vertices()[arc_.Nuvertices() - 1] = y2;
        }
    }
} // namespace OHOS
