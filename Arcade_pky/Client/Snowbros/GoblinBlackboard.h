#pragma once
#include "AI/AIBlackboard.h"

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
    bool isOnFloor = false;
    bool turned    = false;

public:
    inline static const float jumpTime      = 0.2f;
    inline static const float dizzyTime     = 1.f;
    inline static const float walkSpeedX    = 1.f;
    inline static const float jumpForceFull = 6.625f;
    inline static const float jumpForceHalf = 6.625f;
    inline static const float jumpEpsilon   = 0.2f;

public:
    void Destroy() override;
    void SetDirection(float direction) override;

    Vector2 GetTargetJumpForce() const;
};
