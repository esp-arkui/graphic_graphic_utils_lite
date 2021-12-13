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
#include "gfx_utils/graphics/graphic_transform/graphic_transform_affine.h"

namespace OHOS {
    const TransAffine& TransAffine::ParlToParl(const double* src,
                                               const double* dst)
    {
        scaleX = src[parlIndex2] - src[parlIndex0];
        shearY = src[parlIndex3] - src[parlIndex1];
        shearX = src[parlIndex4] - src[parlIndex0];
        scaleY = src[parlIndex5] - src[parlIndex1];
        translateX = src[parlIndex0];
        translateY = src[parlIndex1];
        Invert();
        Multiply(TransAffine(dst[parlIndex2] - dst[parlIndex0], dst[parlIndex3] - dst[parlIndex1],
                             dst[parlIndex4] - dst[parlIndex0], dst[parlIndex5] - dst[parlIndex1],
                             dst[parlIndex0], dst[parlIndex1]));
        return *this;
    }

    const TransAffine& TransAffine::RectToParl(double x1, double y1,
                                               double x2, double y2,
                                               const double* parl)
    {
        double src[parlIndexSize];
        src[parlIndex0] = x1;
        src[parlIndex1] = y1;
        src[parlIndex2] = x2;
        src[parlIndex3] = y1;
        src[parlIndex4] = x2;
        src[parlIndex5] = y2;
        ParlToParl(src, parl);
        return *this;
    }

    const TransAffine& TransAffine::Multiply(const TransAffine& metrix)
    {
        double t0 = scaleX * metrix.scaleX + shearY * metrix.shearX;
        double t2 = shearX * metrix.scaleX + scaleY * metrix.shearX;
        double t4 = translateX * metrix.scaleX + translateY * metrix.shearX + metrix.translateX;
        shearY = scaleX * metrix.shearY + shearY * metrix.scaleY;
        scaleY = shearX * metrix.shearY + scaleY * metrix.scaleY;
        translateY = translateX * metrix.shearY + translateY * metrix.scaleY + metrix.translateY;
        scaleX = t0;
        shearX = t2;
        translateX = t4;
        return *this;
    }

    const TransAffine& TransAffine::Invert()
    {
        double d = DeterminantReciprocal();

        double t0 = scaleY * d;
        scaleY = scaleX * d;
        shearY = -shearY * d;
        shearX = -shearX * d;

        double t4 = -translateX * t0 - translateY * shearX;
        translateY = -translateX * shearY - translateY * scaleY;

        scaleX = t0;
        translateX = t4;
        return *this;
    }

    const TransAffine& TransAffine::Reset()
    {
        scaleX = scaleY = 1.0;
        shearY = shearX = translateX = translateY = 0.0;
        return *this;
    }

    
    bool TransAffine::IsIdentity(double epsilon) const
    {
        return IsEqualEps(scaleX, 1.0, epsilon) &&
               IsEqualEps(shearY, 0.0, epsilon) &&
               IsEqualEps(shearX, 0.0, epsilon) &&
               IsEqualEps(scaleY, 1.0, epsilon) &&
               IsEqualEps(translateX, 0.0, epsilon) &&
               IsEqualEps(translateY, 0.0, epsilon);
    }

    bool TransAffine::IsValid(double epsilon) const
    {
        return std::fabs(scaleX) > epsilon && std::fabs(scaleY) > epsilon;
    }

} // namespace OHOS
