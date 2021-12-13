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
        scaleX = src[2] - src[0];
        shearY = src[3] - src[1];
        shearX = src[4] - src[0];
        scaleY = src[5] - src[1];
        translateX = src[0];
        translateY = src[1];
        Invert();
        Multiply(TransAffine(dst[2] - dst[0], dst[3] - dst[1],
                             dst[4] - dst[0], dst[5] - dst[1],
                             dst[0], dst[1]));
        return *this;
    }

    const TransAffine& TransAffine::RectToParl(double x1, double y1,
                                               double x2, double y2,
                                               const double* parl)
    {
        double src[6];
        src[0] = x1;
        src[1] = y1;
        src[2] = x2;
        src[3] = y1;
        src[4] = x2;
        src[5] = y2;
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

    //------------------------------------------------------------------------
    bool TransAffine::IsValid(double epsilon) const
    {
        return std::fabs(scaleX) > epsilon && std::fabs(scaleY) > epsilon;
    }

} // namespace OHOS
