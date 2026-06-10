#pragma once
#include "SnowbrosPlayerState.h"

class PlayerStateSpawned : public SnowbrosPlayerState
{
public:
    PlayerStateSpawned();
    ~PlayerStateSpawned() override = default;

public:
    float _accTime = 0.f;
    int32 _timerID = -1;

public:
    inline static const float threshold = 3.f;

public:
    Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> player,
      Ptr<class InputAction>                                action,
      ButtonEventType::Type                                 buttonEvent) override;

    void Enter(Ptr<class PlayerComponent> playerComponent) override;
    void Exit(Ptr<class PlayerComponent> playerComponent) override;
    void Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime) override;
};
