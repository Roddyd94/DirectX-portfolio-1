#include "pch.h"

#include "SnowballStateForming.h"

#include "SnowballBlackboard.h"
#include "SnowballComponent.h"
#include "SnowballStateFormed.h"
#include "AI/AIComponent.h"
#include "Core/Actor.h"
#include "Core/Animation/SpriteComponent.h"
#include "Core/Collision/CollisionComponent.h"
#include "Core/Collision/CollisionProfile.h"

SnowballStateForming::SnowballStateForming()
{
    _stateType = SnowballStateType::Forming;
}

void SnowballStateForming::Enter(Ptr<class SnowballComponent> snowball)
{
    auto enemy     = snowball->GetEnemyComponent()->GetOwner();
    auto enemyRoot = enemy->GetRoot();

    enemyRoot->SetEnable(true);
}

void SnowballStateForming::Exit(Ptr<class SnowballComponent> snowball) {}

void SnowballStateForming::Tick(Ptr<class SnowballComponent> snowball, float deltaTime)
{
    HandleGravity(snowball, deltaTime);

    auto       blackboard = snowball->GetBlackboard();
    Ptr<Actor> actor      = snowball->GetOwner();
    Ptr<Actor> enemy      = snowball->GetEnemyComponent()->GetOwner();
    actor->SetWorldPosition(enemy->GetWorldPosition());

    blackboard->accValue -= decPerSecond * deltaTime;

    if (blackboard->accValue <= phaseThreshold[0])
    {
        snowball->OnDestroy();
        actor->SetActive(false);
        return;
    }

    for (int32 i = snowballStateFormingPhaseCount - 1; i >= 0; i--)
    {
        if (blackboard->accValue > phaseThreshold[i])
        {
            auto sprite = actor->FindSceneComponent<SpriteComponent>("Root");
            sprite->SetFrameIndex(i);

            if (i == snowballStateFormingPhaseCount - 1)
                snowball->Transition(New<SnowballStateFormed>());

            return;
        }
    }
}

void SnowballStateForming::CollideWith(Ptr<class SnowballComponent> snowball,
  CollisionState::Type                                              collisionState,
  Weak<class CollisionComponent>                                    collider)
{
    auto blackboard = snowball->GetBlackboard();

    Ptr<CollisionComponent> colliderLock = Lock(collider);
    ColliderType::Type      colliderType = colliderLock->GetProfile()->GetColliderType();

    switch (colliderType)
    {
    case ColliderType::PlayerProjectile:
        if (CollisionState::Enter == collisionState)
            blackboard->accValue += formingIncValue;
        break;
    case ColliderType::EnemyProjectile:
        break;
    case ColliderType::Snowball: // Rolling인 경우 파괴
        break;
    }
}
