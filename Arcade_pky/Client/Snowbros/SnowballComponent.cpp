#include "pch.h"

#include "SnowballComponent.h"

#include "Snowball.h"
#include "SnowballStateForming.h"
#include "SnowballStateMachine.h"

bool SnowballComponent::Init(int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    ActorComponent::Init(componentID, name, owner);

    _stateMachine = New<SnowballStateMachine>();
    _stateMachine->Init(This<SnowballComponent>(), New<SnowballStateForming>());

    return true;
}

void SnowballComponent::Destroy()
{
    ActorComponent::Destroy();
    DESTROY(_stateMachine);
}

void SnowballComponent::Tick(float deltaTime)
{
    _stateMachine->Tick(deltaTime);
}

void SnowballComponent::Transition(Ptr<class SnowballState> state)
{
    _stateMachine->Transition(state);
}

bool SnowballComponent::TryPush(float direction)
{
    return _stateMachine->TryPush(direction);
}

bool SnowballComponent::TryKick(float direction)
{
    return _stateMachine->TryKick(direction);
}

void SnowballComponent::CollideWith(Weak<class CollisionComponent> collider)
{
    return _stateMachine->CollideWith(collider);
}

void SnowballComponent::OnDestroy()
{
    if (_onDestroyCallback)
        _onDestroyCallback();
}
