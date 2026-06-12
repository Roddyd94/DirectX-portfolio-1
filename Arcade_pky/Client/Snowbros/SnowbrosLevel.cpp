#include "pch.h"

#include "SnowbrosLevel.h"

#include "Core/ResourceManager.h"
#include "Core/TimeManager.h"

#include "SnowbrosCheat.h"
#include "SnowbrosDataParser.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosPlayer.h"
#include "SnowbrosUI.h"
#include "Core/Camera.h"
#include "Core/Input/InputContext.h"
#include "Core/Input/InputSystem.h"
#include "Core/InstanceRendererComponent.h"
#include "Core/Mesh.h"
#include "Core/Texture.h"
#include "Snowbros/IndexedSpriteInstanceRenderer.h"
#include "Tilemap/IndexedTilemap.h"
#include "Tilemap/Tile.h"

bool SnowbrosLevel::Init(Ptr<class World> world, const std::string& path)
{
    TilemapLevel::Init(world, path);

    InputSystem::Instance().FindOrAddInputContext("None");

    auto contextGround1   = InputSystem::Instance().FindOrAddInputContext("Ground1");
    auto contextMidair1   = InputSystem::Instance().FindOrAddInputContext("Midair1");
    auto contextSnowball1 = InputSystem::Instance().FindOrAddInputContext("Snowball1");
    auto contextGround2   = InputSystem::Instance().FindOrAddInputContext("Ground2");
    auto contextMidair2   = InputSystem::Instance().FindOrAddInputContext("Midair2");
    auto contextSnowball2 = InputSystem::Instance().FindOrAddInputContext("Snowball2");

    auto actionMoveLeft    = InputSystem::Instance().FindOrAddInputAction("MoveLeft");
    auto actionMoveRight   = InputSystem::Instance().FindOrAddInputAction("MoveRight");
    auto actionJump        = InputSystem::Instance().FindOrAddInputAction("Jump");
    auto actionShootGround = InputSystem::Instance().FindOrAddInputAction("ShootGround");
    auto actionShootMidair = InputSystem::Instance().FindOrAddInputAction("ShootMidair");

    contextGround1->BindInputAction(actionMoveLeft, 'A');
    contextGround1->BindInputAction(actionMoveRight, 'D');
    contextGround1->BindInputAction(actionJump, 'T');
    contextGround1->BindInputAction(actionShootGround, 'R');

    contextMidair1->BindInputAction(actionMoveLeft, 'A');
    contextMidair1->BindInputAction(actionMoveRight, 'D');
    contextMidair1->BindInputAction(actionShootMidair, 'R');

    contextSnowball1->BindInputAction(actionJump, 'T');

    contextGround2->BindInputAction(actionMoveLeft, 'H');
    contextGround2->BindInputAction(actionMoveRight, 'K');
    contextGround2->BindInputAction(actionJump, 'P');
    contextGround2->BindInputAction(actionShootGround, 'O');

    contextMidair2->BindInputAction(actionMoveLeft, 'H');
    contextMidair2->BindInputAction(actionMoveRight, 'K');
    contextMidair2->BindInputAction(actionShootMidair, 'O');

    contextSnowball2->BindInputAction(actionJump, 'P');

    Vector3 position = Vector3{0.f, 0.f, 1.f};
    Vector3 scale    = Vector3{1.f, 1.f, 1.f};
    Vector3 rotation = Vector3{0.f, 0.f, 0.f};

    SpawnActor<SnowbrosCheat>(position, scale, rotation);

    SnowbrosDataParser::ReadPalettes(L"snowbros_palettes.bin");

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
    spriteInstanceRenderer
      = SpawnInstanceRenderer<IndexedSpriteInstanceRenderer>("Score", position, scale, rotation);
    spriteInstanceRenderer->SetTexture(playerTexture);

    position.x = -8.f;
    position.y = 7.f;

    _ui = SpawnActor<SnowbrosUI>(position, Vector3::one, Vector3::zero);

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

    if (nullptr == _player[0])
        SpawnPlayer(0);
    if (_enablePlayer2 && nullptr == _player[1])
        SpawnPlayer(1);

    std::vector<Ptr<Actor>> enemies;
    FindActors("Enemy", enemies);
    std::vector<Ptr<Actor>> items;
    FindActors("Item", items);
    std::vector<Ptr<Actor>> scores;
    FindActors("Score", scores);

    if (!enemies.size() && !items.size() && !scores.size())
    {
        // auto enemy = SpawnActor<SnowbrosEnemy>(position, scale, rotation);
        // enemy->SetEnemyType(SnowbrosEnemyType::Spawn);
        // enemy->SetDirection(enemyData.direction);

        if (!_startingNextStage)
            StartStage(_stageNumber + 1);
    }
}

Ptr<class Player> SnowbrosLevel::FindNearestPlayerFrom(Vector2 position) const
{
    if (nullptr == _player[1])
        return _player[0];
    else if (nullptr == _player[0])
        return _player[1];

    Vector2 playerPosition1 = _player[0]->GetWorldPosition().ToVector2();
    Vector2 playerPosition2 = _player[1]->GetWorldPosition().ToVector2();

    return position.SquareDistance(playerPosition1) < position.SquareDistance(playerPosition2)
           ? _player[0]
           : _player[1];
}

Ptr<class Player> SnowbrosLevel::FindNearestPlayerFrom(Vector3 position) const
{
    return FindNearestPlayerFrom(position.ToVector2());
}

Ptr<class Player> SnowbrosLevel::GetPlayer(int32 number) const
{
    return _player[number];
}

void SnowbrosLevel::SetPlayer(int32 number, Ptr<class SnowbrosPlayer> player)
{
    _player[number] = player;
}

Ptr<class Player> SnowbrosLevel::SpawnPlayer(int32 number)
{
    auto& stageData = _stageData[_stageNumber];

    Vector3 playerPosition = Vector3::one;

    playerPosition.x = stageData.playerPosition[number].x;
    playerPosition.y = stageData.playerPosition[number].y;

    auto player = SpawnActor<SnowbrosPlayer>(playerPosition, 2 * Vector3::one, Vector3::zero);
    player->SetPlayerNumber(number);
    player->SetDirection(stageData.playerDirection[number]);

    player->StartStage();
    player->ResetState();
    _player[number] = player;

    _ui->SwitchUI("InsertCoin2P", false);
    _ui->Refresh();

    return player;
}

Ptr<class Item> SnowbrosLevel::SpawnItem(Vector3 position, Item::Type type)
{
    auto item = SpawnActor<Item>(position, Vector3::one, Vector3::zero);
    item->SetItemType(type);

    return item;
}

void SnowbrosLevel::AddScore(int32 playerNumber, int32 score)
{
    _score[playerNumber] += score;

    int32 highScore = std::max(40'000, std::max(_score[0], _score[1]));

    ScoreType::Type playerType = static_cast<ScoreType::Type>(playerNumber);
    _ui->SetScore(playerType, _score[playerNumber]);
    _ui->SetScore(ScoreType::HighScore, highScore);
}

void SnowbrosLevel::ResetScore(int32 playerNumber)
{
    _score[playerNumber] = 0;
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

    Vector3 position = Vector3{0.f, 0.f, 1.f};
    Vector3 scale    = Vector3{1.f, 1.f, 1.f};
    Vector3 rotation = Vector3{0.f, 0.f, 0.f};

    if (nullptr != _tilemap)
    {
        position.x = -7.5f;
        position.y = 7.5f;

        auto tilemap = SpawnActor<IndexedTilemap>(position, scale, rotation);
        SnowbrosDataParser::ParseIndexedTexture("Tile", L"snowbros_tilemap.bmp");

        tilemap->SetPaletteSet(stageData.paletteNumber);
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

        if (_player[0])
            _player[0]->EndStage();

        if (_player[1])
            _player[1]->EndStage();

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

              if (nullptr == _nextTilemap->GetRoot())
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

              if (_player[0])
              {
                  playerPosition.x = stageData.playerPosition[0].x;
                  playerPosition.y = stageData.playerPosition[0].y;
                  _player[0]->SetWorldPosition(playerPosition);
                  _player[0]->SetDirection(stageData.playerDirection[0]);
                  _player[0]->ResetState();
                  _player[0]->StartStage();
              }
              if (_player[1])
              {
                  playerPosition.x = stageData.playerPosition[1].x;
                  playerPosition.y = stageData.playerPosition[1].y;
                  _player[1]->SetWorldPosition(playerPosition);
                  _player[1]->SetDirection(stageData.playerDirection[1]);
                  _player[1]->ResetState();
                  _player[1]->StartStage();
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
                  enemy->SetPatrolInfo(enemyData.patrolData);
              }

              _startingNextStage = false;
          });

        return;
    }

    position.x = -7.5f;
    position.y = -6.5f;

    auto tilemap = SpawnActor<IndexedTilemap>(position, scale, rotation);
    SnowbrosDataParser::ParseIndexedTexture("Tile", L"snowbros_tilemap.bmp");

    tilemap->SetPaletteSet(stageData.paletteNumber);
    tilemap->SetTexture("Tile");
    tilemap->CreateTile(16, 14, {1.f, 1.f}, 0);
    tilemap->AddTileSprites({16.f, 16.f});
    _tilemap = tilemap;

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

    SpawnPlayer(0);
    if (_enablePlayer2)
        SpawnPlayer(1);

    scale    = 2 * Vector3::one;
    rotation = Vector3::zero;
    for (auto& enemyData : stageData.enemies)
    {
        Vector3 position = Vector3::one;

        position.x = enemyData.position.x;
        position.y = enemyData.position.y;

        auto enemy = SpawnActor<SnowbrosEnemy>(position, scale, rotation);
        enemy->SetEnemyType(enemyData.type);
        enemy->SetDirection(enemyData.direction);
        enemy->SetPatrolInfo(enemyData.patrolData);
    }

    _startingNextStage = false;
}

void SnowbrosLevel::RemoveItems()
{
    std::vector<Ptr<Actor>> items;
    FindActors("Item", items);

    for (auto& item : items)
        item->SetActive(false);
}

void SnowbrosLevel::RemoveEnemies()
{
    std::vector<Ptr<Actor>> enemies;
    FindActors("Enemy", enemies);

    for (auto& enemy : enemies)
        enemy->SetActive(false);
}

void SnowbrosLevel::RemoveScorePopups()
{
    std::vector<Ptr<Actor>> scores;
    FindActors("Score", scores);

    for (auto& score : scores)
        score->SetActive(false);
}
