#pragma once
#include "SnowballState.h"

class SnowballStateFormed : public SnowballState
{
public:
    SnowballStateFormed();
    ~SnowballStateFormed() override = default;

private:
    inline static const float decPerSecond     = 5.f;
    inline static const float formedBonusValue = 30.f;
    inline static const float formedIncValue   = 3.f;
    inline static const float moveDeltaX       = 0.0625f;

public:
    void Enter(Ptr<class SnowballComponent> snowball) override;
    void Exit(Ptr<class SnowballComponent> snowball) override;
    void Tick(Ptr<class SnowballComponent> snowball, float deltaTime) override;

    bool TryKick(Ptr<class SnowballComponent> snowball, float direction) override;
    bool TryPush(Ptr<class SnowballComponent> snowball, float direction) override;
    void CollideWith(Ptr<class SnowballComponent> snowball,
      CollisionState::Type                        collisionState,
      Weak<class CollisionComponent>              collider) override;
};
