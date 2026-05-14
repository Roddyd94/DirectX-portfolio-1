#pragma once
#include "Player/PlayerState.h"

class PlayerStateSnowball : public PlayerState
{
public:
    PlayerStateSnowball();
    ~PlayerStateSnowball() override = default;

public:
    static const Ptr<class PlayerStateSnowball> instance;

public:
    Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> playerComponent,
      Ptr<class InputAction>                                action,
      ButtonEventType::Type                                 buttonEvent) override;

    void Enter(Ptr<class PlayerComponent> playerComponent) override;
    void Exit(Ptr<class PlayerComponent> playerComponent) override;
    void Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime) override;
};
