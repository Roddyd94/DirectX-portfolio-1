#include "pch.h"

#include "PlayerStateGround.h"

#include "PlayerStateMidair.h"
#include "Types.h"
#include "Core/Actor.h"
#include "Core/Animation/SpriteComponent.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Platformer/PlatformerMovementComponent.h"
#include "Player/PlayerComponent.h"

const Ptr<PlayerStateGround> PlayerStateGround::instance = New<PlayerStateGround>();

PlayerStateGround::PlayerStateGround()
{
    _stateType = PlayerStateType::Ground;
}

Ptr<PlayerState> PlayerStateGround::HandleInput(Ptr<class PlayerComponent> playerComponent,
  Ptr<class InputAction>                                                   action,
  ButtonEventType::Type                                                    buttonEvent)
{
    Ptr<Actor>                       player = playerComponent->GetOwner();
    Ptr<PlatformerMovementComponent> movement
      = player->FindActorComponent<PlatformerMovementComponent>("PlatformerMovement");

    Ptr<SpriteComponent> sprite = player->FindSceneComponent<SpriteComponent>("Root");

    if (action->GetName() == "MoveLeft")
    {
        switch (buttonEvent)
        {
        case ButtonEventType::Hold:
            movement->MoveLeft();
            sprite->ChangeAnimation("player_walk");
            sprite->SetFlipX(false);
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
            movement->MoveRight();
            sprite->ChangeAnimation("player_walk");
            sprite->SetFlipX(true);
            break;
        case ButtonEventType::Up:
            movement->Stop();
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
            Ptr<PlatformerKinematicComponent> kinematic
              = player->FindActorComponent<PlatformerKinematicComponent>("PlatformerKinematic");

            movement->Jump();
            kinematic->ChangeStateTo(PlatformerKinematicState::OnAir);

            return New<PlayerStateMidair>();
        }
        break;
        }
    }

    return nullptr;
}

void PlayerStateGround::Enter(Ptr<class PlayerComponent> playerComponent)
{
    Ptr<Actor>                        player = playerComponent->GetOwner();
    Ptr<PlatformerKinematicComponent> kinematic
      = player->FindActorComponent<PlatformerKinematicComponent>("PlatformerKinematic");
    Ptr<SpriteComponent> sprite = player->FindSceneComponent<SpriteComponent>("Root");

    float deltaX = kinematic->GetVelocity().x;
    if (std::abs(deltaX) < FLT_EPSILON)
        sprite->ChangeAnimation("player_stand");
    else
        sprite->ChangeAnimation("player_walk");
}

void PlayerStateGround::Exit(Ptr<class PlayerComponent> playerComponent) {}

void PlayerStateGround::Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime) {}
