#pragma once
#include "Player/PlayerState.h"

class PlayerStateMidair : public PlayerState
{
public:
    PlayerStateMidair(bool jumped = true);
    ~PlayerStateMidair() override = default;

private:
    bool  _jumped  = true;
    float _accTime = 0.f;

private:
    inline static const float jumpTime   = 0.13333333333333333333f;
    inline static const float tumbleTime = 0.53333333333333333333f;

public:
    Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> playerComponent,
      Ptr<class InputAction>                                action,
      ButtonEventType::Type                                 buttonEvent) override;

    void Enter(Ptr<class PlayerComponent> playerComponent) override;
    void Exit(Ptr<class PlayerComponent> playerComponent) override;
    void Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime) override;
};
