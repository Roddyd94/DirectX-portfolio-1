#pragma once
#include "AI/AIBlackboard.h"

class BossBlackboard : public AIBlackboard
{
public:
    BossBlackboard()           = default;
    ~BossBlackboard() override = default;

public:
    Vector2 previousDelta       = Vector2::zero;
    Vector2 jumpTargetDirection = Vector2::zero;

    int32 hp          = 3'000;
    int32 targetFloor = 0;
    int32 temp        = 0;
    int32 loopTimerID = -1;

    bool isJumping = false;

public:
    inline static const float jumpEpsilon = 0.0625f;

public:
    Vector2 GetTargetJumpForce() const;
};
