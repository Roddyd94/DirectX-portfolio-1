#include "pch.h"

#include "SnowballMorphableEnemyStateMachine.h"

#include "Core/Collision/CollisionManager.h"

#include "SnowbrosLevel.h"
#include "AI/AIComponent.h"
#include "Core/Actor.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Platformer/PlatformerKinematicComponent.h"

bool SnowballMorphableEnemyStateMachine::TryMoveX(float deltaX)
{
    auto actor     = GetOwner()->GetOwner();
    auto kinematic = actor->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

    if (kinematic->IsColliderMovingAgainstBoundaryX(deltaX))
        return false;

    if (kinematic->IsColliderMovingAgainstWallX(deltaX))
        return false;

    auto thisCollider = actor->FindSceneComponent<AABBCollisionComponent>("Collider");

    Rect    thisRect     = thisCollider->GetBox();
    Vector2 thisPosition = thisCollider->GetWorldPosition().ToVector2();

    auto                                      level = Cast<Level, SnowbrosLevel>(actor->GetLevel());
    auto                                      collisionManager = level->GetCollisionManager();
    std::vector<Weak<AABBCollisionComponent>> snowballs;
    SnowballMorphableEnemyStateMachine::FindSnowballs(collisionManager, snowballs);

    for (auto& snowball : snowballs)
    {
        auto otherCollider = Lock(snowball);

        if (thisCollider->GetColliderID() == otherCollider->GetColliderID())
            continue;

        Rect    otherRect     = otherCollider->GetBox();
        Vector2 otherPosition = otherCollider->GetWorldPosition().ToVector2();

        if (otherRect.top < thisRect.bottom)
            continue;

        if (thisRect.top < otherRect.bottom)
            continue;

        if (otherRect.right < thisRect.left)
            continue;

        if (thisRect.right < otherRect.left)
            continue;

        bool canMoveLeft  = thisPosition.x > otherPosition.x && deltaX < 0.f;
        bool canMoveRight = thisPosition.x < otherPosition.x && deltaX > 0.f;

        if (!canMoveLeft && !canMoveRight)
            continue;

        auto otherActor = otherCollider->GetOwner();
        auto otherAI    = otherActor->FindActorComponent<AIComponent>("AI");
        auto otherStateMachine
          = Cast<AIStateMachine, SnowballMorphableEnemyStateMachine>(otherAI->GetAIStateMachine());

        if (nullptr == otherStateMachine)
            continue;

        if (otherStateMachine->TryMoveX(deltaX))
        {
            kinematic->MoveX(deltaX);
            return true;
        }
        else
            return false;
    }

    kinematic->MoveX(deltaX);
    return true;
}

void SnowballMorphableEnemyStateMachine::FindSnowballs(Ptr<class CollisionManager> collisionManager,
  std::vector<Weak<class AABBCollisionComponent>>&                                 snowballs)
{
    std::vector<Ptr<CollisionComponent>> enemyColliders;
    collisionManager->FindColliders(ColliderType::Enemy, enemyColliders);

    for (auto& otherCollider : enemyColliders)
    {
        auto otherActor = otherCollider->GetOwner();
        auto otherAI    = otherActor->FindActorComponent<AIComponent>("AI");
        if (nullptr == otherAI)
            continue;

        auto otherStateMachine = otherAI->GetAIStateMachine();
        if (nullptr == Cast<AIStateMachine, SnowballMorphableEnemyStateMachine>(otherStateMachine))
            continue;

        auto otherState = otherStateMachine->GetCurrentState();
        if (nullptr == otherState)
            continue;

        if (otherState->GetName() != "Snowball")
            continue;

        auto aabb = Cast<CollisionComponent, AABBCollisionComponent>(otherCollider);
        snowballs.push_back(aabb);
    }
}
