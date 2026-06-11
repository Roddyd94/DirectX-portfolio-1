#pragma once
#include "Core/TimeManager.h"

#include "SnowbrosEnemyState.h"

class BossState : public SnowbrosEnemyState
{
public:
    BossState() = default;
    BossState(EnemyStateType stateType);
    ~BossState() override = default;

protected:
    std::function<void(bool)> _callbackSpriteEffect;

public:
    void OnTick(float deltaTime) override;

public:
    template <typename T>
    void RegisterCallbackSpriteEffect(T&& func)
    {
        _callbackSpriteEffect = std::forward<T>(func);
    }

    template <typename T>
    void RegisterCallbackSpriteEffect(T* obj, void (T::*memFunc)())
    {
        _callbackSpriteEffect = std::bind(memFunc, obj, std::placeholders::_1);
    }
};
