#pragma once
#include "Object.h"

class Timer : public Object
{
public:
    Timer()           = default;
    ~Timer() override = default;
    DELETE_SPECIAL_FUNC(Timer)

private:
    std::function<void()> _callback;
    float                 _expireTime   = 0.f;
    float                 _accTime      = 0.f;
    bool                  _isRunning    = false;
    bool                  _shouldRepeat = false;

public:
    void Destroy() override;
    void Tick(float deltaTime);

    bool IsRunning() const;
    void Stop();

    template <typename T>
    void SetTimer(float expireTime, bool repeat, T&& func)
    {
        if (_isRunning)
            return;

        _expireTime   = expireTime;
        _shouldRepeat = repeat;
        _callback     = std::forward<T>(func);
        _accTime      = 0.f;
        _isRunning    = true;
    }

    template <typename T>
    void SetTimer(float expireTime, bool repeat, T* obj, void (T::*func)())
    {
        if (_isRunning)
            return;

        _expireTime   = expireTime;
        _shouldRepeat = repeat;
        _callback     = std::bind(func, obj);
        _accTime      = 0.f;
        _isRunning    = true;
    }
};
