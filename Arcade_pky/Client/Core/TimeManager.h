#pragma once
#include "Singleton.h"
#include "Timer.h"

#include <chrono>

class TimeManager : public Singleton<TimeManager>
{
    DECLARE_SINGLETON(TimeManager)
    using TimePoint = std::chrono::high_resolution_clock::time_point;

public:
    inline static const float targetDeltaTime = 0.01333333333333333333333333333f;
    inline static const float targetFPS       = 75.f;

private:
    std::map<int32, Ptr<class Timer>> _timers;
    std::vector<int32>                _timersToRemove;

    TimePoint _prevTime;
    float     _deltaTime      = 0.f;
    int32     _frameCounter   = 0;
    int32     _timerIDCounter = 0;

public:
    bool  Init();
    void  Destroy();
    float Tick();

    int32 GetFrameCount() const;
    float GetDeltaTime() const { return _deltaTime; }
    float GetFPS();

    void RemoveTimer(int32 id);

    template <typename T>
    int32 SetTimer(float expireTime, bool repeat, T&& func)
    {
        int32      id    = _timerIDCounter++;
        Ptr<Timer> timer = New<Timer>();
        timer->SetTimer(expireTime, repeat, std::forward<T>(func));
        _timers[id] = timer;

        return id;
    }

    template <typename T>
    int32 SetTimer(float expireTime, bool repeat, T* obj, void (T::*func)())
    {
        int32      id    = _timerIDCounter++;
        Ptr<Timer> timer = New<Timer>();
        timer->SetTimer(expireTime, repeat, obj, func);
        _timers[id] = timer;

        return id;
    }
};
