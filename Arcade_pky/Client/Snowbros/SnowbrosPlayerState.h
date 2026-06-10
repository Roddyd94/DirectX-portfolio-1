#pragma once
#include "Player/PlayerState.h"

class SnowbrosPlayerState : public PlayerState
{
public:
    SnowbrosPlayerState()           = default;
    ~SnowbrosPlayerState() override = default;

protected:
    inline static const float speedMultiplierSnowball = 0.5f;

public:
    void Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime) override;

    void CollideWith(Ptr<class PlayerComponent> playerComponent,
      CollisionState::Type                      collisionType,
      Weak<class CollisionComponent>            collider) override;
    Ptr<class SnowballMorphableEnemyStateMachine> FindSnowballToPush(
      Ptr<class PlayerComponent> playerComponent, float deltaX);
    Ptr<class AABBCollisionComponent> FindSnowballToStand(
      Ptr<class PlayerComponent> playerComponent);

protected:
    static Ptr<class SnowbrosPlayerBlackboard> GetBlackboard(
      Ptr<class PlayerComponent> playerComponent);
};
