#include "pch.h"

#include "PlayerComponent.h"

#include "PlayerStateMachine.h"

namespace snowbros
{
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
} // namespace snowbros
