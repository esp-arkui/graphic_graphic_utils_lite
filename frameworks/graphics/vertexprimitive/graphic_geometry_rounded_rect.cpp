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

#include <gfx_utils/graphics/vertexprimitive/graphic_geometry_rounded_rect.h>

namespace OHOS {
#if GRAPHIC_GEOMETYR_ENABLE_ROUNDEDRECT_VERTEX_SOURCE && GRAPHIC_GEOMETYR_ENABLE_ARC_VERTEX_SOURCE
    RoundedRect::RoundedRect(double x1, double y1, double x2, double y2, double r) :
        arcCurveXStart(x1), arcCurveYStart(y1), arcCurveXEnd(x2), arcCurveYEnd(y2),
        radiusXStart(r), radiusYStart(r), radiusXControlOne(r), radiusYControlOne(r),
        radiusXControlTwo(r), radiusYControlTwo(r), radiusXEnd(r), radiusYEnd(r)
    {
        if (x1 > x2) {
            arcCurveXStart = x2;
            arcCurveXEnd = x1;
        }
        if (y1 > y2) {
            arcCurveYStart = y2;
            arcCurveYEnd = y1;
        }
    }

    void RoundedRect::Rect(double x1, double y1, double x2, double y2)
    {
        arcCurveXEnd = x2;
        arcCurveYEnd = y2;
        arcCurveXStart = x1;
        arcCurveYStart = y1;
        if (x1 > x2) {
            arcCurveXStart = x2;
            arcCurveXEnd = x1;
        }
        if (y1 > y2) {
            arcCurveYStart = y2;
            arcCurveYEnd = y1;
        }
    }

    void RoundedRect::Radius(double r)
    {
        radiusXStart = radiusYStart = radiusXControlOne = radiusYControlOne = radiusXControlTwo = radiusYControlTwo = radiusXEnd = radiusYEnd = r;
    }

    void RoundedRect::Radius(double rx, double ry)
    {
        radiusYStart = radiusYControlOne = radiusYControlTwo = radiusYEnd = ry;
        radiusXStart = radiusXControlOne = radiusXControlTwo = radiusXEnd = rx;
    }

    void RoundedRect::Radius(double rxBottom, double ryBottom,
                             double rxTop, double ryTop)
    {
        radiusXStart = radiusXControlOne = rxBottom;
        radiusXControlTwo = radiusXEnd = rxTop;
        radiusYStart = radiusYControlOne = ryBottom;
        radiusYControlTwo = radiusYEnd = ryTop;
    }

    void RoundedRect::Radius(double rx1,
                             double ry1,
                             double rx2,
                             double ry2,
                             double rx3, double ry3, double rx4, double ry4)
    {
        radiusXStart = rx1;
        radiusYStart = ry1;
        radiusXControlOne = rx2;
        radiusYControlOne = ry2;
        radiusXControlTwo = rx3;
        radiusYControlTwo = ry3;
        radiusXEnd = rx4;
        radiusYEnd = ry4;
    }

    void RoundedRect::NormalizeRadius()
    {
        double dx = std::fabs(arcCurveYEnd - arcCurveYStart);
        double dy = std::fabs(arcCurveXEnd - arcCurveXStart);

        double t = dx / (radiusXStart + radiusXControlOne);
        double k = 1.0;
        if (t < k)
            k = t;
        t = dx / (radiusXControlTwo + radiusXEnd);
        if (t < k)
            k = t;
        t = dy / (radiusYStart + radiusYControlOne);
        if (t < k)
            k = t;
        t = dy / (radiusYControlTwo + radiusYEnd);
        if (t < k)
            k = t;

        if (k < 1.0) {
            radiusXStart *= k;
            radiusYStart *= k;
            radiusXControlOne *= k;
            radiusYControlOne *= k;
            radiusXControlTwo *= k;
            radiusYControlTwo *= k;
            radiusXEnd *= k;
            radiusYEnd *= k;
        }
    }

    void RoundedRect::Rewind(unsigned)
    {
        arcCurveStatus = 0;
    }

    unsigned RoundedRect::Vertex(double* x, double* y)
    {
        unsigned cmd = PATH_CMD_STOP;
        switch (arcCurveStatus) {
            case VERTEX_STATUS:
                arcCurveObject.Init(arcCurveXStart + radiusXStart, arcCurveYStart + radiusYStart, radiusXStart, radiusYStart, PI, PI + PI * HALFNUM);
                arcCurveObject.Rewind(0);
                arcCurveStatus++;
            case VERTEX_STATUS1:
                cmd = arcCurveObject.Vertex(x, y);
                if (IsStop(cmd)) {
                    arcCurveStatus++;
                } else {
                    return cmd;
                }
            case VERTEX_STATUS2:
                arcCurveObject.Init(arcCurveXEnd - radiusXControlOne, arcCurveYStart + radiusYControlOne, radiusXControlOne, radiusYControlOne, PI + PI * HALFNUM, 0.0);
                arcCurveObject.Rewind(0);
                arcCurveStatus++;
            case VERTEX_STATUS3:
                cmd = arcCurveObject.Vertex(x, y);
                if (IsStop(cmd)) {
                    arcCurveStatus++;
                } else {
                    return PATH_CMD_LINE_TO;
                }
            case VERTEX_STATUS4:
                arcCurveObject.Init(arcCurveXEnd - radiusXControlTwo, arcCurveYEnd - radiusYControlTwo, radiusXControlTwo, radiusYControlTwo,
                                    0.0, PI * HALFNUM);
                arcCurveObject.Rewind(0);
                arcCurveStatus++;
            case VERTEX_STATUS5:
                cmd = arcCurveObject.Vertex(x, y);
                if (IsStop(cmd)) {
                    arcCurveStatus++;
                } else {
                    return PATH_CMD_LINE_TO;
                }
            case VERTEX_STATUS6:
                arcCurveObject.Init(arcCurveXStart + radiusXEnd, arcCurveYEnd - radiusYEnd, radiusXEnd, radiusYEnd, PI * HALFNUM, PI);
                arcCurveObject.Rewind(0);
                arcCurveStatus++;
            case VERTEX_STATUS7:
                cmd = arcCurveObject.Vertex(x, y);
                if (IsStop(cmd)) {
                    arcCurveStatus++;
                } else {
                    return PATH_CMD_LINE_TO;
                }
            case VERTEX_STATUS8:
                cmd = PATH_CMD_END_POLY | PATH_FLAGS_CLOSE | PATH_FLAGS_CLOCKWISE;
                arcCurveStatus++;
                break;
        }
        return cmd;
    }
#endif
} // namespace OHOS
