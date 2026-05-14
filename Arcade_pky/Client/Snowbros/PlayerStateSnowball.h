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
    Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> player,
      Ptr<class InputAction>                                action,
      ButtonEventType::Type                                 buttonEvent) override;

    void Enter(Ptr<class PlayerComponent> player) override;
    void Exit(Ptr<class PlayerComponent> player) override;
    void Tick(Ptr<class PlayerComponent> player, float deltaTime) override;
};
