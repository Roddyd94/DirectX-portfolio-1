#pragma once
#include "AI/AIBlackboard.h"

class BossBlackboard : public AIBlackboard
{
public:
    BossBlackboard()           = default;
    ~BossBlackboard() override = default;

public:
    Vector2 jumpTargetDirection = Vector2::zero;

    int32 hp               = 3; //'000;
    int32 actionLeft       = 4;
    int32 targetFloor      = 0;
    int32 currentPaletteID = 0;
    int32 damageTimerID    = -1;
    int32 jumpTimerID      = -1;

    bool damaged = false;

public:
    inline static const Vector2 hopForce            = {0.f, 6.f};
    inline static const float   jumpEpsilon         = 0.0625f;
    inline static const float   targetFloorBoundary = -2.5f;
    inline static const float   jumpForceFloor0     = 7.f;
    inline static const float   jumpForceFloor1     = 12.f;
    inline static const float   jumpTimeFloor0      = 1.9866666666666666666f;
    inline static const float   jumpTimeFloor1      = 1.9066666666666666666f;

public:
    Vector2 GetTargetJumpForceX(float deltaX) const;
};
