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

    float speedX          = 2.f;
    float speedMultiplier = 1.f;

    bool jumpedFromSnowball = false;

    bool speedUpgraded = false;
    bool powerUpgraded = false;
    bool rangeUpgraded = false;
    bool invincible    = false;

public:
    inline static const float speedMultiplierSnowball = 0.5f;
    inline static const float invincibleTime = 5.f;
};
