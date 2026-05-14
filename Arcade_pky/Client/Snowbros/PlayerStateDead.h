#pragma once
#include "Player/PlayerState.h"

class PlayerStateDead : public PlayerState
{
public:
    PlayerStateDead();
    ~PlayerStateDead() override = default;

public:
    Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> player,
      Ptr<InputAction>                                      action,
      ButtonEventType::Type                                 buttonEvent) override;

    void Enter(Ptr<class PlayerComponent> player) override;
    void Exit(Ptr<class PlayerComponent> player) override;
    void Tick(Ptr<class PlayerComponent> player, float deltaTime) override;
};
