#pragma once
#include "Player/Player.h"

class SnowbrosPlayer : public Player
{
public:
    SnowbrosPlayer()           = default;
    ~SnowbrosPlayer() override = default;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
};
