#pragma once
#include "Core/Actor.h"

class EnemyProjectile : public Actor
{
public:
    EnemyProjectile()           = default;
    ~EnemyProjectile() override = default;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    void SetDirection(float direction);
};
