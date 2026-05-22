#include "pch.h"

#include "SnowballStateMachine.h"

#include "SnowballState.h"

void SnowballStateMachine::Init(
  Ptr<class SnowballComponent> snowball, Ptr<class SnowballState> state)
{
    _owner        = snowball;
    _currentState = state;
}

void SnowballStateMachine::Destroy() {}

void SnowballStateMachine::Tick(float deltaTime)
{
    _currentState->Tick(GetOwner(), deltaTime);
}

void SnowballStateMachine::Transition(Ptr<class SnowballState> state)
{
    Ptr<SnowballComponent> owner = GetOwner();
    if (_currentState)
        _currentState->Exit(owner);
    _currentState = state;
    _currentState->Enter(owner);
}

bool SnowballStateMachine::TryKick(float direction)
{
    return _currentState->TryKick(GetOwner(), direction);
}

bool SnowballStateMachine::TryPush(float direction)
{
    return _currentState->TryPush(GetOwner(), direction);
}

void SnowballStateMachine::CollideWith(
  CollisionState::Type collisionState, Weak<class CollisionComponent> collider)
{
    _currentState->CollideWith(GetOwner(), collisionState, collider);
}

Ptr<class SnowballComponent> SnowballStateMachine::GetOwner() const
{
    return Lock(_owner);
}

SnowballStateType SnowballStateMachine::GetCurrentStateType() const
{
    return _currentState->GetType();
}
