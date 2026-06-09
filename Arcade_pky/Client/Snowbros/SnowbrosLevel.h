#pragma once
#include "Types.h"
#include "Tilemap/TilemapLevel.h"

class SnowbrosLevel : public TilemapLevel
{
public:
    SnowbrosLevel()           = default;
    ~SnowbrosLevel() override = default;
    DELETE_SPECIAL_FUNC(SnowbrosLevel)

private:
    std::vector<StageData> _stageData;
    Ptr<class Player>      _player;
    TileMetadata           _tileMetadata;
    int32                  _stageNumber = 0;

public:
    bool Init(Ptr<class World> world, const std::string& path) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    Ptr<class Player> GetPlayer() const;

    void StartStage(int32 stageNumber);
};
