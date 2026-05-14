#pragma once
#include "Core/Actor.h"

class SnowProjectile : public Actor
{
public:
    SnowProjectile()           = default;
    ~SnowProjectile() override = default;

protected:
    Ptr<class SnowProjectileComponent> _projectileComponent;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;

    Ptr<class SnowProjectileComponent> GetProjectileComponent() const;

    bool IsPoweredUp() const;

    void SetPowerUp(bool powerUp);
    void SetRangeUp(bool rangeUp);
    void SetDirection(float direction);
};
