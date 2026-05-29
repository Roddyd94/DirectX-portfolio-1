#include "pch.h"

#include "AIComponent.h"

#include "AIController.h"

bool AIComponent::Init(int32 componentID, const std::string& name, Ptr<Actor> owner)
{
    return ActorComponent::Init(componentID, name, owner);
}

void AIComponent::Destroy()
{
    DESTROY(_stateMachine)
}

void AIComponent::Tick(float deltaTime)
{
    ActorComponent::Tick(deltaTime);

    if (!_stateMachine)
        return;

    _stateMachine->Tick(deltaTime);
}

Ptr<class Pawn> AIComponent::GetPawn() const
{
    return GetController()->GetPawn();
}

Ptr<AIController> AIComponent::GetController() const
{
    return Lock(_controller);
}

Ptr<AIStateMachine> AIComponent::GetAIStateMachine()
{
    return _stateMachine;
}

void AIComponent::SetController(Ptr<AIController> controller)
{
    _controller = controller;
}
