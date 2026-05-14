#include "pch.h"

#include "PlayerStateDead.h"

#include "Types.h"

PlayerStateDead::PlayerStateDead()
{
    _stateType = PlayerStateType::Dead;
}

Ptr<PlayerState> PlayerStateDead::HandleInput(
  Ptr<class PlayerComponent> player, Ptr<InputAction> action, ButtonEventType::Type buttonEvent)
{
    return nullptr;
}

void PlayerStateDead::Enter(Ptr<class PlayerComponent> player) {}

void PlayerStateDead::Exit(Ptr<class PlayerComponent> player) {}

void PlayerStateDead::Tick(Ptr<class PlayerComponent> player, float deltaTime) {}
