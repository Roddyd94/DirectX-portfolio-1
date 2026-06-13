#include "pch.h"

#include "PlayerStateSpawned.h"

#include "Core/TimeManager.h"

#include "IndexedSpriteInstanceComponent.h"
#include "PlayerStateClear.h"
#include "PlayerStateGround.h"
#include "SnowbrosPlayerBlackboard.h"
#include "Types.h"
#include "Core/Animation/SpriteInstanceComponent.h"
#include "Platformer/PlatformerKinematicPlayerComponent.h"
#include "Platformer/PlatformerMovementComponent.h"
#include "Player/Player.h"
#include "Player/PlayerController.h"

PlayerStateSpawned::PlayerStateSpawned()
{
    _stateType = SnowbrosPlayerStateType::Spawned;
}

Ptr<PlayerState> PlayerStateSpawned::HandleInput(Ptr<class PlayerComponent> player,
  Ptr<class InputAction>                                                    action,
  ButtonEventType::Type                                                     buttonEvent)
{
    return nullptr;
}

void PlayerStateSpawned::Enter(Ptr<class PlayerComponent> playerComponent)
{
    auto player = playerComponent->GetPlayer();
    auto sprite = player->FindSceneComponent<SpriteInstanceComponent>("Sprite");
    sprite->ChangeAnimation("player_stand");

    auto effect = player->FindSceneComponent<SpriteInstanceComponent>("Effect");
    effect->SetEnable(true);
    effect->ChangeAnimation("effect_eruption");

    auto controller = player->GetController();
    controller->SetActiveContext("None");

    auto kinematic = player->FindActorComponent<PlatformerKinematicPlayerComponent>("Kinematic");
    kinematic->SetVelocity(Vector2::zero);

    _timerID = TimeManager::Instance().SetTimer(0.f, true,
      [weakSprite = Weak(sprite), weakEffect = Weak(effect)]()
      {
          auto sprite = Lock(weakSprite);
          auto effect = Lock(weakEffect);

          if (nullptr == sprite || nullptr == effect)
              return;

          if (effect->GetFrameIndex() % 2 == 1)
              sprite->SetEnable(false);
          else
              sprite->SetEnable(true);
      });
}

void PlayerStateSpawned::Exit(Ptr<class PlayerComponent> playerComponent)
{
    SnowbrosPlayerState::Exit(playerComponent);
    TimeManager::Instance().RemoveTimer(_timerID);

    auto blackboard        = GetBlackboard(playerComponent);
    blackboard->invincible = true;

    TimeManager::Instance().RemoveTimer(blackboard->invincibleTimer);
    blackboard->invincibleTimer
      = TimeManager::Instance().SetTimer(blackboard->invincibleTime, false,
        [weakPlayerComponent = Weak(playerComponent)]()
        {
            auto playerComponent = Lock(weakPlayerComponent);
            auto blackboard      = GetBlackboard(playerComponent);
            auto player          = playerComponent->GetPlayer();
            auto sprite = player->FindSceneComponent<SpriteInstanceComponent>("Sprite");

            blackboard->invincible = false;
            sprite->SetEnable(true);
        });
}

void PlayerStateSpawned::Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime) {}
