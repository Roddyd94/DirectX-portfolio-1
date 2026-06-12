#pragma once
#include "Tilemap/Tile.h"

namespace SnowbrosPlayerStateType
{
    enum Type : uint8
    {
        Ground,
        Midair,
        Snowball,
        Dead,
        Clear,
        Spawned,
        End
    };
}

namespace SnowbrosColliderType
{
    enum Type : uint8
    {
        Player,
        Enemy,
        Boss,
        PlayerProjectile,
        EnemyProjectile,
        Item,
        End,
    };
}

enum class SnowbrosEnemyType
{
    Goblin,
    Monkey,
    Spitter,
    Pumpkin,
    Ghost,
    Spawn,
    Boss,
    End,
};

struct TileMetadata
{
    std::vector<byte> paletteNumbers;

    std::map<TileType::Type, std::set<uint32>> tiles;

    uint16 tileSize      = 1;
    uint16 tileTypeCount = 0;
};

struct EnemyPatrolData
{
    std::vector<Vector2> patrolPoints;
    int32                loopCount = 1;
};

struct EnemyData
{
    EnemyPatrolData   patrolData;
    SnowbrosEnemyType type;
    Vector2           position;
    float             direction = 0.f;
};

struct StageData
{
    std::wstring           filename;
    std::vector<EnemyData> enemies;
    Vector2                playerPosition[2]  = {};
    float                  playerDirection[2] = {};
    int32                  number             = -1;
    int32                  paletteNumber      = 0;
};
