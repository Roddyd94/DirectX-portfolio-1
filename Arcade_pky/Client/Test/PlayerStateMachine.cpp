#include "pch.h"

#include "PlayerStateMachine.h"

#include "PlayerState.h"

namespace snowbros
{
    void PlayerStateMachine::Init(Ptr<class PlayerComponent> player)
    {
        _owner        = player;
        _currentState = PlayerState::ground;
    }

    void PlayerStateMachine::Destroy() {}

    void PlayerStateMachine::Transition(Ptr<class PlayerState> state)
    {
        Ptr<PlayerComponent> owner = GetOwner();
        _currentState->Exit(owner);
        _currentState = state;
        _currentState->Enter(owner);
    }

    void PlayerStateMachine::Tick(float deltaTime)
    {
        _currentState->Tick(GetOwner(), deltaTime);
    }

    void PlayerStateMachine::HandleInput(
      Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
    {
        _currentState->HandleInput(GetOwner(), action, buttonEvent);
    }
    Ptr<class PlayerComponent> PlayerStateMachine::GetOwner() const
    {
        return Lock(_owner);
    }
} // namespace snowbros
