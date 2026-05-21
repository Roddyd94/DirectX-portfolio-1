#include "pch.h"

#include "SnowballStateFormed.h"

#include "SnowballComponent.h"
#include "SnowballStateForming.h"
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
    if (_accValue < 0.f)
    {
        snowball->Transition(New<SnowballStateForming>(
          SnowballStateForming::phaseThreshold[snowballStateFormingPhaseCount - 1] + _accValue));
    }
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

void SnowballStateFormed::CollideWith(
  Ptr<class SnowballComponent> snowball, Weak<class CollisionComponent> collider)
{
    Ptr<CollisionComponent> colliderLock = Lock(collider);
    ColliderType::Type      colliderType = colliderLock->GetProfile()->GetColliderType();

    switch (colliderType)
    {
    case ColliderType::Player:
        break;
    case ColliderType::PlayerProjectile:
        _accValue += formedIncValue;
        break;
    case ColliderType::EnemyProjectile:
        break;
    case ColliderType::Snowball: // rolling, formed
        break;
    }
}
