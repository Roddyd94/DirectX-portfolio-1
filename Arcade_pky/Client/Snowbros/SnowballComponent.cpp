#include "pch.h"

#include "SnowballComponent.h"

#include "Snowball.h"
#include "SnowballBlackboard.h"
#include "SnowballStateForming.h"
#include "SnowballStateMachine.h"
#include "AI/AIComponent.h"

bool SnowballComponent::Init(int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    ActorComponent::Init(componentID, name, owner);

    _blackboard   = New<SnowballBlackboard>();
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

SnowballStateType SnowballComponent::GetCurrentStateType() const
{
    return _stateMachine->GetCurrentStateType();
}

Ptr<class SnowballBlackboard> SnowballComponent::GetBlackboard() const
{
    return _blackboard;
}

Ptr<class AIComponent> SnowballComponent::GetEnemyComponent() const
{
    return Lock(_enemyComponent);
}

void SnowballComponent::SetEnemyComponent(Ptr<class AIComponent> enemy)
{
    _enemyComponent = enemy;
}

bool SnowballComponent::TryPush(float direction)
{
    return _stateMachine->TryPush(direction);
}

bool SnowballComponent::TryKick(float direction)
{
    return _stateMachine->TryKick(direction);
}

void SnowballComponent::CollideWith(
  CollisionState::Type collisionState, Weak<class CollisionComponent> collider)
{
    return _stateMachine->CollideWith(collisionState, collider);
}

void SnowballComponent::SynchronizePosition()
{
    Ptr<Actor> actor = GetOwner();
    Ptr<Actor> enemy = GetEnemyComponent()->GetOwner();

    enemy->SetWorldPosition(actor->GetWorldPosition());
}

void SnowballComponent::OnDestroy()
{
    if (_onDestroyCallback)
        _onDestroyCallback();
}
