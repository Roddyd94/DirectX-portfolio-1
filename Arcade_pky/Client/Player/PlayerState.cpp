#include "pch.h"

#include "PlayerState.h"

#include "PlayerComponent.h"
#include "AI/AIComponent.h"
#include "Core/Actor.h"
#include "Core/Collision/CollisionComponent.h"
#include "Snowbros/SnowbrosPlayerBlackboard.h"

void PlayerState::Destroy() {}

void PlayerState::CollideWith(Ptr<class PlayerComponent> playerComponent,
  CollisionState::Type                                   collisionType,
  Weak<class CollisionComponent>                         collider)
{
    auto blackboard = playerComponent->GetStateMachine()->GetBlackboard<SnowbrosPlayerBlackboard>();

    auto thisActor         = playerComponent->GetOwner();
    auto thisCollider      = thisActor->FindSceneComponent<CollisionComponent>("Collider");
    auto otherCollider     = Lock(collider);
    auto otherColliderType = otherCollider->GetColliderType();
    auto otherActor        = otherCollider->GetOwner();

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

        if (otherState->GetName() == "Snowball")
        {
            switch (collisionType)
            {
            case CollisionState::Enter:
            {
                // TODO: 눈덩이 위에 착지한 경우?
            }
            break;
            case CollisionState::Stay:
            {
                // TODO: 눈덩이 위에 착지한 경우?
            }
            break;
            }
        }
        else if (otherState->GetName() == "Struggle" || otherState->GetName() == "Launched"
                 || otherState->GetName() == "Dead")
        {
            return;
        }
        else
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

uint8 PlayerState::GetType() const
{
    return _stateType;
}
