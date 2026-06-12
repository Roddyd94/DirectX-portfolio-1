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
    TileMetadata              _tileMetadata;
    std::vector<StageData>    _stageData;
    Ptr<class SnowbrosPlayer> _player[2]         = {};
    Ptr<class SnowbrosUI>     _ui                = nullptr;
    int32                     _score[2]          = {};
    int32                     _stageNumber       = 0;
    bool                      _startingNextStage = false;

public:
    bool Init(Ptr<class World> world, const std::string& path) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    Ptr<class Player> FindNearestPlayerFrom(Vector2 position) const;
    Ptr<class Player> FindNearestPlayerFrom(Vector3 position) const;
    Ptr<class Player> GetPlayer(int32 number) const;

    void SetPlayer(int32 number, Ptr<class SnowbrosPlayer> player);

    Ptr<class Item> SpawnItem(Vector3 position, Item::Type type);

    void AddScore(int32 playerNumber, int32 score);
    void ResetScore(int32 playerNumber);

    void StartStage(int32 stageNumber);
    void RemoveItems();
    void RemoveEnemies();
    void RemoveScorePopups();
};
