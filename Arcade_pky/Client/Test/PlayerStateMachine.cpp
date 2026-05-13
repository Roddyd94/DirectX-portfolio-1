#include "pch.h"

#include "PlayerStateMachine.h"

#include "PlayerState.h"
#include "PlayerStateGround.h"

void PlayerStateMachine::Init(Ptr<class PlayerComponent> player)
{
    _owner        = player;
    _currentState = PlayerState::ground;
}

void PlayerStateMachine::Destroy() {}

void PlayerStateMachine::Tick(float deltaTime)
{
    _currentState->Tick(GetOwner(), deltaTime);
}

void PlayerStateMachine::Transition(Ptr<class PlayerState> state)
{
    Ptr<PlayerComponent> owner = GetOwner();
    _currentState->Exit(owner);
    _currentState = state;
    _currentState->Enter(owner);
}

void PlayerStateMachine::HandleInput(
  Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
{
    Ptr<PlayerState> newState = _currentState->HandleInput(GetOwner(), action, buttonEvent);

    if (nullptr == newState)
        return;

    Transition(newState);
}

Ptr<class PlayerComponent> PlayerStateMachine::GetOwner() const
{
    return Lock(_owner);
}

PlayerStateType PlayerStateMachine::GetStateType() const
{
    return _currentState->GetType();
}
