#include "pch.h"

#include "PlayerStateClear.h"

#include "Core/TimeManager.h"

#include "Types.h"
#include "Core/Animation/SpriteInstanceComponent.h"
#include "Platformer/PlatformerKinematicPlayerComponent.h"
#include "Platformer/PlatformerMovementComponent.h"
#include "Player/Player.h"
#include "Player/PlayerController.h"

const Ptr<PlayerStateClear> PlayerStateClear::instance = New<PlayerStateClear>();

PlayerStateClear::PlayerStateClear()
{
    _stateType = SnowbrosPlayerStateType::Clear;
}

Ptr<PlayerState> PlayerStateClear::HandleInput(Ptr<class PlayerComponent> player,
  Ptr<class InputAction>                                                  action,
  ButtonEventType::Type                                                   buttonEvent)
{
    return nullptr;
}

void PlayerStateClear::Enter(Ptr<class PlayerComponent> playerComponent)
{
    Ptr<Player> player = playerComponent->GetPlayer();

    auto controller = player->GetController();
    controller->SetActiveContext("None");

    Ptr<PlatformerKinematicPlayerComponent> kinematic
      = player->FindActorComponent<PlatformerKinematicPlayerComponent>("Kinematic");

    auto sprite = player->FindSceneComponent<SpriteInstanceComponent>("Sprite");

    kinematic->ChangeStateTo(PlatformerKinematicState::None);
    kinematic->SetVelocity({0.f, 10.f});

    sprite->ChangeAnimation("player_fly");
}

void PlayerStateClear::Exit(Ptr<class PlayerComponent> playerComponent)
{
    SnowbrosPlayerState::Exit(playerComponent);
}

void PlayerStateClear::Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime) {}
