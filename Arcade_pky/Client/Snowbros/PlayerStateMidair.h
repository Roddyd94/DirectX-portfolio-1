#pragma once
#include "Player/PlayerState.h"

class PlayerStateMidair : public PlayerState
{
public:
    PlayerStateMidair(bool jumped = true);
    ~PlayerStateMidair() override = default;

private:
    bool _jumped       = true;
    bool _initialFlipX = false;
    bool _flippedX     = false;

public:
    Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> playerComponent,
      Ptr<class InputAction>                                action,
      ButtonEventType::Type                                 buttonEvent) override;

    void Enter(Ptr<class PlayerComponent> playerComponent) override;
    void Exit(Ptr<class PlayerComponent> playerComponent) override;
    void Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime) override;
};
