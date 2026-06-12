#include "pch.h"

#include "PlayerStateMidair.h"

#include "Core/TimeManager.h"

#include "SnowballMorphableEnemyStateMachine.h"
#include "SnowbrosPlayer.h"
#include "SnowbrosPlayerBlackboard.h"
#include "Types.h"
#include "Core/Animation/SpriteInstanceComponent.h"
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

    auto sprite = player->FindSceneComponent<SpriteInstanceComponent>("Sprite");

    if (sprite->GetFlipX() != _initialFlipX)
        _flippedX = true;

    if (action->GetName() == "MoveLeft")
    {
        switch (buttonEvent)
        {
        case ButtonEventType::Hold:
        {
            float speedX = _flippedX ? (blackboard->speedX * 0.5f) : blackboard->speedX;
            float deltaX = -speedX * deltaTime * blackboard->speedMultiplier;

            auto snowball = FindSnowballToPush(playerComponent, deltaX);
            if (nullptr != snowball)
            {
                if (snowball->TryMoveX(deltaX))
                    movement->MoveLeft(
                      speedX * blackboard->speedMultiplier * blackboard->speedMultiplierSnowball);
                else
                    movement->Stop();
            }
            else
            {
                movement->MoveLeft(speedX * blackboard->speedMultiplier);
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
            float deltaX = speedX * deltaTime * blackboard->speedMultiplier;

            auto snowball = FindSnowballToPush(playerComponent, deltaX);
            if (nullptr != snowball)
            {
                if (snowball->TryMoveX(deltaX))
                    movement->MoveRight(
                      speedX * blackboard->speedMultiplier * blackboard->speedMultiplierSnowball);
                else
                    movement->Stop();
            }
            else
            {
                movement->MoveRight(speedX * blackboard->speedMultiplier);
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
    auto player = Cast<Player, SnowbrosPlayer>(playerComponent->GetPlayer());

    auto controller = player->GetController();

    if (0 == player->GetPlayerNumber())
        controller->SetActiveContext("Midair1");
    if (1 == player->GetPlayerNumber())
        controller->SetActiveContext("Midair2");

    auto sprite = player->FindSceneComponent<SpriteInstanceComponent>("Sprite");

    if (_jumped)
        sprite->ChangeAnimation("player_jump");
    else if (sprite->GetCurrentClipName() != "player_kick")
        sprite->ChangeAnimation("player_midair");

    _initialFlipX = sprite->GetFlipX();
}

void PlayerStateMidair::Exit(Ptr<class PlayerComponent> playerComponent)
{
    SnowbrosPlayerState::Exit(playerComponent);
}

void PlayerStateMidair::Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime)
{
    SnowbrosPlayerState::Tick(playerComponent, deltaTime);

    Ptr<Actor>                       player = playerComponent->GetOwner();
    Ptr<PlatformerMovementComponent> movement
      = player->FindActorComponent<PlatformerMovementComponent>("Movement");

    movement->AccelerateGravity(deltaTime);
}
