#pragma once
#include "Timer.h"
#include "Singleton.h"

#include <chrono>

class TimeManager : public Singleton<TimeManager>
{
    DECLARE_SINGLETON(TimeManager)
    using TimePoint = std::chrono::high_resolution_clock::time_point;

private:
    inline static const float _targetDeltaTime = 0.0069444444f;

private:
    std::map<int32, Ptr<class Timer>> _timers;
    std::vector<int32>                _timersToRemove;

    TimePoint _prevTime;
    float     _deltaTime      = 0.f;
    int32     _timerIDCounter = 0;

public:
    bool  Init();
    void  Destroy();
    float Tick();

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
