#include "pch.h"

#include "SnowbrosLevel.h"

#include "Core/ResourceManager.h"
#include "Core/TimeManager.h"

#include "SnowbrosCheat.h"
#include "SnowbrosDataParser.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosPlayer.h"
#include "Core/Camera.h"
#include "Core/InstanceRendererComponent.h"
#include "Core/Mesh.h"
#include "Core/Texture.h"
#include "Snowbros/IndexedSpriteInstanceRenderer.h"
#include "Tilemap/IndexedTilemap.h"
#include "Tilemap/Tile.h"

bool SnowbrosLevel::Init(Ptr<class World> world, const std::string& path)
{
    TilemapLevel::Init(world, path);

    Vector3 position = Vector3{0.f, 0.f, 1.f};
    Vector3 scale    = Vector3{1.f, 1.f, 1.f};
    Vector3 rotation = Vector3{0.f, 0.f, 0.f};

    SpawnActor<SnowbrosCheat>(position, scale, rotation);

    position.x = -7.5f;
    position.y = -6.5f;

    auto tilemap = SpawnActor<IndexedTilemap>(position, scale, rotation);
    SnowbrosDataParser::ParseIndexedTexture("Tile", L"snowbros_tilemap.bmp");
    tilemap->SetTexture("Tile");
    tilemap->CreateTile(16, 14, {1.f, 1.f}, 0);
    tilemap->AddTileSprites({16.f, 16.f});
    _tilemap = tilemap;

    SnowbrosDataParser::ReadPalettes(L"snowbros_palettes.bin");

    int32 tileCount = 12 * 13;

    SnowbrosDataParser::ParseTileMetadata(_tileMetadata);
    SnowbrosDataParser::ParseStageData(L"snowbros_stages.bin", _stageData);

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

    auto colliderRenderer
      = SpawnInstanceRenderer<InstanceRenderer>("SquareCollider", position, scale, rotation);
    colliderRenderer->CreateRendererComponent<InstanceRendererComponent>();
    colliderRenderer->SetMesh(MESH_LINE_RECT);
    colliderRenderer->SetShader("TileOutlineInstanceShader");
    colliderRenderer->SetBuffer("RectMesh");
    colliderRenderer->SetRenderLayer("Collider");

    colliderRenderer
      = SpawnInstanceRenderer<InstanceRenderer>("SphereCollider", position, scale, rotation);
    colliderRenderer->CreateRendererComponent<InstanceRendererComponent>();
    colliderRenderer->SetMesh(MESH_LINE_SPHERE);
    colliderRenderer->SetShader("TileOutlineInstanceShader");
    colliderRenderer->SetBuffer("SphereMesh");
    colliderRenderer->SetRenderLayer("Collider");

    auto spriteInstanceRenderer
      = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>("Item", position, scale, rotation);
    spriteInstanceRenderer->SetTexture(playerTexture);
    spriteInstanceRenderer = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>(
      "EnemyBehind", position, scale, rotation);
    spriteInstanceRenderer->SetTexture(enemyTexture);
    spriteInstanceRenderer
      = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>("Snowball", position, scale, rotation);
    spriteInstanceRenderer->SetTexture(playerTexture);
    spriteInstanceRenderer
      = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>("Enemy", position, scale, rotation);
    spriteInstanceRenderer->SetTexture(enemyTexture);
    spriteInstanceRenderer = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>(
      "EnemyLaunched", position, scale, rotation);
    spriteInstanceRenderer->SetTexture(enemyTexture);
    spriteInstanceRenderer = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>(
      "EnemyProjectile", position, scale, rotation);
    spriteInstanceRenderer->SetTexture(enemyTexture);
    spriteInstanceRenderer = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>(
      "PlayerProjectile", position, scale, rotation);
    spriteInstanceRenderer->SetTexture(playerTexture);
    spriteInstanceRenderer
      = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>("Player", position, scale, rotation);
    spriteInstanceRenderer->SetTexture(playerTexture);
    spriteInstanceRenderer = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>(
      "PlayerEffect", position, scale, rotation);
    spriteInstanceRenderer->SetTexture(playerTexture);

    position.x = 0.f;
    position.y = 0.f;
    scale *= 2.f;

    StartStage(0);

    return true;
}

void SnowbrosLevel::Destroy()
{
    TilemapLevel::Destroy();
}

void SnowbrosLevel::Tick(float deltaTime)
{
    TilemapLevel::Tick(deltaTime);

    if (nullptr == _player)
    {
        auto& stageData = _stageData[_stageNumber];

        Vector3 playerPosition = Vector3::one;

        playerPosition.x = stageData.playerPosition.x;
        playerPosition.y = stageData.playerPosition.y;

        auto player = SpawnActor<SnowbrosPlayer>(playerPosition, 2 * Vector3::one, Vector3::zero);
        player->SetDirection(stageData.playerDirection);

        _player = player;
    }

    std::vector<Ptr<Actor>> enemies;
    FindActors("Enemy", enemies);
    std::vector<Ptr<Actor>> items;
    FindActors("Item", items);

    if (!enemies.size() && !items.size())
    {
        // auto enemy = SpawnActor<SnowbrosEnemy>(position, scale, rotation);
        // enemy->SetEnemyType(SnowbrosEnemyType::Spawn);
        // enemy->SetDirection(enemyData.direction);

        if (!_startingNextStage)
            StartStage(_stageNumber + 1);
    }
}

Ptr<class Player> SnowbrosLevel::GetPlayer() const
{
    return _player;
}

void SnowbrosLevel::StartStage(int32 stageNumber)
{
    if (_startingNextStage)
        return;

    if (stageNumber < 0 || stageNumber >= _stageData.size())
        return;

    _stageNumber       = stageNumber;
    _startingNextStage = true;

    auto& stageData = _stageData[stageNumber];

    byte tileData[224] = {};
    SnowbrosDataParser::ParseStageMapData(stageData.filename, tileData, 224);

    if (0 != stageNumber)
    {
        // move player up

        Vector3 position = Vector3{0.f, 0.f, 1.f};
        Vector3 scale    = Vector3{1.f, 1.f, 1.f};
        Vector3 rotation = Vector3{0.f, 0.f, 0.f};

        position.x = -7.5f;
        position.y = 7.5f;

        auto tilemap = SpawnActor<IndexedTilemap>(position, scale, rotation);
        SnowbrosDataParser::ParseIndexedTexture("Tile", L"snowbros_tilemap.bmp");

        tilemap->SetTexture("Tile");
        tilemap->CreateTile(16, 14, {1.f, 1.f}, 0);
        tilemap->AddTileSprites({16.f, 16.f});

        for (size_t i = 0; i < 14; i++)
        {
            for (size_t j = 0; j < 16; j++)
            {
                Ptr<IndexedTile> tile
                  = Cast<Tile, IndexedTile>(tilemap->GetTile((13 - i) * 16 + j));

                int32 spriteIndex = tileData[i * 16 + j];
                tile->SetSprite(spriteIndex);

                tile->SetPalette(_tileMetadata.paletteNumbers[spriteIndex]);

                if (_tileMetadata.tiles[TileType::Block].contains(spriteIndex))
                    tile->SetTileType(TileType::Block);
            }
        }

        _nextTilemap = tilemap;

        int32 timerID = TimeManager::Instance().SetTimer(0.f, true,
          [this]()
          {
              float velocityY = -15.f / 5.f;

              auto deltaTime = TimeManager::Instance().GetDeltaTime();

              float previousTilemapThreshold = -20.5f;
              float nextTilemapThreshold     = -6.5f;

              Vector3 previousTilemapPosition = _tilemap->GetWorldPosition();
              previousTilemapPosition.y += velocityY * deltaTime;
              previousTilemapPosition.y
                = std::max(previousTilemapPosition.y, previousTilemapThreshold);
              _tilemap->SetWorldPosition(previousTilemapPosition);
              _tilemap->Refresh();

              if (nullptr == _nextTilemap)
                  return;

              Vector3 nextTilemapPosition = _nextTilemap->GetWorldPosition();
              nextTilemapPosition.y += velocityY * deltaTime;
              nextTilemapPosition.y = std::max(nextTilemapPosition.y, nextTilemapThreshold);
              _nextTilemap->SetWorldPosition(nextTilemapPosition);
              _nextTilemap->Refresh();
          });

        TimeManager::Instance().SetTimer(5.f, false,
          [this, &stageData, timerID]()
          {
              TimeManager::Instance().RemoveTimer(timerID);

              _tilemap->SetActive(false);
              _tilemap     = _nextTilemap;
              _nextTilemap = nullptr;

              Vector3 playerPosition = Vector3::one;

              playerPosition.x = stageData.playerPosition.x;
              playerPosition.y = stageData.playerPosition.y;

              _player->SetWorldPosition(playerPosition);
              _player->SetDirection(stageData.playerDirection);
              _player->ResetState();
              _player->StartStage();

              Vector3 scale    = 2 * Vector3::one;
              Vector3 rotation = Vector3::zero;

              for (auto& enemyData : stageData.enemies)
              {
                  Vector3 position = Vector3::one;

                  position.x = enemyData.position.x;
                  position.y = enemyData.position.y;

                  auto enemy = SpawnActor<SnowbrosEnemy>(position, scale, rotation);
                  enemy->SetEnemyType(enemyData.type);
                  enemy->SetDirection(enemyData.direction);
              }

              _startingNextStage = false;
          });

        return;
    }

    Vector3 playerPosition = Vector3::one;

    playerPosition.x = stageData.playerPosition.x;
    playerPosition.y = stageData.playerPosition.y;

    auto player = SpawnActor<SnowbrosPlayer>(playerPosition, 2 * Vector3::one, Vector3::zero);
    player->SetDirection(stageData.playerDirection);

    _player = player;
    _player->StartStage();

    for (size_t i = 0; i < 14; i++)
    {
        for (size_t j = 0; j < 16; j++)
        {
            Ptr<IndexedTile> tile = Cast<Tile, IndexedTile>(_tilemap->GetTile((13 - i) * 16 + j));

            int32 spriteIndex = tileData[i * 16 + j];
            tile->SetSprite(spriteIndex);

            tile->SetPalette(_tileMetadata.paletteNumbers[spriteIndex]);

            if (_tileMetadata.tiles[TileType::Block].contains(spriteIndex))
                tile->SetTileType(TileType::Block);
        }
    }

    Vector3 scale    = 2 * Vector3::one;
    Vector3 rotation = Vector3::zero;
    for (auto& enemyData : stageData.enemies)
    {
        Vector3 position = Vector3::one;

        position.x = enemyData.position.x;
        position.y = enemyData.position.y;

        auto enemy = SpawnActor<SnowbrosEnemy>(position, scale, rotation);
        enemy->SetEnemyType(enemyData.type);
        enemy->SetDirection(enemyData.direction);
    }

    _startingNextStage = false;
}
