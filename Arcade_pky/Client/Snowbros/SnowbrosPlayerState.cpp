#include "pch.h"

#include "SnowbrosPlayerState.h"

#include "Core/Collision/CollisionManager.h"
#include "Core/TimeManager.h"

#include "SnowballMorphableEnemyStateMachine.h"
#include "SnowbrosLevel.h"
#include "AI/AIComponent.h"
#include "Core/Actor.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Core/Collision/PointCollisionComponent.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Platformer/PlatformerKinematicPlayerComponent.h"
#include "Player/Player.h"
#include "Player/PlayerComponent.h"
#include "Snowbros/SnowbrosPlayerBlackboard.h"

void SnowbrosPlayerState::CollideWith(Ptr<class PlayerComponent> playerComponent,
  CollisionState::Type                                           collisionType,
  Weak<class CollisionComponent>                                 collider)
{
    auto blackboard = playerComponent->GetStateMachine()->GetBlackboard<SnowbrosPlayerBlackboard>();

    auto thisActor         = playerComponent->GetOwner();
    auto thisCollider      = thisActor->FindSceneComponent<CollisionComponent>("Collider");
    auto otherCollider     = Lock(collider);
    auto otherColliderType = otherCollider->GetColliderType();
    auto otherActor        = otherCollider->GetOwner();

    auto kinematic = thisActor->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

    switch (otherColliderType)
    {
    case ColliderType::Enemy:
    {
        auto otherAI = otherActor->FindActorComponent<AIComponent>("AI");
        if (nullptr == otherAI)
            return;

        auto otherStateMachine = otherAI->GetAIStateMachine();
        auto otherState        = otherStateMachine->GetCurrentState();

        if (nullptr == otherState)
            return;

        if (otherState->GetName() != "Snowball" && otherState->GetName() != "Struggle"
            && otherState->GetName() != "Launched" && otherState->GetName() != "Dead")
        {
            switch (collisionType)
            {
            case CollisionState::Enter:
                // todo: kill player
                break;
            }
        }
    }
    break;
    case ColliderType::EnemyProjectile:
        break;
    case ColliderType::Item:
        break;
    }
}

Ptr<class SnowballMorphableEnemyStateMachine> SnowbrosPlayerState::FindSnowballToPush(
  Ptr<class PlayerComponent> playerComponent, float deltaX)
{
    auto actor = playerComponent->GetOwner();

    auto thisCollider = actor->FindSceneComponent<AABBCollisionComponent>("Collider");

    Ptr<PointCollisionComponent> handCollider = nullptr;
    if (deltaX < 0.f)
        handCollider = actor->FindSceneComponent<PointCollisionComponent>("HandColliderLeft");
    else if (deltaX > 0.f)
        handCollider = actor->FindSceneComponent<PointCollisionComponent>("HandColliderRight");

    if (nullptr == handCollider)
        return nullptr;

    auto level            = Cast<Level, SnowbrosLevel>(actor->GetLevel());
    auto collisionManager = level->GetCollisionManager();

    std::vector<Weak<AABBCollisionComponent>> snowballs;
    SnowballMorphableEnemyStateMachine::FindSnowballs(collisionManager, snowballs);

    for (auto& snowball : snowballs)
    {
        auto otherCollider = Lock(snowball);

        if (!CollisionSystem::AABBToPoint(snowball, handCollider))
            continue;

        auto otherActor = otherCollider->GetOwner();
        auto otherAI    = otherActor->FindActorComponent<AIComponent>("AI");
        auto otherStateMachine
          = Cast<AIStateMachine, SnowballMorphableEnemyStateMachine>(otherAI->GetAIStateMachine());

        if (nullptr == otherStateMachine)
            continue;

        return otherStateMachine;
    }

    return nullptr;
}

Ptr<class AABBCollisionComponent> SnowbrosPlayerState::FindSnowballToStand(
  Ptr<class PlayerComponent> playerComponent)
{
    auto actor = playerComponent->GetOwner();

    auto thisCollider = actor->FindSceneComponent<AABBCollisionComponent>("Collider");

    Ptr<PointCollisionComponent> footCollider
      = actor->FindSceneComponent<PointCollisionComponent>("FootCollider");

    auto level            = Cast<Level, SnowbrosLevel>(actor->GetLevel());
    auto collisionManager = level->GetCollisionManager();

    std::vector<Weak<AABBCollisionComponent>> snowballs;
    SnowballMorphableEnemyStateMachine::FindSnowballs(collisionManager, snowballs);

    for (auto& snowball : snowballs)
    {
        auto otherCollider = Lock(snowball);

        if (thisCollider->GetColliderID() == otherCollider->GetColliderID())
            continue;

        Rect    otherRect    = otherCollider->GetBox();
        Vector2 footPosition = footCollider->GetWorldPosition().ToVector2();

        if (!CollisionSystem::AABBToPoint(otherRect, footPosition))
            continue;

        return otherCollider;
    }

    return nullptr;
}
