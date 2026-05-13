#include "pch.h"

#include "TestLevel.h"

#include "Player.h"
#include "SnowBrosTileParser.h"
#include "Core/Camera.h"
#include "Tilemap/Tile.h"
#include "Tilemap/Tilemap.h"

bool TestLevel::Init(Ptr<World> world, const std::string& path)
{
    Level::Init(world, path);

    Vector3 position = Vector3{0.f, 0.f, 1.f};
    Vector3 scale    = Vector3{1.f, 1.f, 1.f};
    Vector3 rotation = Vector3{0.f, 0.f, 0.f};

    position.x = -7.5f;
    position.y = -6.5f;

    _tilemap = SpawnActor<Tilemap>(position, scale, rotation);
    _tilemap->SetTexture("Tile", L"snowbros_stages_flat.png");
    _tilemap->CreateTile(16, 14, {1.f, 1.f}, 0);

    float tileSize = 16.f;

    for (size_t i = 0; i < 12; i++)
    {
        for (size_t j = 0; j < 13; j++)
        {
            float startX = tileSize * j;
            float startY = tileSize * i;
            _tilemap->AddTileSprite(startX, startY, tileSize, tileSize);
        }
    }

    int32 tileCount = 12 * 13;

    TileMetadata tileMetadata;
    SnowBrosTileParser::ParseTileMetadata(tileMetadata);

    byte stageData[224] = {};
    SnowBrosTileParser::ParseStageData(L"snowbros_stage_2.bin", stageData, 224);
    for (size_t i = 0; i < 14; i++)
    {
        for (size_t j = 0; j < 16; j++)
        {
            Ptr<Tile> tile = _tilemap->GetTile((13 - i) * 16 + j);

            int32 spriteIndex = stageData[i * 16 + j];
            tile->SetSprite(spriteIndex);

            if (tileMetadata.tiles[TileType::Ceiling].contains(spriteIndex))
                tile->SetTileType(TileType::Ceiling);
            else if (tileMetadata.tiles[TileType::PlatformEnd].contains(spriteIndex))
                tile->SetTileType(TileType::PlatformEnd);
            else if (tileMetadata.tiles[TileType::Platform].contains(spriteIndex))
                tile->SetTileType(TileType::Platform);
            else if (tileMetadata.tiles[TileType::Wall].contains(spriteIndex))
                tile->SetTileType(TileType::Wall);
            else if (tileMetadata.tiles[TileType::Roof].contains(spriteIndex))
                tile->SetTileType(TileType::Roof);
        }
    }

    SnowBrosTileParser::ParseAnimationData("snowbros_player", L"snowbros_player.png",
      "snowbros_player", L"snowbros_player_animation.bin");

    position.x = 0.f;
    position.y = 0.f;
    scale *= 2.f;

    Ptr<Player> player = SpawnActor<Player>(position, scale, rotation);
    player->SetName("Player");

    return true;
}

Ptr<class Tilemap> TestLevel::GetTilemap() const
{
    return _tilemap;
}
