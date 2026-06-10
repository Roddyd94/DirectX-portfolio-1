#include "pch.h"

#include "SnowbrosPlayerState.h"

#include "Core/Collision/CollisionManager.h"
#include "Core/TimeManager.h"

#include "IndexedSpriteInstanceComponent.h"
#include "Item.h"
#include "PlayerStateDead.h"
#include "PlayerStateSnowball.h"
#include "ShootComponent.h"
#include "SnowballMorphableEnemyStateMachine.h"
#include "SnowbrosEnemyState.h"
#include "SnowbrosLevel.h"
#include "SnowbrosPlayerBlackboard.h"
#include "Types.h"
#include "AI/AIComponent.h"
#include "Core/Actor.h"
#include "Core/Animation/SpriteInstanceComponent.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Core/Collision/PointCollisionComponent.h"
#include "Platformer/PlatformerKinematicPlayerComponent.h"
#include "Platformer/PlatformerMovementComponent.h"
#include "Player/Player.h"
#include "Player/PlayerComponent.h"

void SnowbrosPlayerState::Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime)
{
    auto player     = playerComponent->GetPlayer();
    auto blackboard = GetBlackboard(playerComponent);

    auto sprite = player->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");

    if (blackboard->invincible)
    {
        int32 unitFrameCounts  = static_cast<int32>(TimeManager::targetFPS * 0.2f);
        int32 frameNumber = TimeManager::Instance().GetFrameCount() % unitFrameCounts;
        if (frameNumber > unitFrameCounts / 2)
            sprite->SetEnable(false);
        else
            sprite->SetEnable(true);
    }
}

void SnowbrosPlayerState::CollideWith(Ptr<class PlayerComponent> playerComponent,
  CollisionState::Type                                           collisionType,
  Weak<class CollisionComponent>                                 collider)
{
    auto blackboard = GetBlackboard(playerComponent);

    auto thisActor         = playerComponent->GetOwner();
    auto thisCollider      = thisActor->FindSceneComponent<CollisionComponent>("Collider");
    auto otherCollider     = Lock(collider);
    auto otherColliderType = otherCollider->GetColliderType();

    auto kinematic = thisActor->FindActorComponent<PlatformerKinematicPlayerComponent>("Kinematic");

    switch (otherColliderType)
    {
    case ColliderType::Enemy:
    {
        auto otherPawn  = Cast<Actor, Pawn>(otherCollider->GetOwner());
        auto otherActor = otherPawn->GetController();

        auto otherAI = otherActor->FindActorComponent<AIComponent>("AI");
        if (nullptr == otherAI)
            return;

        auto otherStateMachine = otherAI->GetAIStateMachine();
        auto otherState        = otherStateMachine->GetCurrentState<SnowbrosEnemyState>();

        if (nullptr == otherState)
            return;

        auto otherStateType = otherState->GetStateType();

        switch (otherStateType)
        {
        case SnowbrosEnemyState::Stand:
        case SnowbrosEnemyState::Patrol:
        case SnowbrosEnemyState::Walk:
        case SnowbrosEnemyState::Turn:
        case SnowbrosEnemyState::Jump:
        case SnowbrosEnemyState::Fall:
        case SnowbrosEnemyState::Fire:
        case SnowbrosEnemyState::Crouch:
        case SnowbrosEnemyState::Dizzy:
            switch (collisionType)
            {
            case CollisionState::Stay:
                if (this->GetType() != SnowbrosPlayerStateType::Ground
                    && this->GetType() != SnowbrosPlayerStateType::Midair)
                    break;

                kinematic->SetVelocity(Vector2::zero);
                kinematic->ChangeStateTo(PlatformerKinematicState::None);
                playerComponent->Transition(New<PlayerStateDead>());
                auto sprite = thisActor->FindSceneComponent<SpriteInstanceComponent>("Sprite");
                TimeManager::Instance().SetTimer(1.0f, false,
                  [weakSprite = Weak(sprite)]()
                  {
                      auto sprite = Lock(weakSprite);
                      sprite->ChangeAnimation("player_dead");
                  });
            }
            break;
        case SnowbrosEnemyState::SnowballRolling:
        {
            switch (_stateType)
            {
            case SnowbrosPlayerStateType::Ground:
            case SnowbrosPlayerStateType::Midair:
            {
                if (blackboard->jumpedFromSnowball)
                    break;

                blackboard->bindTargetSnowball = collider;
                playerComponent->Transition(PlayerStateSnowball::instance);
            }
            break;
            }
        }
        break;
        case SnowbrosEnemyState::SnowballCrashing:
            break;
        case SnowbrosEnemyState::Launched:
            break;
        case SnowbrosEnemyState::Dead:
            break;
        default:
            break;
        }
    }
    break;
    case ColliderType::EnemyProjectile:
    {
        if (this->GetType() != SnowbrosPlayerStateType::Ground
            && this->GetType() != SnowbrosPlayerStateType::Midair)
            break;

        kinematic->SetVelocity(Vector2::zero);
        kinematic->ChangeStateTo(PlatformerKinematicState::None);
        playerComponent->Transition(New<PlayerStateDead>());
        auto sprite = thisActor->FindSceneComponent<SpriteInstanceComponent>("Sprite");
        TimeManager::Instance().SetTimer(1.0f, false,
          [weakSprite = Weak(sprite)]()
          {
              auto sprite = Lock(weakSprite);
              sprite->ChangeAnimation("player_melt");
          });
    }
    break;
    case ColliderType::Item:
    {
        auto item = Cast<Actor, Item>(otherCollider->GetOwner());
        if (nullptr == item)
            return;

        Item::Type itemType = item->GetItemType();

        switch (itemType)
        {
        case Item::Speed:
        {
            if (blackboard->speedUpgraded)
            {
                // TODO 1000 점
                return;
            }

            blackboard->speedMultiplier = 2.f;
            blackboard->speedUpgraded   = true;
        }
        break;
        case Item::Power:
        {
            if (blackboard->powerUpgraded)
            {
                // TODO 1000 점
                return;
            }

            auto shoot = thisActor->FindActorComponent<ShootComponent>("Shoot");
            shoot->PowerUp();

            blackboard->powerUpgraded = true;
        }
        break;
        case Item::Range:
        {
            if (blackboard->rangeUpgraded)
            {
                // TODO 1000 점
                return;
            }

            auto shoot = thisActor->FindActorComponent<ShootComponent>("Shoot");
            shoot->RangeUp();

            blackboard->rangeUpgraded = true;
        }
        break;
        case Item::Invincibility:
            break;
        case Item::Sushi:
            break;
        case Item::Envelope:
            break;
        case Item::Special:
            break;
        case Item::End:
            break;
        default:
            break;
        }
    }
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

        auto otherPawn  = Cast<Actor, Pawn>(otherCollider->GetOwner());
        auto otherActor = otherPawn->GetController();

        auto otherAI = otherActor->FindActorComponent<AIComponent>("AI");
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

Ptr<class SnowbrosPlayerBlackboard> SnowbrosPlayerState::GetBlackboard(
  Ptr<class PlayerComponent> playerComponent)
{
    auto stateMachine = playerComponent->GetStateMachine();
    if (nullptr == stateMachine)
        return nullptr;

    return stateMachine->GetBlackboard<SnowbrosPlayerBlackboard>();
}
