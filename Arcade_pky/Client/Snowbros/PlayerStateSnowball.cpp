#include "pch.h"

#include "PlayerStateSnowball.h"

#include "Types.h"
#include "Player/Player.h"
#include "Player/PlayerComponent.h"
#include "Player/PlayerController.h"

const Ptr<PlayerStateSnowball> PlayerStateSnowball::instance = New<PlayerStateSnowball>();

PlayerStateSnowball::PlayerStateSnowball()
{
    _stateType = SnowbrosPlayerStateType::Snowball;
}

Ptr<PlayerState> PlayerStateSnowball::HandleInput(Ptr<class PlayerComponent> playerComponent,
  Ptr<class InputAction>                                                     action,
  ButtonEventType::Type                                                      buttonEvent)
{
    // if (action)

    return nullptr;
}

void PlayerStateSnowball::Enter(Ptr<class PlayerComponent> playerComponent)
{
    Ptr<Player> player = playerComponent->GetPlayer();

    auto controller = player->GetController();
    controller->SetActiveContext("Snowball");
}

void PlayerStateSnowball::Exit(Ptr<class PlayerComponent> playerComponent) {}

void PlayerStateSnowball::Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime) {}
