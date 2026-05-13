#include "pch.h"

#include "PlayerComponent.h"

#include "PlayerStateMachine.h"

bool PlayerComponent::Init(int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    ActorComponent::Init(componentID, name, owner);

    _stateMachine = New<PlayerStateMachine>();
    _stateMachine->Init(This<PlayerComponent>());

    return true;
}

void PlayerComponent::Destroy()
{
    ActorComponent::Destroy();
}

void PlayerComponent::Tick(float deltaTime)
{
    ActorComponent::Tick(deltaTime);
    _stateMachine->Tick(deltaTime);
}

void PlayerComponent::Transition(Ptr<class PlayerState> state)
{
    _stateMachine->Transition(state);
}

void PlayerComponent::HandleInput(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
{
    _stateMachine->HandleInput(action, buttonEvent);
}

PlayerStateType PlayerComponent::GetStateType() const
{
    return _stateMachine->GetStateType();
}
