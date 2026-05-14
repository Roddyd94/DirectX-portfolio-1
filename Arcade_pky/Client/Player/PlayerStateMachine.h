#pragma once
#include "PlayerState.h"
#include "Core/Input/Types.h"
#include "Core/Object.h"

class PlayerStateMachine : public Object
{
public:
    PlayerStateMachine()           = default;
    ~PlayerStateMachine() override = default;

private:
    Weak<class PlayerComponent> _owner;
    Ptr<class PlayerState>      _currentState = nullptr;

public:
    void Init(Ptr<class PlayerComponent> player, Ptr<class PlayerState> state);
    void Destroy() override;
    void Tick(float deltaTime);

    void Transition(Ptr<class PlayerState> state);
    void HandleInput(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);

    Ptr<class PlayerComponent> GetOwner() const;
    uint8                      GetStateType() const;
};
