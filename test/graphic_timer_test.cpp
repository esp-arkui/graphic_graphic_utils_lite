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

#include <ctime>
#include <gtest/gtest.h>
#include <unistd.h>

#include "hal_tick.h"

using namespace testing::ext;
namespace {
constexpr int32_t NORMAL_PERIORD = 50;  // 50ms
constexpr int32_t NEGTIVE_PERIORD = -1; // -1ms
constexpr int32_t NONSENSE_PERIORD = 0; // 0ms
constexpr int32_t OVERFLOW_PERIORD = OHOS::GraphicTimer::MAX_PERIOD_MS + 1;
constexpr uint32_t NEAR_PEROID = 20; // 20ms

int32_t GetRandonPeriod()
{
    return rand() % OVERFLOW_PERIORD;
}
} // namespace

namespace OHOS {
class GraphicTimerTest : public testing::Test {
public:
    GraphicTimerTest() {}
    ~GraphicTimerTest() {}
    static void SetUpTestCase(void)
    {
        srand(time(0));
    }
    static void TearDownTestCase(void) {}
    void SetUp();
    void TearDown();
    static void TimerCb(void* arg)
    {
        GraphicTimerTest* t = reinterpret_cast<GraphicTimerTest*>(arg);
        t->cbCnt_++;
        t->lastPeriod_ = HALTick::GetInstance().GetElapseTime(t->lastStampMs_);
        t->lastStampMs_ = HALTick::GetInstance().GetTime();
    }

    int32_t cbCnt_ = 0;
    uint32_t lastStampMs_ = 0;
    uint32_t lastPeriod_ = 0;
};

void GraphicTimerTest::SetUp() {}

void GraphicTimerTest::TearDown() {}

HWTEST_F(GraphicTimerTest, GraphicTimerConstructTest, TestSize.Level0)
{
    GraphicTimer timer1(NEGTIVE_PERIORD, GraphicTimerTest::TimerCb, &timer1, false);
    EXPECT_EQ(timer1.Start(), false);

    GraphicTimer timer2(NONSENSE_PERIORD, GraphicTimerTest::TimerCb, &timer2, false);
    EXPECT_EQ(timer2.SetPeriod(NORMAL_PERIORD), false);

    GraphicTimer timer3(OVERFLOW_PERIORD, GraphicTimerTest::TimerCb, &timer3, false);
    EXPECT_EQ(timer3.Start(), false);

    GraphicTimer timer4(GetRandonPeriod(), GraphicTimerTest::TimerCb, &timer4);
    EXPECT_EQ(timer4.SetPeriod(NORMAL_PERIORD), true);
    EXPECT_EQ(timer4.Start(), true);
}

HWTEST_F(GraphicTimerTest, GraphicTimerIsPeriodTest, TestSize.Level0)
{
    GraphicTimer timer1(NORMAL_PERIORD, GraphicTimerTest::TimerCb, &timer1, true);
    EXPECT_EQ(timer1.IsPeriodic(), true);

    GraphicTimer timer2(NORMAL_PERIORD, GraphicTimerTest::TimerCb, &timer2, false);
    EXPECT_EQ(timer2.IsPeriodic(), false);
}

HWTEST_F(GraphicTimerTest, GraphicTimerSetPeriodTest, TestSize.Level0)
{
    GraphicTimer timer(NORMAL_PERIORD, GraphicTimerTest::TimerCb, &timer, false);
    EXPECT_EQ(timer.SetPeriod(OVERFLOW_PERIORD), false);
}

HWTEST_F(GraphicTimerTest, GraphicTimerSequence, TestSize.Level1)
{
    GraphicTimer timer(NORMAL_PERIORD, GraphicTimerTest::TimerCb, &timer, true);
    int32_t period = GraphicTimer::MAX_PERIOD_MS;
    EXPECT_EQ(timer.SetPeriod(period), true);
    EXPECT_EQ(timer.GetPeriod(), period);

    timer.Stop();
    timer.SetPeriod(NORMAL_PERIORD);

    lastStampMs_ = HALTick::GetInstance().GetTime();
    EXPECT_EQ(timer.Start(), true);
    usleep(NORMAL_PERIORD * 2);
    EXPECT_NEAR(lastPeriod_, NORMAL_PERIORD, NEAR_PEROID);

    int8_t repeat = 10; // repeat 10 times
    while (repeat-- < 0) {
        EXPECT_EQ(timer.Start(), true);
        usleep(NEAR_PEROID); // intercept 10ms
    }
    lastStampMs_ = HALTick::GetInstance().GetTime();
    usleep(NORMAL_PERIORD);
    timer.Stop();
    EXPECT_NEAR(lastPeriod_, NORMAL_PERIORD, NEAR_PEROID);

    cbCnt_ = 0;
    EXPECT_EQ(timer.Start(), true);
    timer.Stop();
    EXPECT_EQ(cbCnt_, 0);
}

HWTEST_F(GraphicTimerTest, GraphicTimerPrecision, TestSize.Level1)
{
    int8_t repeat = 10; // repeat 10 times
    GraphicTimer timer(NORMAL_PERIORD, GraphicTimerTest::TimerCb, &timer, true);
    EXPECT_EQ(timer.Start(), true);
    usleep(NORMAL_PERIORD * repeat);
    timer.Stop();
    EXPECT_NEAR(cbCnt_, repeat, 1); // 1 abs diff is allowed
}

HWTEST_F(GraphicTimerTest, GraphicTimerNonPeriodic, TestSize.Level1)
{
    GraphicTimer timer(NORMAL_PERIORD, GraphicTimerTest::TimerCb, &timer, false);
    EXPECT_EQ(timer.Start(), true);
    usleep(NORMAL_PERIORD * 3); // wait long enough
    EXPECT_EQ(cbCnt_, 1);       // timeout 1 time
}
} // namespace OHOS