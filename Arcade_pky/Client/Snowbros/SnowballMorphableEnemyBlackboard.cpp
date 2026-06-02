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

Vector2 SnowballMorphableEnemyBlackboard::CalculateForceToTarget(Vector2 targetDelta)
{
    float a = -PlatformerKinematicComponent::gravity;
    float y = targetDelta.y;

    float c = std::sqrt(std::abs(2 * a * y));
    float t = c / a;

    return {targetDelta.x * 0.75f / t, c};
}
