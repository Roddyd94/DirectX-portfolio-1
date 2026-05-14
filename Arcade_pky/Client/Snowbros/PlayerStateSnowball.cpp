#include "pch.h"

#include "PlayerStateSnowball.h"

const Ptr<PlayerStateSnowball> PlayerStateSnowball::instance = New<PlayerStateSnowball>();

PlayerStateSnowball::PlayerStateSnowball()
{
    _stateType = PlayerStateType::Snowball;
}

Ptr<PlayerState> PlayerStateSnowball::HandleInput(Ptr<class PlayerComponent> player,
  Ptr<class InputAction>                                                     action,
  ButtonEventType::Type                                                      buttonEvent)
{
    // if (action)

    return nullptr;
}

void PlayerStateSnowball::Enter(Ptr<class PlayerComponent> player) {}

void PlayerStateSnowball::Exit(Ptr<class PlayerComponent> player) {}

void PlayerStateSnowball::Tick(Ptr<class PlayerComponent> player, float deltaTime) {}
