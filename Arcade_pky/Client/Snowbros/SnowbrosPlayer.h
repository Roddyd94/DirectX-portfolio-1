#pragma once
#include "Player/Player.h"

class SnowbrosPlayer : public Player
{
public:
    SnowbrosPlayer()           = default;
    ~SnowbrosPlayer() override = default;

private:
    Ptr<class AABBCollisionComponent> _collider;
    Ptr<class CollisionComponent> _headCollider;
    Ptr<class CollisionComponent> _handColliderLeft;
    Ptr<class CollisionComponent> _handColliderRight;
    Ptr<class CollisionComponent> _footCollider;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
};
