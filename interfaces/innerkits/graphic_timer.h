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

#ifndef GRAPHIC_LITE_TIMER_H
#define GRAPHIC_LITE_TIMER_H

#include <cstdint>
#ifndef _WIN32
#include <ctime>
#endif

namespace OHOS {
class GraphicTimer final {
public:
    using GraphicTimerCb = void (*)(void*);
    GraphicTimer(int32_t periodMs, GraphicTimerCb cb, void* arg, bool isPeriodic = false);
    ~GraphicTimer();

    /* delayMs is ignored when timer is non-periodic */
    bool Start(int32_t delayMs = 0);
    bool SetPeriod(int32_t periodMs);
    void Stop();
    bool IsPeriodic()
    {
        return isPeriodic_;
    }
    void Callback()
    {
        if (cb_ != nullptr) {
            cb_(arg_);
        }
    }

    static constexpr int32_t MAX_PERIOD_MS = 36E5;

private:
    int32_t periodMs_ = -1;
    GraphicTimerCb cb_ = nullptr;
    void* arg_ = nullptr;
    bool isPeriodic_ = false;

#ifdef _WIN32
    void* timer_ = nullptr;
#else
    timer_t timer_;
#endif

    GraphicTimer() = delete;
    GraphicTimer(const GraphicTimer&) = delete;
    GraphicTimer& operator=(const GraphicTimer&) = delete;
    GraphicTimer(GraphicTimer&&) = delete;
    GraphicTimer& operator=(GraphicTimer&&) = delete;
};
};     // namespace OHOS
#endif // GRAPHIC_LITE_TIMER_H
