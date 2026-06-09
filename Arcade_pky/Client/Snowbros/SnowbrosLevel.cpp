#include "pch.h"

#include "SnowbrosLevel.h"

#include "Core/ResourceManager.h"

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
    _tilemap = tilemap;

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

    position.x = 0.f;
    position.y = 0.f;
    scale *= 2.f;

    _player = SpawnActor<SnowbrosPlayer>(position, scale, rotation);
    _player->SetName("Player");

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

void SnowbrosLevel::StartStage(int32 stageNumber)
{
    if (stageNumber < 0 || stageNumber >= _stageData.size())
        return;

    auto stageData = _stageData[stageNumber];

    byte tileData[224] = {};
    SnowbrosDataParser::ParseStageMapData(stageData.filename, tileData, 224);

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
}
