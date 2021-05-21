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

#include "gfx_utils/graphic_log.h"
#ifdef _WIN32
#include "windows.h"
#else
#include <csignal>
#include <cstring>
#include <errno.h>
#endif

namespace {
constexpr int16_t MS_PER_SECOND = 1000;
constexpr int32_t NS_PER_MS = 1000000;
constexpr int32_t HUNDRED_NS_PER_MS = 10000;
} // namespace

namespace OHOS {
bool GraphicTimer::SetPeriod(int32_t periodMs)
{
    if (periodMs_ < 0) {
        GRAPHIC_LOGE("Timer set period failed, timer should be created first.");
        return false;
    }

    if ((periodMs > MAX_PERIOD_MS) || (periodMs <= 0)) {
        GRAPHIC_LOGE("Timer set period failed, period should be within (0, %d].(period=%d)", MAX_PERIOD_MS, periodMs);
        return false;
    }

    periodMs_ = periodMs;
    return true;
}

#ifdef _WIN32
static void CALLBACK TimerCallback(LPVOID lpArg, DWORD dwTimerLowValue, DWORD dwTimerHighValue)
{
    UNREFERENCED_PARAMETER(dwTimerLowValue);
    UNREFERENCED_PARAMETER(dwTimerHighValue);

    GraphicTimer* timer = reinterpret_cast<GraphicTimer*>(lpArg);
    if (timer != nullptr) {
        (void)timer->Callback();
    }
}

static DWORD WINAPI WinAsyncThread(LPVOID lpParam)
{
    GraphicTimer* timer = (GraphicTimer*)lpParam;
    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -(timer->GetPeriod() * HUNDRED_NS_PER_MS);

    LONG period = (timer->IsPeriodic() ? timer->GetPeriod() : 0);
    if (!SetWaitableTimer(timer->GetNativeTimer(), &liDueTime, period, TimerCallback, timer, FALSE)) {
        GRAPHIC_LOGE("Timer start failed.(Error=%d)", GetLastError());
        return 1;
    }
    SleepEx(INFINITE, TRUE);
    return 0;
}

GraphicTimer::GraphicTimer(int32_t periodMs, GraphicTimerCb cb, void* arg, bool isPeriodic)
    : cb_(cb), arg_(arg), isPeriodic_(isPeriodic)
{
    if ((periodMs > MAX_PERIOD_MS) || (periodMs <= 0)) {
        GRAPHIC_LOGE("Timer create failed, period should be within (0, %d].(period=%d)", MAX_PERIOD_MS, periodMs);
        return;
    }

    timer_ = CreateWaitableTimer(nullptr, TRUE, nullptr);
    if (timer_ == nullptr) {
        GRAPHIC_LOGE("Timer create failed.(err=%s)", strerror(errno));
        return;
    }
    periodMs_ = periodMs;
}

GraphicTimer::~GraphicTimer()
{
    if (timer_ != nullptr) {
        CloseHandle(timer_);
    }
}

bool GraphicTimer::Start()
{
    if (timer_ == nullptr) {
        GRAPHIC_LOGE("Timer start failed, timer should be created first.");
        return false;
    }

    DWORD dwThreadId;
    HANDLE hThread;
    hThread = CreateThread(NULL,           // default security attributes
                           0,              // use default stack size
                           WinAsyncThread, // thread function name
                           this,           // argument to thread function
                           0,              // use default creation flags
                           &dwThreadId);   // returns the thread identifier

    if (hThread == nullptr) {
        GRAPHIC_LOGE("Timer start failed.(Error=%d)", GetLastError());
        return false;
    }
    return true;
}

void GraphicTimer::Stop()
{
    if (timer_ == nullptr) {
        GRAPHIC_LOGE("Timer stop failed, timer should be created first.");
        return;
    }
    if (CancelWaitableTimer(timer_) == 0) {
        GRAPHIC_LOGE("Timer stop failed.(Error=%d)", GetLastError());
        return;
    }
}

#else
static void TimerCallback(union sigval v)
{
    GraphicTimer* timer = reinterpret_cast<GraphicTimer*>(v.sival_ptr);
    if (timer != nullptr) {
        timer->Callback();
    }
    if (timer->IsPeriodic()) {
        timer->Start();
    }
}

GraphicTimer::GraphicTimer(int32_t periodMs, GraphicTimerCb cb, void* arg, bool isPeriodic)
    : cb_(cb), arg_(arg), isPeriodic_(isPeriodic)
{
    if ((periodMs > MAX_PERIOD_MS) || (periodMs <= 0)) {
        GRAPHIC_LOGE("Timer create failed, period should be within (0, %d].(period=%d)", MAX_PERIOD_MS, periodMs);
        return;
    }

    struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = TimerCallback;
    sev.sigev_value.sival_ptr = this;
    if (timer_create(CLOCK_REALTIME, &sev, &timer_) == -1) {
        GRAPHIC_LOGE("Timer create failed.(err=%s)", strerror(errno));
        return;
    }
    periodMs_ = periodMs;
}

GraphicTimer::~GraphicTimer()
{
    if (periodMs_ >= 0) {
        if (timer_delete(timer_) == -1) {
            GRAPHIC_LOGE("Timer delete failed.(err=%s)", strerror(errno));
        }
    }
}

bool GraphicTimer::Start()
{
    if (periodMs_ < 0) {
        GRAPHIC_LOGE("Timer start failed, timer should be created first.");
        return false;
    }

    struct itimerspec its;
    its.it_interval.tv_nsec = periodMs_ % NS_PER_MS;
    its.it_interval.tv_sec = periodMs_ / MS_PER_SECOND;
    its.it_value.tv_nsec = its.it_interval.tv_nsec;
    its.it_value.tv_sec = its.it_interval.tv_sec;

    if (timer_settime(timer_, 0, &its, nullptr) == -1) {
        GRAPHIC_LOGE("Timer start failed.(timerid=%d, err=%s)", timer_, strerror(errno));
        return false;
    }
    return true;
}

void GraphicTimer::Stop()
{
    if (periodMs_ < 0) {
        return;
    }
    struct itimerspec its;
    its.it_value.tv_nsec = 0;
    its.it_value.tv_sec = 0;
    if (timer_settime(timer_, 0, &its, nullptr) == -1) {
        GRAPHIC_LOGE("Timer stop failed.(timerid=%d, err=%s)", timer_, strerror(errno));
        return;
    }
}
#endif
}; // namespace OHOS
