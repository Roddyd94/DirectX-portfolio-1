#include "pch.h"

#include "PlayerStateMidair.h"

#include "Core/TimeManager.h"

#include "SnowballMorphableEnemyStateMachine.h"
#include "SnowbrosPlayerBlackboard.h"
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
    Ptr<Actor> player = playerComponent->GetOwner();

    float deltaTime = TimeManager::Instance().GetDeltaTime();

    auto blackboard = GetBlackboard(playerComponent);

    Ptr<PlatformerMovementComponent> movement
      = player->FindActorComponent<PlatformerMovementComponent>("Movement");

    Ptr<SpriteComponent> sprite = player->FindSceneComponent<SpriteComponent>("Root");

    if (sprite->GetFlipX() != _initialFlipX)
        _flippedX = true;

    if (action->GetName() == "MoveLeft")
    {
        switch (buttonEvent)
        {
        case ButtonEventType::Hold:
        {
            float speedX = _flippedX ? (blackboard->speedX * 0.5f) : blackboard->speedX;

            auto snowball = FindSnowballToPush(playerComponent, -speedX * deltaTime);
            if (nullptr != snowball)
            {
                if (snowball->TryMoveX(-speedX * deltaTime))
                    movement->MoveLeft(speedX * blackboard->speedMultiplierSnowball);
                else
                    movement->Stop();
            }
            else
            {
                movement->MoveLeft(speedX);
            }

            sprite->SetFlipX(false);
        }
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
        {
            float speedX = _flippedX ? (blackboard->speedX * 0.5f) : blackboard->speedX;

            auto snowball = FindSnowballToPush(playerComponent, speedX * deltaTime);
            if (nullptr != snowball)
            {
                if (snowball->TryMoveX(speedX * deltaTime))
                    movement->MoveRight(speedX * blackboard->speedMultiplierSnowball);
                else
                    movement->Stop();
            }
            else
            {
                movement->MoveRight(speedX);
            }

            sprite->SetFlipX(true);
        }
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
    Ptr<Player> player = playerComponent->GetPlayer();

    auto controller = player->GetController();
    controller->SetActiveContext("Midair");

    Ptr<SpriteComponent> sprite = player->FindSceneComponent<SpriteComponent>("Root");

    if (_jumped)
        sprite->ChangeAnimation("player_jump");
    else if (sprite->GetCurrentClipName() != "player_kick")
        sprite->ChangeAnimation("player_midair");
    _initialFlipX = sprite->GetFlipX();
}

void PlayerStateMidair::Exit(Ptr<class PlayerComponent> playerComponent) {}

void PlayerStateMidair::Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime)
{
    Ptr<Actor>                       player = playerComponent->GetOwner();
    Ptr<PlatformerMovementComponent> movement
      = player->FindActorComponent<PlatformerMovementComponent>("Movement");

    movement->AccelerateGravity(deltaTime);
}
