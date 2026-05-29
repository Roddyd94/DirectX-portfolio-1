#pragma once
#include "Core/Types.h"
#include "Player/PlayerBlackboard.h"

class SnowbrosPlayerBlackboard : public PlayerBlackboard
{
public:
    SnowbrosPlayerBlackboard()           = default;
    ~SnowbrosPlayerBlackboard() override = default;

public:
    Weak<class CollisionComponent> bindTargetSnowball;

    float speedX             = 2.f;
    bool  jumpedFromSnowball = false;

public:
    inline static const float speedMultiplierSnowball = 0.5f;
};
