#pragma once
#include "Player/PlayerState.h"

class PlayerStateDead : public PlayerState
{
public:
    PlayerStateDead();
    ~PlayerStateDead() override = default;

public:
    Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> playerComponent,
      Ptr<InputAction>                                      action,
      ButtonEventType::Type                                 buttonEvent) override;

    void Enter(Ptr<class PlayerComponent> playerComponent) override;
    void Exit(Ptr<class PlayerComponent> playerComponent) override;
    void Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime) override;
};
