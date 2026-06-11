#include "pch.h"

#include "BossBlackboard.h"

#include "Platformer/PlatformerKinematicComponent.h"

Vector2 BossBlackboard::GetTargetJumpForceX(float deltaX) const
{
    if (targetFloor == 1)
        return {deltaX / jumpTimeFloor1, jumpForceFloor1};
    else
        return {deltaX / jumpTimeFloor0, jumpForceFloor0};
}
