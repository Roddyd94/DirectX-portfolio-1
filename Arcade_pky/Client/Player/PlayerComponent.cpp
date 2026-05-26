#include "pch.h"

#include "PlayerComponent.h"

#include "Player.h"
#include "PlayerStateMachine.h"

bool PlayerComponent::Init(int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    ActorComponent::Init(componentID, name, owner);

    _stateMachine = New<PlayerStateMachine>();
    _stateMachine->Init(This<PlayerComponent>(), nullptr);

    return true;
}

void PlayerComponent::Destroy()
{
    ActorComponent::Destroy();
    DESTROY(_stateMachine);
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

Ptr<class Player> PlayerComponent::GetPlayer() const
{
    return Cast<Actor, Player>(GetOwner());
}

Ptr<class PlayerStateMachine> PlayerComponent::GetStateMachine() const
{
    return _stateMachine;
}

uint8 PlayerComponent::GetStateType() const
{
    return _stateMachine->GetCurrentStateType();
}

void PlayerComponent::SetBlackboard(Ptr<class PlayerBlackboard> blackboard)
{
    _stateMachine->SetBlackboard(blackboard);
}

void PlayerComponent::CollideWith(
  CollisionState::Type collisionType, Weak<class CollisionComponent> collider)
{
    _stateMachine->CollideWith(collisionType, collider);
}
