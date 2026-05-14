#pragma once
#include "PlayerState.h"
#include "Core/ActorComponent.h"

class PlayerComponent : public ActorComponent
{
public:
    PlayerComponent()           = default;
    ~PlayerComponent() override = default;

private:
    Ptr<class PlayerStateMachine> _stateMachine = nullptr;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;
    void Tick(float deltaTime) override;

    void Transition(Ptr<class PlayerState> state);
    void HandleInput(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);

    uint8 GetStateType() const;
};
