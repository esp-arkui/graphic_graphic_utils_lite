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
        scaleX_ = src[INDEX_TWO] - src[INDEX_ZERO];
        shearY = src[INDEX_THREE] - src[INDEX_ONE];
        shearX = src[INDEX_FOUR] - src[INDEX_ZERO];
        scaleY_ = src[INDEX_FIVE] - src[INDEX_ONE];
        translateX = src[INDEX_ZERO];
        translateY = src[INDEX_ONE];
        Invert();
        Multiply(TransAffine(dst[INDEX_TWO] - dst[INDEX_ZERO], dst[INDEX_THREE] - dst[INDEX_ONE],
                             dst[INDEX_FOUR] - dst[INDEX_ZERO], dst[INDEX_FIVE] - dst[INDEX_ONE],
                             dst[INDEX_ZERO], dst[INDEX_ONE]));
        return *this;
    }

    const TransAffine& TransAffine::RectToParl(double x1, double y1,
                                               double x2, double y2,
                                               const double* parl)
    {
        const int parlIndexSize = 6;
        double src[parlIndexSize];
        src[INDEX_ZERO] = x1;
        src[INDEX_ONE] = y1;
        src[INDEX_TWO] = x2;
        src[INDEX_THREE] = y1;
        src[INDEX_FOUR] = x2;
        src[INDEX_FIVE] = y2;
        ParlToParl(src, parl);
        return *this;
    }

    const TransAffine& TransAffine::Multiply(const TransAffine& metrix)
    {
        double t0 = scaleX_ * metrix.scaleX_ + shearY * metrix.shearX;
        double t2 = shearX * metrix.scaleX_ + scaleY_ * metrix.shearX;
        double t4 = translateX * metrix.scaleX_ + translateY * metrix.shearX + metrix.translateX;
        shearY = scaleX_ * metrix.shearY + shearY * metrix.scaleY_;
        scaleY_ = shearX * metrix.shearY + scaleY_ * metrix.scaleY_;
        translateY = translateX * metrix.shearY + translateY * metrix.scaleY_ + metrix.translateY;
        scaleX_ = t0;
        shearX = t2;
        translateX = t4;
        return *this;
    }

    const TransAffine& TransAffine::Invert()
    {
        double d = DeterminantReciprocal();

        double t0 = scaleY_ * d;
        scaleY_ = scaleX_ * d;
        shearY = -shearY * d;
        shearX = -shearX * d;

        double t4 = -translateX * t0 - translateY * shearX;
        translateY = -translateX * shearY - translateY * scaleY_;

        scaleX_ = t0;
        translateX = t4;
        return *this;
    }

    const TransAffine& TransAffine::Reset()
    {
        scaleX_ = scaleY_ = 1.0;
        shearY = shearX = translateX = translateY = 0.0;
        return *this;
    }

    bool TransAffine::IsIdentity(double epsilon) const
    {
        return IsEqualEps(scaleX_, 1.0, epsilon) &&
               IsEqualEps(shearY, 0.0, epsilon) &&
               IsEqualEps(shearX, 0.0, epsilon) &&
               IsEqualEps(scaleY_, 1.0, epsilon) &&
               IsEqualEps(translateX, 0.0, epsilon) &&
               IsEqualEps(translateY, 0.0, epsilon);
    }

    bool TransAffine::IsValid(double epsilon) const
    {
        return std::fabs(scaleX_) > epsilon && std::fabs(scaleY_) > epsilon;
    }
} // namespace OHOS
