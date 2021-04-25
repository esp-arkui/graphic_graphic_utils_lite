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

#include "graphic_timer.h"

#include <gtest/gtest.h>

using namespace testing::ext;
namespace OHOS {
class GraphicTimerTest : public testing::Test {
public:
    GraphicTimerTest() {}
    ~GraphicTimerTest() {}
    static void SetUpTestCase(void) {}
    static void TearDownTestCase(void) {}
    void SetUp();
    void TearDown();
};

void GraphicTimerTest::SetUp() {}

void GraphicTimerTest::TearDown() {}

HWTEST_F(GraphicTimerTest, ListFront_001, TestSize.Level0)
{
    EXPECT_EQ(list_->Front(), FIRST_VALUE);
}
} // namespace OHOS