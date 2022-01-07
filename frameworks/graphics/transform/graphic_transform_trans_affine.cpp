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
        data_[INDEX_ZERO] = src[INDEX_TWO] - src[INDEX_ZERO];
        data_[INDEX_THREE] = src[INDEX_THREE] - src[INDEX_ONE];
        data_[INDEX_ONE] = src[INDEX_FOUR] - src[INDEX_ZERO];
        data_[INDEX_FOUR] = src[INDEX_FIVE] - src[INDEX_ONE];
        data_[INDEX_TWO] = src[INDEX_ZERO];
        data_[INDEX_FIVE] = src[INDEX_ONE];
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
        float t0 = data_[INDEX_ZERO] * metrix.data_[INDEX_ZERO] + data_[INDEX_THREE] * metrix.data_[INDEX_ONE];
        float t2 = data_[INDEX_ONE] * metrix.data_[INDEX_ZERO] + data_[INDEX_FOUR] * metrix.data_[INDEX_ONE];
        float t4 = data_[INDEX_TWO] * metrix.data_[INDEX_ZERO] + data_[INDEX_FIVE] * metrix.data_[INDEX_ONE]
                + metrix.data_[INDEX_TWO];
        data_[INDEX_THREE] = data_[INDEX_ZERO] * metrix.data_[INDEX_THREE]
                + data_[INDEX_THREE] * metrix.data_[INDEX_FOUR];
        data_[INDEX_FOUR] = data_[INDEX_ONE] * metrix.data_[INDEX_THREE]
                + data_[INDEX_FOUR] * metrix.data_[INDEX_FOUR];
        data_[INDEX_FIVE] = data_[INDEX_TWO] * metrix.data_[INDEX_THREE] + data_[INDEX_FIVE] * metrix.data_[INDEX_FOUR]
                + metrix.data_[INDEX_FIVE];
        data_[INDEX_ZERO] = t0;
        data_[INDEX_ONE] = t2;
        data_[INDEX_TWO] = t4;
        return *this;
    }

    const TransAffine& TransAffine::Invert()
    {
        float d = DeterminantReciprocal();

        float t0 = data_[INDEX_FOUR] * d;
        data_[INDEX_FOUR] = data_[0] * d;
        data_[INDEX_THREE] = -data_[INDEX_THREE] * d;
        data_[INDEX_ONE] = -data_[INDEX_ONE] * d;

        float t4 = -data_[INDEX_TWO] * t0 - data_[INDEX_FIVE] * data_[INDEX_ONE];
        data_[INDEX_FIVE] = -data_[INDEX_TWO] * data_[INDEX_THREE] - data_[INDEX_FIVE] * data_[INDEX_FOUR];

        data_[INDEX_ZERO] = t0;
        data_[INDEX_TWO] = t4;
        return *this;
    }

    const TransAffine& TransAffine::Reset()
    {
        data_[INDEX_ONE] = 0;
        data_[INDEX_TWO] = 0;
        data_[INDEX_THREE] = 0;
        data_[INDEX_FIVE] = 0;
        data_[INDEX_SIX] = 0;
        data_[INDEX_SEVEN] = 0;
        data_[INDEX_ZERO] = 1;
        data_[INDEX_FOUR] = 1;
        data_[INDEX_EIGHT] = 1;
        return *this;
    }

    bool TransAffine::IsIdentity(float epsilon) const
    {
        return IsEqualEps(data_[INDEX_ZERO], 1.0f, epsilon) &&
               IsEqualEps(data_[INDEX_THREE], 0.0f, epsilon) &&
               IsEqualEps(data_[INDEX_ONE], 0.0f, epsilon) &&
               IsEqualEps(data_[INDEX_FOUR], 1.0f, epsilon) &&
               IsEqualEps(data_[INDEX_TWO], 0.0f, epsilon) &&
               IsEqualEps(data_[INDEX_FIVE], 0.0f, epsilon);
    }

    bool TransAffine::IsValid(float epsilon) const
    {
        return std::fabs(data_[INDEX_ZERO]) > epsilon && std::fabs(data_[INDEX_FOUR]) > epsilon;
    }
} // namespace OHOS
