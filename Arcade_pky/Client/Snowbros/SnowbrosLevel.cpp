#include "pch.h"

#include "SnowbrosLevel.h"

#include "Core/ResourceManager.h"

#include "SnowbrosDataParser.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosPlayer.h"
#include "Core/Camera.h"
#include "Core/Texture.h"
#include "Snowbros/IndexedSpriteInstanceRenderer.h"
#include "Tilemap/Tile.h"
#include "Tilemap/Tilemap.h"

bool SnowbrosLevel::Init(Ptr<class World> world, const std::string& path)
{
    TilemapLevel::Init(world, path);

    Vector3 position = Vector3{0.f, 0.f, 1.f};
    Vector3 scale    = Vector3{1.f, 1.f, 1.f};
    Vector3 rotation = Vector3{0.f, 0.f, 0.f};

    position.x = -7.5f;
    position.y = -6.5f;

    _tilemap = SpawnActor<Tilemap>(position, scale, rotation);
    _tilemap->SetTexture("Tile", L"snowbros_tilemap.png");
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

    SnowbrosDataParser::ReadPalettes(L"snowbros_palettes.bin");

    int32 tileCount = 12 * 13;

    TileMetadata tileMetadata;
    SnowbrosDataParser::ParseTileMetadata(tileMetadata);

    byte stageData[224] = {};
    SnowbrosDataParser::ParseStageData(L"snowbros_stage_1.bin", stageData, 224);
    for (size_t i = 0; i < 14; i++)
    {
        for (size_t j = 0; j < 16; j++)
        {
            Ptr<Tile> tile = _tilemap->GetTile((13 - i) * 16 + j);

            int32 spriteIndex = stageData[i * 16 + j];
            tile->SetSprite(spriteIndex);

            if (tileMetadata.tiles[TileType::Block].contains(spriteIndex))
                tile->SetTileType(TileType::Block);
        }
    }

    SnowbrosDataParser::ParseAnimationData("snowbros_player", L"snowbros_player.bmp",
      "snowbros_player", L"snowbros_player_animation.bin");
    SnowbrosDataParser::ParseAnimationData(
      "snowbros_enemy", L"snowbros_enemies.bmp", "snowbros_enemy", L"snowbros_enemy_animation.bin");
    auto playerSpriteSheet
      = ANIMATION_MANAGER->FindSpriteSheet<IndexedTextureAnimation2DSpriteSheet>("snowbros_player");
    auto enemySpriteSheet
      = ANIMATION_MANAGER->FindSpriteSheet<IndexedTextureAnimation2DSpriteSheet>("snowbros_enemy");
    auto playerTexture = playerSpriteSheet->GetTexture();
    auto enemyTexture  = enemySpriteSheet->GetTexture();

    auto renderer
      = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>("Item", position, scale, rotation);
    renderer->SetTexture(playerTexture);
    renderer = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>(
      "EnemyBehind", position, scale, rotation);
    renderer->SetTexture(enemyTexture);
    renderer
      = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>("Snowball", position, scale, rotation);
    renderer->SetTexture(playerTexture);
    renderer
      = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>("Enemy", position, scale, rotation);
    renderer->SetTexture(enemyTexture);
    renderer = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>(
      "EnemyLaunched", position, scale, rotation);
    renderer->SetTexture(enemyTexture);
    renderer = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>(
      "EnemyProjectile", position, scale, rotation);
    renderer->SetTexture(enemyTexture);
    renderer = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>(
      "PlayerProjectile", position, scale, rotation);
    renderer->SetTexture(playerTexture);
    renderer
      = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>("Player", position, scale, rotation);
    renderer->SetTexture(playerTexture);

    position.x = 0.f;
    position.y = 0.f;
    scale *= 2.f;

    _player = SpawnActor<SnowbrosPlayer>(position, scale, rotation);
    _player->SetName("Player");

    position.x = 2.5f;
    position.y = 3.5f;
    {
        Ptr<SnowbrosEnemy> enemy = SpawnActor<SnowbrosEnemy>(position, scale, rotation);
        enemy->SetName("Enemy");
        enemy->SetEnemyType(SnowbrosEnemyType::Goblin);
        enemy->SetDirection(1.f);
    }

    position.x = -2.5f;
    {
        Ptr<SnowbrosEnemy> enemy = SpawnActor<SnowbrosEnemy>(position, scale, rotation);
        enemy->SetName("Enemy");
        enemy->SetEnemyType(SnowbrosEnemyType::Goblin);
        enemy->SetDirection(-1.f);
    }

    position.x = -1.5f;
    {
        Ptr<SnowbrosEnemy> enemy = SpawnActor<SnowbrosEnemy>(position, scale, rotation);
        enemy->SetName("Enemy");
        enemy->SetEnemyType(SnowbrosEnemyType::Goblin);
        enemy->SetDirection(-1.f);
    }

    position.x = 1.5f;
    {
        Ptr<SnowbrosEnemy> enemy = SpawnActor<SnowbrosEnemy>(position, scale, rotation);
        enemy->SetName("Enemy");
        enemy->SetEnemyType(SnowbrosEnemyType::Goblin);
        enemy->SetDirection(1.f);
    }

    return true;
}

void SnowbrosLevel::Destroy()
{
    TilemapLevel::Destroy();
}

void SnowbrosLevel::Tick(float deltaTime)
{
    TilemapLevel::Tick(deltaTime);

    std::vector<Ptr<Actor>> enemies;
    FindActors("Enemy", enemies);

    if (!enemies.size())
    {
        for (int i = 0; i < 11; ++i)
        {
            Ptr<SnowbrosEnemy> enemy = SpawnActor<SnowbrosEnemy>(
              {-2.f + 0.5f * i, 3.5f, 0.f}, 2 * Vector3::one, Vector3::zero);

            enemy->SetName("Enemy");
            enemy->SetEnemyType(SnowbrosEnemyType::Goblin);
            enemy->SetDirection(1.f);
        }
    }
}

Ptr<class Player> SnowbrosLevel::GetPlayer() const
{
    return _player;
}
