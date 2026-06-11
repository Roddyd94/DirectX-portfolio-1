#pragma once
#include "AI/AIBlackboard.h"

class BossBlackboard : public AIBlackboard
{
public:
    BossBlackboard()           = default;
    ~BossBlackboard() override = default;

public:
    Vector2 jumpTargetDirection = Vector2::zero;

    int32 hp               = 3'000;
    int32 actionLeft       = 4;
    int32 targetFloor      = 0;
    int32 spawnPattern     = 0;
    int32 currentPaletteID = 0;
    int32 damageTimerID    = -1;
    int32 jumpTimerID      = -1;

    bool damaged       = false;
    bool spawnedTop    = false;
    bool spawnedMiddle = false;
    bool spawnedBottom = false;

public:
    inline static const Vector2 hopForce                       = {0.f, 6.f};
    inline static const Vector2 spawnPositionDeltaFromLandSpot = {};

    inline static const float jumpEpsilon             = 0.0625f;
    inline static const float targetFloorBoundary     = -2.5f;
    inline static const float jumpForceFloor0         = 8.5f;
    inline static const float jumpForceFloor1         = 13.f;
    inline static const float jumpTimeFloor0          = 2.1466666666666666666f;
    inline static const float jumpTimeFloor1          = 2.2533333333333333333f;
    inline static const float spawnLandPositionXLeft  = -5.5f;
    inline static const float spawnLandPositionXRight = 2.f;
    inline static const float spawnPositionYTop       = 2.f;
    inline static const float spawnPositionYMiddle    = -1.f;
    inline static const float spawnPositionYBottom    = -5.f;
    inline static const float spawnPattern1Bias       = -2.f;

public:
    Vector2 GetTargetJumpForceX(float deltaX) const;
};
