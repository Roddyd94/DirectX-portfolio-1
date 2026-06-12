#pragma once
#include "AI/AIBlackboard.h"

constexpr size_t snowballStateFormingPhaseCount = 4;

class SnowballMorphableEnemyBlackboard : public AIBlackboard
{
public:
    SnowballMorphableEnemyBlackboard()           = default;
    ~SnowballMorphableEnemyBlackboard() override = default;

public:
    std::vector<Vector2> patrolPoints;

    Vector2 previousPosition    = Vector2::zero;
    Vector2 jumpTargetDirection = Vector2::zero;

    int32 patrolLoopCount      = 0;
    int32 currentPatrolIndex   = 0;
    int32 hitCount             = 0;
    int32 snowballKickedPlayer = 0;
    int32 airborneTimerID      = -1;

    float walkSpeedX = 1.f;
    float direction  = -1.f;
    float accTime    = 0.f;

    bool turned               = false;
    bool hitByReinforced      = false;
    bool isJumping            = false;
    bool isSnowballReinforced = false;
    bool isSnowballFalling    = false;

public:
    inline static const float phaseThreshold[snowballStateFormingPhaseCount]
      = {0.f, 12.f, 18.5f, 23.f};

    inline static const float dizzyTime     = 1.f;
    inline static const float jumpTime      = 0.2f;
    inline static const float jumpForceFull = 6.625f;
    inline static const float jumpForceHalf = 6.625f;
    inline static const float jumpEpsilon   = 0.0625f;
    inline static const float airborneTime  = 2.f;

    inline static const float snowballDragFloor           = 0.9f;
    inline static const float snowballFrameDistance       = 0.125f;
    inline static const float snowballDecPerSecond        = 5.f;
    inline static const float snowballIncForming          = 5.f;
    inline static const float snowballIncFormed           = 3.f;
    inline static const float snowballFormingInitialValue = 2.f;
    inline static const float snowballFormedBonusValue    = 30.f;
    inline static const float snowballHeadingForceX       = 3.f;
    inline static const float snowballRepulsiveDeltaX     = 0.1f;
    inline static const float snowballRepulsiveDistance   = 0.9f;
    inline static const float snowballRollingSpeedX       = 10.f;

public:
    Vector2 GetTargetJumpForce() const;

public:
    static Vector2 CalculateForceToTarget(Vector2 targetDelta);
};
