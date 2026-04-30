#include "pch.h"

#include "TimeManager.h"
#include <utility>

bool TimeManager::Init()
{
    _prevTime = std::chrono::high_resolution_clock::now();

    return true;
}

void TimeManager::Destroy()
{
    for (auto& [_, timer] : _timers)
        DESTROY(timer);

    _timers.clear();
    _timersToRemove.clear();
}

float TimeManager::Tick()
{
    auto curTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> frameTime = curTime - _prevTime;

    _deltaTime = frameTime.count();
#ifdef _DEBUG
    _deltaTime = std::min(_deltaTime, _targetDeltaTime);
#endif // _DEBUG

    _prevTime = curTime;

    for (int32 id : _timersToRemove)
    {
        auto it = _timers.find(id);
        if (_timers.end() == it)
            continue;

        DESTROY(it->second);
        _timers.erase(id);
    }

    _timersToRemove.clear();

    for (auto& [id, timer] : _timers)
    {
        if (nullptr == timer)
        {
            RemoveTimer(id);
            continue;
        }

        timer->Tick(_deltaTime);
        if (!timer->IsRunning())
            RemoveTimer(id);
    }

    return _deltaTime;
}

float TimeManager::GetFPS()
{
    if (_deltaTime <= 0.f)
        return 0.f;

    return 1.f / _deltaTime;
}

void TimeManager::RemoveTimer(int32 id)
{
    _timersToRemove.push_back(id);
}
