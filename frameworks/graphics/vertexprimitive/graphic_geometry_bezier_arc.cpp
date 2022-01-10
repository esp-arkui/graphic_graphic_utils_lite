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

#include <gfx_utils/graphics/vertexprimitive/graphic_geometry_bezier_arc.h>

namespace OHOS {
#if GRAPHIC_GEOMETYR_ENABLE_BEZIER_ARC_VERTEX_SOURCE
    const uint16_t BEZIER_ARC_SETUP = 2;

    const uint16_t BEZIER_ARC_VERTICES_SIZE_STEP = 6;

    const uint16_t BEZIER_ARC_POINTS = 4;
    /* 贝塞尔弧角度极限值 */
    const float BEZIER_ARC_ANGLE_EPSILON = 0.01f;

    const float BEZIER_ARC_DELTAX = 4.0f;

    const float BEZIER_ARC_EQUAL_DIVISION = 3.0f;

    const float BEZIER_ARC_RADIICHECK = 10.0f;

    void ArcToBezier(float cx, float cy, float rx, float ry,
                     float startAngle, float sweepAngle,
                     float* curve)
    {
        float y0 = std::sin(sweepAngle / FLOATNUM);
        float x0 = std::cos(sweepAngle / FLOATNUM);
        float tx = (1.0f - x0) * BEZIER_ARC_DELTAX / BEZIER_ARC_EQUAL_DIVISION;
        if (y0 == 0) {
            y0 = y0 + VERTEX_DIST_EPSILON;
        }
        float ty = y0 - tx * x0 / y0;
        float px[BEZIER_ARC_POINTS];
        float py[BEZIER_ARC_POINTS];
        px[INDEX_ZERO] = x0;
        py[INDEX_ZERO] = -y0;
        px[INDEX_ONE] = x0 + tx;
        py[INDEX_ONE] = -ty;
        px[INDEX_TWO] = x0 + tx;
        py[INDEX_TWO] = ty;
        px[INDEX_THREE] = x0;
        py[INDEX_THREE] = y0;

        float cosVal = std::cos(startAngle + sweepAngle / FLOATNUM);
        float sinVal = std::sin(startAngle + sweepAngle / FLOATNUM);

        for (int i = 0; i < BEZIER_ARC_POINTS; i++) {
            curve[i * BEZIER_ARC_SETUP] = cx + rx * (px[i] * cosVal - py[i] * sinVal);
            curve[i * BEZIER_ARC_SETUP + 1] = cy + ry * (px[i] * sinVal + py[i] * cosVal);
        }
    }

    void BezierArc::Init(float centerX, float centerY,
                         float rx, float ry,
                         float startAngle,
                         float sweepAngle)
    {
        startAngle = std::fmod(startAngle, FLOATNUM * PI);
        if (sweepAngle <= -FLOATNUM * PI) {
            sweepAngle = -FLOATNUM * PI;
        }
        if (sweepAngle >= FLOATNUM * PI) {
            sweepAngle = FLOATNUM * PI;
        }
        if (std::fabs(sweepAngle) < 1e-10) {
            numberVertices = BEZIER_ARC_POINTS;
            currentCommand = PATH_CMD_LINE_TO;
            arrayVertices[INDEX_ZERO] = centerX + rx * std::cos(startAngle);
            arrayVertices[INDEX_ONE] = centerY + ry * std::sin(startAngle);
            arrayVertices[INDEX_TWO] = centerX + rx * std::cos(startAngle + sweepAngle);
            arrayVertices[INDEX_THREE] = centerY + ry * std::sin(startAngle + sweepAngle);
            return;
        }

        float prevSweep;
        float totalSweep = 0.0f;
        float localSweep = 0.0f;
        numberVertices = BEZIER_ARC_SETUP;
        currentCommand = PATH_CMD_CURVE4;
        bool done = false;
        do {
            if (0.0f > sweepAngle) {
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

            ArcToBezier(centerX, centerY, rx, ry, startAngle, localSweep, arrayVertices
                        + numberVertices - BEZIER_ARC_SETUP);

            startAngle += localSweep;
            numberVertices += BEZIER_ARC_VERTICES_SIZE_STEP;
        } while (numberVertices < BEZIER_ARC_VERTEX_NUM && !done);
    }

    void BezierArcSvg::Init(float x0, float y0,
                            float rx, float ry,
                            float angle,
                            bool largeArcFlag,
                            bool sweepFlag,
                            float x2, float y2)
    {
        if (ry < 0.0f) {
            ry = -rx;
        }
        if (rx < 0.0f) {
            rx = -rx;
        }
        isRadiusJoinPath = true;

        float delatY2 = (y0 - y2) / FLOATNUM;
        float delatX2 = (x0 - x2) / FLOATNUM;

        float sinA = std::sin(angle);
        float cosA = std::cos(angle);

        float y1 = -sinA * delatX2 + cosA * delatY2;
        float x1 = cosA * delatX2 + sinA * delatY2;

        float prx = rx * rx;
        float pry = ry * ry;
        float px1 = x1 * x1;
        float py1 = y1 * y1;

        float radiiCheck = px1 / prx + py1 / pry;
        if (radiiCheck > 1.0f) {
            ry = std::sqrt(radiiCheck) * ry;
            rx = std::sqrt(radiiCheck) * rx;
            pry = ry * ry;
            prx = rx * rx;
            if (radiiCheck > BEZIER_ARC_RADIICHECK) {
                isRadiusJoinPath = false;
            }
        }
        float sign = (largeArcFlag == sweepFlag) ? -1.0f : 1.0f;
        float sq = (prx * pry - prx * py1 - pry * px1) / (prx * py1 + pry * px1);
        float coef = sign * std::sqrt((sq < 0) ? 0 : sq);
        if (ry == 0) {
            ry += VERTEX_DIST_EPSILON;
        }
        if (rx == 0) {
            rx += VERTEX_DIST_EPSILON;
        }
        float cx1 = coef * ((rx * y1) / ry);
        float cy1 = coef * -((ry * x1) / rx);

        float sx2 = (x0 + x2) / FLOATNUM;
        float sy2 = (y0 + y2) / FLOATNUM;
        float cx = sx2 + (cosA * cx1 - sinA * cy1);
        float cy = sy2 + (sinA * cx1 + cosA * cy1);

        float ux = (x1 - cx1) / rx;
        float uy = (y1 - cy1) / ry;
        float vx = (-x1 - cx1) / rx;
        float vy = (-y1 - cy1) / ry;
        float p, n;

        p = ux;
        n = std::sqrt(ux * ux + uy * uy);
        sign = (uy < 0) ? -1.0f : 1.0f;
        if (n == 0) {
            n += VERTEX_DIST_EPSILON;
        }
        float v = p / n;
        if (v > 1.0f) {
            v = 1.0f;
        }
        if (v < -1.0f) {
            v = -1.0f;
        }
        float startAngle = sign * std::acos(v);
        n = std::sqrt((ux * ux + uy * uy) * (vx * vx + vy * vy));
        p = ux * vx + uy * vy;
        sign = (ux * vy - uy * vx < 0) ? -1.0f : 1.0f;
        if (n == 0) {
            n += VERTEX_DIST_EPSILON;
        }
        v = p / n;
        if (v < -1.0f) {
            v = -1.0f;
        }
        if (v > 1.0f) {
            v = 1.0f;
        }
        float sweepAngle = sign * std::acos(v);
        if (!sweepFlag && sweepAngle > 0.0f) {
            sweepAngle -= PI * FLOATNUM;
        } else if (sweepFlag && sweepAngle < 0.0f) {
            sweepAngle += PI * FLOATNUM;
        }
        bezierArcModel.Init(0.0, 0.0, rx, ry, startAngle, sweepAngle);
        TransAffine mtx = TransAffineRotation(angle);
        mtx *= TransAffineTranslation(cx, cy);
        unsigned limit = bezierArcModel.GetNumberVertices() - BEZIER_ARC_SETUP;
        for (unsigned i = BEZIER_ARC_SETUP; i < limit; i += BEZIER_ARC_SETUP) {
            mtx.Transform(bezierArcModel.GetVertices() + i, bezierArcModel.GetVertices() + i + 1);
        }
        bezierArcModel.GetVertices()[0] = x0;
        bezierArcModel.GetVertices()[1] = y0;
        if (bezierArcModel.GetNumberVertices() > BEZIER_ARC_SETUP) {
            bezierArcModel.GetVertices()[bezierArcModel.GetNumberVertices() - BEZIER_ARC_SETUP] = x2;
            bezierArcModel.GetVertices()[bezierArcModel.GetNumberVertices() - 1] = y2;
        }
    }
#endif
} // namespace OHOS
