#include "pch.h"

#include "SnowballState.h"

#include "SnowballBlackboard.h"
#include "SnowballComponent.h"
#include "Core/Actor.h"
#include "Platformer/PlatformerKinematicComponent.h"

void SnowballState::Destroy() {}

void SnowballState::CollideWith(Ptr<class SnowballComponent> snowball,
  CollisionState::Type                                       collisionState,
  Weak<class CollisionComponent>                             collider)
{
}

void SnowballState::HandleGravity(Ptr<class SnowballComponent> snowball, float deltaTime)
{
    auto blackboard = snowball->GetBlackboard();
    snowball->SynchronizePosition();

    auto actor     = snowball->GetOwner();
    auto kinematic = actor->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

    if (kinematic->IsColliderOnFloor())
    {
        kinematic->SetVelocity(Vector2::zero);
        return;
    }

    kinematic->AddGravity(deltaTime);
    if (kinematic->IsColliderMovingAgainstFloor(kinematic->GetVelocity() * deltaTime))
    {
        kinematic->AdjustPositionToFloor(kinematic->GetVelocity() * deltaTime);
        kinematic->SetVelocity(Vector2::zero);
    }
}

SnowballStateType SnowballState::GetType() const
{
    return _stateType;
}
