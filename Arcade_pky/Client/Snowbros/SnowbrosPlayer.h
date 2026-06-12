#pragma once
#include "Core/Input/Types.h"
#include "Player/Player.h"

class SnowbrosPlayer : public Player
{
public:
    SnowbrosPlayer()           = default;
    ~SnowbrosPlayer() override = default;

private:
    Ptr<class AABBCollisionComponent>  _collider;
    Ptr<class PointCollisionComponent> _headCollider;
    Ptr<class PointCollisionComponent> _handColliderLeft;
    Ptr<class PointCollisionComponent> _handColliderRight;
    Ptr<class PointCollisionComponent> _footCollider;

    int32 _playerNumber = 0;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void SetDirection(float direction);
    void ResetState();
    void StartStage();
    void EndStage();

    int32 GetPlayerNumber() const;
    void SetPlayerNumber(int32 number);

private:
    void OnShootButtonEvent(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);
};
