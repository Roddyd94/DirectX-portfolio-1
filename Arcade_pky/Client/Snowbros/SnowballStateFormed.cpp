#include "pch.h"

#include "SnowballStateFormed.h"

#include "SnowballComponent.h"
#include "SnowballStateRolling.h"
#include "Core/Actor.h"
#include "Core/Collision/CollisionComponent.h"
#include "Core/Collision/CollisionProfile.h"

SnowballStateFormed::SnowballStateFormed(float initialValue)
{
    _accValue = initialValue + formedBonusValue;
}

void SnowballStateFormed::Enter(Ptr<class SnowballComponent> snowball) {}

void SnowballStateFormed::Exit(Ptr<class SnowballComponent> snowball) {}

void SnowballStateFormed::Tick(Ptr<class SnowballComponent> snowball, float deltaTime)
{
    Ptr<Actor> actor = snowball->GetOwner();

    _accValue -= decPerSecond * deltaTime;
}

bool SnowballStateFormed::TryKick(Ptr<class SnowballComponent> snowball, float direction)
{
    snowball->Transition(New<SnowballStateRolling>(direction));

    return true;
}

bool SnowballStateFormed::TryPush(Ptr<class SnowballComponent> snowball, float direction)
{
    // todo 이동
    return true;
}

bool SnowballStateFormed::CanCollideWith(
  Ptr<class SnowballComponent> snowball, Weak<class CollisionComponent> collider)
{
    Ptr<CollisionComponent> colliderLock = Lock(collider);
    ColliderType::Type      colliderType = colliderLock->GetProfile()->GetColliderType();

    switch (colliderType)
    {
    case ColliderType::Player:
    case ColliderType::PlayerProjectile:
    case ColliderType::EnemyProjectile:
    case ColliderType::Snowball: // rolling, formed
        return true;
    }

    return false;
}
