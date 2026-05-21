#pragma once
#include "SnowballState.h"

constexpr size_t snowballStateFormingPhaseCount = 4;

class SnowballStateForming : public SnowballState
{
public:
    SnowballStateForming(float initialValue = 2.f);
    ~SnowballStateForming() override = default;

private:
    float _accValue = 0.f;

public:
    inline static const float phaseThreshold[snowballStateFormingPhaseCount] = {0.f, 12.f, 18.5f, 23.f};

private:
    inline static const float decPerSecond    = 5.f;
    inline static const float formingIncValue = 5.f;

public:
    void Enter(Ptr<class SnowballComponent> snowball) override;
    void Exit(Ptr<class SnowballComponent> snowball) override;
    void Tick(Ptr<class SnowballComponent> snowball, float deltaTime) override;

    void CollideWith(Ptr<class SnowballComponent> snowball, Weak<class CollisionComponent> collider) override;
};
