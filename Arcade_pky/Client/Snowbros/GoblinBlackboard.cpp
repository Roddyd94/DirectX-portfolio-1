#include "pch.h"

#include "GoblinBlackboard.h"

#include "Platformer/PlatformerKinematicComponent.h"

void GoblinBlackboard::Destroy()
{
    AIBlackboard::Destroy();
}

void GoblinBlackboard::SetDirection(float direction)
{
    this->direction = direction;
}

Vector2 GoblinBlackboard::GetTargetJumpForce() const
{
    float a = -PlatformerKinematicComponent::gravity;
    float y = jumpTargetDirection.y + jumpEpsilon;

    float c = std::sqrt(std::abs(2 * a * y));
    float t = c / a;

    return {jumpTargetDirection.x * 0.83333333333333333333f / t, c};
}
