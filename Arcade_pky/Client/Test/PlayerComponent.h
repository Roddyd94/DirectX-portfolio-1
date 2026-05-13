#pragma once
#include "Core/Actor.h"
#include "Core/ActorComponent.h"

namespace snowbros
{
    class PlayerComponent : public ActorComponent
    {
    public:
        PlayerComponent()           = default;
        ~PlayerComponent() override = default;

    private:
        Ptr<class PlayerStateMachine> _stateMachine = nullptr;

    public:
        bool Init(int32 componentID, const std::string& name, Ptr<Actor> owner) override;
        void Destroy() override;

        void Tick(float deltaTime) override;
    };
} // namespace snowbros
