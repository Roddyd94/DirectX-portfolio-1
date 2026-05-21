#pragma once
#include "SnowballState.h"
class SnowballStateRolling : public SnowballState
{
public:
    SnowballStateRolling(float direction);
    ~SnowballStateRolling() override = default;

private:
    float _direction  = 0.f;
    bool  _reinforced = false;

public:
    void Enter(Ptr<class SnowballComponent> snowball) override;
    void Exit(Ptr<class SnowballComponent> snowball) override;
    void Tick(Ptr<class SnowballComponent> snowball, float deltaTime) override;
};
