#pragma once
#include "Core/Input/Types.h"
#include "Core/Object.h"

namespace snowbros
{
    class PlayerStateMachine : public Object
    {
    public:
        PlayerStateMachine()           = default;
        ~PlayerStateMachine() override = default;

    private:
        Weak<class PlayerComponent> _owner;
        Ptr<class PlayerState>      _currentState = nullptr;

    public:
        void Init(Ptr<class PlayerComponent> player);
        void Destroy() override;

        void Transition(Ptr<class PlayerState> state);
        void Tick(float deltaTime);

        void HandleInput(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);

        Ptr<class PlayerComponent> GetOwner() const;
    };
} // namespace snowbros
