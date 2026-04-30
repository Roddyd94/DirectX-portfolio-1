#include "pch.h"
#include "Timer.h"

void Timer::Tick(float deltaTime)
{
    if (!_isRunning)
        return;

    _accTime += deltaTime;
    if (_accTime < _expireTime)
        return;

    _accTime -= _expireTime;
    if (_callback)
        _callback();

    if (!_shouldRepeat)
        _isRunning = false;
}

bool Timer::IsRunning() const
{
    return _isRunning;
}

void Timer::Stop()
{
    _isRunning = false;
    _accTime = 0.f;
}

void Timer::Destroy()
{

}