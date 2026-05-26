#include "pch.h"

#include "PlayerStateGround.h"

#include "PlayerStateMidair.h"
#include "SnowbrosPlayerBlackboard.h"
#include "Types.h"
#include "Core/Animation/SpriteComponent.h"
#include "Core/Collision/CollisionComponent.h"
#include "Platformer/PlatformerKinematicPlayerComponent.h"
#include "Platformer/PlatformerMovementComponent.h"
#include "Player/Player.h"
#include "Player/PlayerComponent.h"
#include "Player/PlayerController.h"

const Ptr<PlayerStateGround> PlayerStateGround::instance = New<PlayerStateGround>();

PlayerStateGround::PlayerStateGround()
{
    _stateType = SnowbrosPlayerStateType::Ground;
}

Ptr<PlayerState> PlayerStateGround::HandleInput(Ptr<class PlayerComponent> playerComponent,
  Ptr<class InputAction>                                                   action,
  ButtonEventType::Type                                                    buttonEvent)
{
    Ptr<Actor> player = playerComponent->GetOwner();

    auto blackboard = playerComponent->GetStateMachine()->GetBlackboard<SnowbrosPlayerBlackboard>();
    auto movement   = player->FindActorComponent<PlatformerMovementComponent>("PlatformerMovement");

    Ptr<SpriteComponent> sprite = player->FindSceneComponent<SpriteComponent>("Root");

    if (action->GetName() == "MoveLeft")
    {
        switch (buttonEvent)
        {
        case ButtonEventType::Hold:
        {
            float speed = blackboard->speedX;

            movement->MoveLeft(speed);
            if (sprite->GetCurrentClipName() != "player_shoot")
                sprite->ChangeAnimation("player_walk");

            sprite->SetFlipX(false);
        }
        break;
        case ButtonEventType::Up:
            movement->Stop();
            sprite->ChangeAnimation("player_stand");
            break;
        }
    }
    else if (action->GetName() == "MoveRight")
    {
        switch (buttonEvent)
        {
        case ButtonEventType::Hold:
        {
            float speed = blackboard->speedX;

            movement->MoveRight(speed);
            if (sprite->GetCurrentClipName() != "player_shoot")
                sprite->ChangeAnimation("player_walk");

            sprite->SetFlipX(true);
        }
        break;
        case ButtonEventType::Up:
            movement->Stop();
            if (sprite->GetCurrentClipName() != "player_shoot")
                sprite->ChangeAnimation("player_stand");
            break;
        }
    }
    else if (action->GetName() == "Jump")
    {
        switch (buttonEvent)
        {
        case ButtonEventType::Down:
        {
            Ptr<PlatformerKinematicPlayerComponent> kinematic
              = player->FindActorComponent<PlatformerKinematicPlayerComponent>("Kinematic");

            movement->Jump();
            kinematic->ChangeStateTo(PlatformerKinematicState::OnAir);

            return New<PlayerStateMidair>(true);
        }
        break;
        }
    }

    return nullptr;
}

void PlayerStateGround::Enter(Ptr<class PlayerComponent> playerComponent)
{
    Ptr<Player> player = playerComponent->GetPlayer();

    auto controller = player->GetController();
    controller->SetActiveContext("Ground");

    Ptr<PlatformerKinematicComponent> kinematic
      = player->FindActorComponent<PlatformerKinematicComponent>("Kinematic");
    float deltaX = kinematic->GetVelocity().x;

    Ptr<SpriteComponent> sprite = player->FindSceneComponent<SpriteComponent>("Root");
    if (std::abs(deltaX) < FLT_EPSILON)
        sprite->ChangeAnimation("player_stand");
    else
        sprite->ChangeAnimation("player_walk");
}

void PlayerStateGround::Exit(Ptr<class PlayerComponent> playerComponent) {}

void PlayerStateGround::Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime)
{
    auto blackboard = playerComponent->GetStateMachine()->GetBlackboard<SnowbrosPlayerBlackboard>();
}

void PlayerStateGround::CollideWith(Ptr<class PlayerComponent> playerComponent,
  CollisionState::Type                                         collisionType,
  Weak<class CollisionComponent>                               collider)
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
    }
    break;
    case ColliderType::EnemyProjectile:
        break;
    case ColliderType::Item:
        break;
    }
}
