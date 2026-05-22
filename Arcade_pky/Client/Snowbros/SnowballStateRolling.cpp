#include "pch.h"

#include "SnowballStateRolling.h"

#include "Core/Collision/CollisionComponent.h"
#include "Core/Collision/CollisionProfile.h"

SnowballStateRolling::SnowballStateRolling(float direction)
{
    _direction = direction;
    _stateType = SnowballStateType::Rolling;
}

void SnowballStateRolling::Enter(Ptr<class SnowballComponent> snowball) {}

void SnowballStateRolling::Exit(Ptr<class SnowballComponent> snowball) {}

void SnowballStateRolling::Tick(Ptr<class SnowballComponent> snowball, float deltaTime)
{
    // todo rolling
}
