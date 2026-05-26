#include "pch.h"

#include "PlayerStateMachine.h"

#include "PlayerState.h"

void PlayerStateMachine::Init(Ptr<class PlayerComponent> player, Ptr<class PlayerState> state)
{
    _owner        = player;
    _currentState = state;
}

void PlayerStateMachine::Destroy()
{
    DESTROY(_blackboard);
}

void PlayerStateMachine::Tick(float deltaTime)
{
    _currentState->Tick(GetOwner(), deltaTime);
}

void PlayerStateMachine::Transition(Ptr<class PlayerState> state)
{
    Ptr<PlayerComponent> owner = GetOwner();
    if (_currentState)
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

uint8 PlayerStateMachine::GetCurrentStateType() const
{
    return _currentState->GetType();
}

void PlayerStateMachine::SetBlackboard(Ptr<class PlayerBlackboard> blackboard)
{
    _blackboard = blackboard;
}

void PlayerStateMachine::CollideWith(
  CollisionState::Type collisionType, Weak<class CollisionComponent> collider)
{
    _currentState->CollideWith(GetOwner(), collisionType, collider);
}
