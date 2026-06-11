#pragma once
#include "Item.h"
#include "Types.h"
#include "Tilemap/TilemapLevel.h"

class SnowbrosLevel : public TilemapLevel
{
public:
    SnowbrosLevel()           = default;
    ~SnowbrosLevel() override = default;
    DELETE_SPECIAL_FUNC(SnowbrosLevel)

private:
    std::vector<StageData>    _stageData;
    Ptr<class SnowbrosPlayer> _player;
    TileMetadata              _tileMetadata;
    int32                     _stageNumber       = 0;
    bool                      _startingNextStage = false;

public:
    bool Init(Ptr<class World> world, const std::string& path) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    Ptr<class Player> GetPlayer() const;

    void SetPlayer(Ptr<class SnowbrosPlayer> player);

    Ptr<class Item> SpawnItem(Vector3 position, Item::Type type);

    void StartStage(int32 stageNumber);
    void RemoveItems();
    void RemoveEnemies();
};
