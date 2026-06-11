#pragma once
#include "Types.h"
#include "AI/AI.h"

class SnowbrosEnemy : public AI
{
public:
    SnowbrosEnemy()           = default;
    ~SnowbrosEnemy() override = default;

protected:
    SnowbrosEnemyType _enemyType;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;

    SnowbrosEnemyType GetEnemyType() const;

    void SetEnemyType(SnowbrosEnemyType enemyType);
    void SetDirection(float direction);

protected:
    void CreateSnowball();
};
