#include "pch.h"

#include "PlayerStateGround.h"

#include "Core/Collision/CollisionManager.h"
#include "Core/TimeManager.h"

#include "PlayerStateMidair.h"
#include "SnowballMorphableEnemyStateMachine.h"
#include "SnowbrosLevel.h"
#include "SnowbrosPlayerBlackboard.h"
#include "Types.h"
#include "AI/AIComponent.h"
#include "Core/Animation/SpriteInstanceComponent.h"
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

    float deltaTime = TimeManager::Instance().GetDeltaTime();

    auto blackboard = GetBlackboard(playerComponent);
    auto movement   = player->FindActorComponent<PlatformerMovementComponent>("Movement");

    auto sprite = player->FindSceneComponent<SpriteInstanceComponent>("Root");

    if (action->GetName() == "MoveLeft")
    {
        switch (buttonEvent)
        {
        case ButtonEventType::Hold:
        {
            float speedX = blackboard->speedX;
            float speedSnowballX
              = speedX * blackboard->speedMultiplier * blackboard->speedMultiplierSnowball;

            auto snowball = FindSnowballToPush(playerComponent, -speedSnowballX * deltaTime);
            if (nullptr != snowball)
            {
                if (snowball->TryMoveX(-speedSnowballX * deltaTime))
                    movement->MoveLeft(
                      speedX * blackboard->speedMultiplier * blackboard->speedMultiplierSnowball);
                else
                    movement->Stop();

                if (sprite->GetCurrentClipName() != "player_shoot"
                    && sprite->GetCurrentClipName() != "player_kick")
                {
                    if (blackboard->speedUpgraded)
                        sprite->ChangeAnimation("player_push_run");
                    else
                        sprite->ChangeAnimation("player_push");
                }
            }
            else
            {
                movement->MoveLeft(speedX * blackboard->speedMultiplier);
                if (sprite->GetCurrentClipName() != "player_shoot"
                    && sprite->GetCurrentClipName() != "player_kick")
                {
                    if (blackboard->speedUpgraded)
                        sprite->ChangeAnimation("player_run");
                    else
                        sprite->ChangeAnimation("player_walk");
                }
            }

            sprite->SetFlipX(false);
        }
        break;
        case ButtonEventType::Up:
        {
            movement->Stop();
            if (sprite->GetCurrentClipName() != "player_shoot")
            {
                if (blackboard->speedUpgraded)
                    sprite->ChangeAnimation("player_run");
                else
                    sprite->ChangeAnimation("player_stand");
            }
        }
        break;
        }
    }
    else if (action->GetName() == "MoveRight")
    {
        switch (buttonEvent)
        {
        case ButtonEventType::Hold:
        {
            float speedX = blackboard->speedX;
            float speedSnowballX
              = speedX * blackboard->speedMultiplier * blackboard->speedMultiplierSnowball;

            auto snowball = FindSnowballToPush(playerComponent, speedSnowballX * deltaTime);
            if (nullptr != snowball)
            {
                if (snowball->TryMoveX(speedSnowballX * deltaTime))
                    movement->MoveRight(
                      speedX * blackboard->speedMultiplier * blackboard->speedMultiplierSnowball);
                else
                    movement->Stop();

                if (sprite->GetCurrentClipName() != "player_shoot")
                {
                    if (blackboard->speedUpgraded)
                        sprite->ChangeAnimation("player_push_run");
                    else
                        sprite->ChangeAnimation("player_push");
                }
            }
            else
            {
                movement->MoveRight(speedX * blackboard->speedMultiplier);
                if (sprite->GetCurrentClipName() != "player_shoot"
                    && sprite->GetCurrentClipName() != "player_kick")
                {
                    if (blackboard->speedUpgraded)
                        sprite->ChangeAnimation("player_run");
                    else
                        sprite->ChangeAnimation("player_walk");
                }
            }

            sprite->SetFlipX(true);
        }
        break;
        case ButtonEventType::Up:
        {
            movement->Stop();
            if (sprite->GetCurrentClipName() != "player_shoot")
            {
                if (blackboard->speedUpgraded)
                    sprite->ChangeAnimation("player_run");
                else
                    sprite->ChangeAnimation("player_stand");
            }
        }
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

    auto blackboard = GetBlackboard(playerComponent);
    auto controller = player->GetController();

    blackboard->jumpedFromSnowball = false;
    controller->SetActiveContext("Ground");

    Ptr<PlatformerKinematicComponent> kinematic
      = player->FindActorComponent<PlatformerKinematicComponent>("Kinematic");
    float deltaX = kinematic->GetVelocity().x;

    auto sprite = player->FindSceneComponent<SpriteInstanceComponent>("Root");

    if (blackboard->speedUpgraded)
    {
        sprite->ChangeAnimation("player_run");
        return;
    }

    if (std::abs(deltaX) < FLT_EPSILON)
        sprite->ChangeAnimation("player_stand");
    else
        sprite->ChangeAnimation("player_walk");
}

void PlayerStateGround::Exit(Ptr<class PlayerComponent> playerComponent) {}

void PlayerStateGround::Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime)
{
    // auto blackboard =
    // playerComponent->GetStateMachine()->GetBlackboard<SnowbrosPlayerBlackboard>();
}
