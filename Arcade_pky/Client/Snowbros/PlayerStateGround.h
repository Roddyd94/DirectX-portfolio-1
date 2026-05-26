#pragma once
#include "Player/PlayerState.h"

class PlayerStateGround : public PlayerState
{
public:
    PlayerStateGround();
    ~PlayerStateGround() override = default;

public:
    static const Ptr<class PlayerStateGround> instance;

private:
    inline static const float speedMultiplierSnowball = 0.5f;

public:
    Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> player,
      Ptr<class InputAction>                                action,
      ButtonEventType::Type                                 buttonEvent) override;

    void Enter(Ptr<class PlayerComponent> playerComponent) override;
    void Exit(Ptr<class PlayerComponent> playerComponent) override;
    void Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime) override;

    void CollideWith(Ptr<class PlayerComponent> playerComponent,
      CollisionState::Type                        collisionType,
      Weak<class CollisionComponent>              collider) override;
};
