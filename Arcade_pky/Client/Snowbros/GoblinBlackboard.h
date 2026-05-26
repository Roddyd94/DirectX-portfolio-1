#pragma once
#include "AI/AIBlackboard.h"

constexpr size_t snowballStateFormingPhaseCount = 4;

class GoblinBlackboard : public AIBlackboard
{
public:
    GoblinBlackboard()           = default;
    ~GoblinBlackboard() override = default;

public:
    Weak<class Snowball> snowball;

    Vector2 previousDelta       = Vector2::zero;
    Vector2 jumpTargetDirection = Vector2::zero;

    float direction = -1.f;
    float accTime   = 0.f;

    bool isJumping = false;
    bool turned    = false;

public:
    inline static const float phaseThreshold[snowballStateFormingPhaseCount]
      = {0.f, 12.f, 18.5f, 23.f};

    inline static const float jumpTime      = 0.2f;
    inline static const float dizzyTime     = 1.f;
    inline static const float walkSpeedX    = 1.f;
    inline static const float jumpForceFull = 6.625f;
    inline static const float jumpForceHalf = 6.625f;
    inline static const float jumpEpsilon   = 0.2f;

    inline static const float snowballDecPerSecond        = 5.f;
    inline static const float snowballIncForming          = 5.f;
    inline static const float snowballIncFormed           = 3.f;
    inline static const float snowballFormingInitialValue = 2.f;
    inline static const float snowballFormedBonusValue    = 30.f;

public:
    void Destroy() override;
    void SetDirection(float direction) override;

    Vector2 GetTargetJumpForce() const;
};
