#include "pch.h"

#include "SnowballStateFormed.h"

#include "SnowballBlackboard.h"
#include "SnowballComponent.h"
#include "SnowballStateForming.h"
#include "SnowballStateRolling.h"
#include "AI/AIComponent.h"
#include "Core/Actor.h"
#include "Core/Collision/CollisionComponent.h"
#include "Core/Collision/CollisionProfile.h"

SnowballStateFormed::SnowballStateFormed()
{
    _stateType = SnowballStateType::Formed;
}

void SnowballStateFormed::Enter(Ptr<class SnowballComponent> snowball)
{
    auto enemy     = snowball->GetEnemyComponent()->GetOwner();
    auto enemyRoot = enemy->GetRoot();

    enemyRoot->SetEnable(false);
}

void SnowballStateFormed::Exit(Ptr<class SnowballComponent> snowball) {}

void SnowballStateFormed::Tick(Ptr<class SnowballComponent> snowball, float deltaTime)
{
    auto blackboard = snowball->GetBlackboard();
    HandleGravity(snowball, deltaTime);

    Ptr<Actor> actor = snowball->GetOwner();

    blackboard->accValue -= decPerSecond * deltaTime;
    if (blackboard->accValue
        < SnowballStateForming::phaseThreshold[snowballStateFormingPhaseCount - 1])
    {
        snowball->Transition(New<SnowballStateForming>());
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

void SnowballStateFormed::CollideWith(Ptr<class SnowballComponent> snowball,
  CollisionState::Type                                             collisionState,
  Weak<class CollisionComponent>                                   collider)
{
    auto                    blackboard = snowball->GetBlackboard();
    Ptr<Actor>              thisActor  = snowball->GetOwner();
    Ptr<CollisionComponent> thisCollider
      = thisActor->FindSceneComponent<CollisionComponent>("Collider");
    Ptr<CollisionComponent> otherCollider     = Lock(collider);
    Ptr<Actor>              otherActor        = otherCollider->GetOwner();
    ColliderType::Type      otherColliderType = otherCollider->GetProfile()->GetColliderType();

    switch (otherColliderType)
    {
    case ColliderType::Player:

    {
    }
    break;
    case ColliderType::PlayerProjectile:
    {
        if (collisionState == CollisionState::Enter)
            blackboard->accValue += formedIncValue;
    }
    break;
    case ColliderType::EnemyProjectile:
    {
        blackboard->accValue = SnowballStateForming::phaseThreshold[0];
    }
    break;
    case ColliderType::Snowball: // rolling, formed
    {
        auto otherSnowball
          = otherCollider->GetOwner()->FindActorComponent<SnowballComponent>("Snowball");
        auto otherState = otherSnowball->GetCurrentStateType();

        switch (otherState)
        {
        case SnowballStateType::Formed:
            if (collisionState == CollisionState::Stay)
            {
                Vector2 thisPosition  = thisActor->GetWorldPosition().ToVector2();
                Vector2 otherPosition = otherActor->GetWorldPosition().ToVector2();

                if (thisPosition.x < otherPosition.x)
                    thisPosition.x -= moveDeltaX;
                else
                    thisPosition.x += moveDeltaX;

                thisActor->SetWorldPosition(thisPosition);
                snowball->SynchronizePosition();
            }
            break;
        case SnowballStateType::Rolling:
            break;
        }
    }
    break;
    }
}
