#pragma once
#include "SnowbrosPlayerState.h"

class PlayerStateGround : public SnowbrosPlayerState
{
public:
    PlayerStateGround();
    ~PlayerStateGround() override = default;

public:
    static const Ptr<class PlayerStateGround> instance;

public:
    Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> player,
      Ptr<class InputAction>                                action,
      ButtonEventType::Type                                 buttonEvent) override;

    void Enter(Ptr<class PlayerComponent> playerComponent) override;
    void Exit(Ptr<class PlayerComponent> playerComponent) override;
    void Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime) override;
};
