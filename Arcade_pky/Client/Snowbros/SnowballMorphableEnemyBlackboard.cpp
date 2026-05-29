#include "pch.h"

#include "SnowballMorphableEnemyBlackboard.h"

#include "Platformer/PlatformerKinematicComponent.h"

Vector2 SnowballMorphableEnemyBlackboard::GetTargetJumpForce() const
{
    float a = -PlatformerKinematicComponent::gravity;
    float y = jumpTargetDirection.y + jumpEpsilon;

    float c = std::sqrt(std::abs(2 * a * y));
    float t = c / a;

    return {jumpTargetDirection.x * 0.75f / t, c};
}
