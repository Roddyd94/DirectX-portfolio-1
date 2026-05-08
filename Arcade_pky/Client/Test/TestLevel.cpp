#include "pch.h"

#include "TestLevel.h"

#include "SnowBrosTileParser.h"
#include "TestActor.h"
#include "Core/Camera.h"
#include "Tilemap/Tile.h"
#include "Tilemap/Tilemap.h"

bool TestLevel::Init(Ptr<World> world, const std::string& path)
{
    Level::Init(world, path);

    Vector3 position = Vector3{0.f, 0.f, 1.f};
    Vector3 scale    = Vector3{1.f, 1.f, 1.f};
    Vector3 rotation = Vector3{0.f, 0.f, 0.f};

    Ptr<TestActor> testActor = SpawnActor<TestActor>(position, scale, rotation);

    position.x = -7.5f;
    position.y = -6.5f;

    Ptr<Tilemap> tilemap = SpawnActor<Tilemap>(position, scale, rotation);
    tilemap->SetTexture("Tile", L"snowbros_stages_flat.png");
    tilemap->CreateTile(16, 14, {1.f, 1.f}, 0);

    float tileSize = 16.f;

    for (size_t i = 0; i < 12; i++)
    {
        for (size_t j = 0; j < 13; j++)
        {
            float startX = tileSize * j;
            float startY = tileSize * i;
            tilemap->AddTileSprite(startX, startY, tileSize, tileSize);
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
            Ptr<Tile> tile = tilemap->GetTile((13 - i) * 16 + j);

            int32 spriteIndex = stageData[i * 16 + j];
            tile->SetSprite(spriteIndex);

            if (tileMetadata.tiles[TileType::Platform].contains(spriteIndex))
                tile->SetTileType(TileType::Platform);
            else if (tileMetadata.tiles[TileType::Wall].contains(spriteIndex))
                tile->SetTileType(TileType::Wall);
            else if (tileMetadata.tiles[TileType::Floor].contains(spriteIndex))
                tile->SetTileType(TileType::Floor);
        }
    }

    return true;
}
