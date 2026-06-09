#pragma once
#include "Types.h"
#include "AI/AI.h"

class SnowbrosEnemy : public AI
{
public:
    SnowbrosEnemy()           = default;
    ~SnowbrosEnemy() override = default;

private:
    SnowbrosEnemyType _enemyType;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void SetEnemyType(SnowbrosEnemyType enemyType);
    void SetDirection(float direction);
};
