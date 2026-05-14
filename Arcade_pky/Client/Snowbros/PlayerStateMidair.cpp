#include "pch.h"

#include "PlayerStateMidair.h"

#include "Types.h"
#include "Core/Animation/SpriteComponent.h"
#include "Platformer/PlatformerMovementComponent.h"
#include "Player/Player.h"
#include "Player/PlayerComponent.h"
#include "Player/PlayerController.h"

PlayerStateMidair::PlayerStateMidair(bool jumped) : _jumped(jumped)
{
    _stateType = SnowbrosPlayerStateType::Midair;
}

Ptr<PlayerState> PlayerStateMidair::HandleInput(Ptr<class PlayerComponent> playerComponent,
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
            sprite->SetFlipX(false);
            break;
        case ButtonEventType::Up:
            movement->Stop();
            break;
        }
    }
    else if (action->GetName() == "MoveRight")
    {
        switch (buttonEvent)
        {
        case ButtonEventType::Hold:
            movement->MoveRight();
            sprite->SetFlipX(true);
            break;
        case ButtonEventType::Up:
            movement->Stop();
            break;
        }
    }

    return nullptr;
}

void PlayerStateMidair::Enter(Ptr<class PlayerComponent> playerComponent)
{
    Ptr<Player>          player = playerComponent->GetPlayer();

    auto controller = player->GetController();
    controller->SetActiveContext("Midair");

    Ptr<SpriteComponent> sprite = player->FindSceneComponent<SpriteComponent>("Root");
    if (_jumped)
        sprite->ChangeAnimation("player_jump");
    else
        sprite->ChangeAnimation("player_midair");
}

void PlayerStateMidair::Exit(Ptr<class PlayerComponent> playerComponent) {}

void PlayerStateMidair::Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime)
{
    Ptr<Actor>                       player = playerComponent->GetOwner();
    Ptr<PlatformerMovementComponent> movement
      = player->FindActorComponent<PlatformerMovementComponent>("PlatformerMovement");

    movement->AccelerateGravity(deltaTime);
}
