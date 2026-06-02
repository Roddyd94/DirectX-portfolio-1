#pragma once
#include "Tilemap/TilemapLevel.h"

class SnowbrosLevel : public TilemapLevel
{
public:
    SnowbrosLevel()           = default;
    ~SnowbrosLevel() override = default;
    DELETE_SPECIAL_FUNC(SnowbrosLevel)

private:
    Ptr<class Player> _player;

public:
    bool Init(Ptr<class World> world, const std::string& path) override;
    void Destroy() override;
    
    void Tick(float deltaTime) override;

    Ptr<class Player> GetPlayer() const;
};
