#pragma once

namespace SnowbrosPlayerStateType
{
    enum Type : uint8
    {
        Ground,
        Midair,
        Snowball,
        Dead,
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
