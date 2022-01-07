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
#include "gfx_utils/graphics/transform/graphic_transform_affine.h"

namespace OHOS {
    const TransAffine& TransAffine::ParlToParl(const float* src,
                                               const float* dst)
    {
        data_[0] = src[INDEX_TWO] - src[INDEX_ZERO];
        data_[3] = src[INDEX_THREE] - src[INDEX_ONE];
        data_[1] = src[INDEX_FOUR] - src[INDEX_ZERO];
        data_[4] = src[INDEX_FIVE] - src[INDEX_ONE];
        data_[2] = src[INDEX_ZERO];
        data_[5] = src[INDEX_ONE];
        Invert();
        Multiply(TransAffine(dst[INDEX_TWO] - dst[INDEX_ZERO], dst[INDEX_THREE] - dst[INDEX_ONE],
                             dst[INDEX_FOUR] - dst[INDEX_ZERO], dst[INDEX_FIVE] - dst[INDEX_ONE],
                             dst[INDEX_ZERO], dst[INDEX_ONE]));
        return *this;
    }

    const TransAffine& TransAffine::RectToParl(float x1, float y1,
                                               float x2, float y2,
                                               const float* parl)
    {
        const int parlIndexSize = 6;
        float src[parlIndexSize];
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
        float t0 = data_[0] * metrix.data_[0] + data_[3] * metrix.data_[1];
        float t2 = data_[1] * metrix.data_[0] + data_[4] * metrix.data_[1];
        float t4 = data_[2] * metrix.data_[0] + data_[5] * metrix.data_[1] + metrix.data_[2];
        data_[3] = data_[0] * metrix.data_[3] + data_[3] * metrix.data_[4];
        data_[4] = data_[1] * metrix.data_[3] + data_[4] * metrix.data_[4];
        data_[5] = data_[2] * metrix.data_[3] + data_[5] * metrix.data_[4] + metrix.data_[5];
        data_[0] = t0;
        data_[1] = t2;
        data_[2] = t4;
        return *this;
    }

    const TransAffine& TransAffine::Invert()
    {
        float d = DeterminantReciprocal();

        float t0 = data_[4] * d;
        data_[4] = data_[0] * d;
        data_[3] = -data_[3] * d;
        data_[1] = -data_[1] * d;

        float t4 = -data_[2] * t0 - data_[5] * data_[1];
        data_[5] = -data_[2] * data_[3] - data_[5] * data_[4];

        data_[0] = t0;
        data_[2] = t4;
        return *this;
    }

    const TransAffine& TransAffine::Reset()
    {
        data_[1] = 0;
        data_[2] = 0;
        data_[3] = 0;
        data_[5] = 0;
        data_[6] = 0;
        data_[7] = 0;
        data_[0] = 1;
        data_[4] = 1;
        data_[8] = 1;
        return *this;
    }

    bool TransAffine::IsIdentity(float epsilon) const
    {
        return IsEqualEps(data_[0], 1.0f, epsilon) &&
               IsEqualEps(data_[3], 0.0f, epsilon) &&
               IsEqualEps(data_[1], 0.0f, epsilon) &&
               IsEqualEps(data_[4], 1.0f, epsilon) &&
               IsEqualEps(data_[2], 0.0f, epsilon) &&
               IsEqualEps(data_[5], 0.0f, epsilon);
    }

    bool TransAffine::IsValid(float epsilon) const
    {
        return std::fabs(data_[0]) > epsilon && std::fabs(data_[4]) > epsilon;
    }
} // namespace OHOS
