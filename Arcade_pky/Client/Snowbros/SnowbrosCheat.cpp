#include "pch.h"

#include "SnowbrosCheat.h"

#include "GoblinBlackboard.h"
#include "GoblinStateMachine.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosLevel.h"
#include "Core/CameraComponent.h"
#include "Core/Input/InputAction.h"
#include "Core/Input/InputComponent.h"
#include "Tilemap/IndexedTilemap.h"
#include "Tilemap/TilemapLevel.h"

#include <AI/AIComponent.h>
#include <Platformer/PlatformerKinematicComponent.h>

bool SnowbrosCheat::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Actor::Init(id, position, scale, rotation);

    auto input = CreateActorComponent<InputComponent>("Input");
    InputSystem::Instance().RegisterInputComponent(GetActorID(), input);

    input->BindAction("Cheat", "ChangePalette_0", '1', this, &SnowbrosCheat::ChangePalette);
    input->BindAction("Cheat", "ChangePalette_1", '2', this, &SnowbrosCheat::ChangePalette);
    input->BindAction("Cheat", "ChangePalette_2", '3', this, &SnowbrosCheat::ChangePalette);
    input->BindAction("Cheat", "ChangeLevel_0", VK_F1, this, &SnowbrosCheat::ChangeLevel);
    input->BindAction("Cheat", "ChangeLevel_1", VK_F2, this, &SnowbrosCheat::ChangeLevel);
    input->BindAction("Cheat", "ChangeLevel_2", VK_F3, this, &SnowbrosCheat::ChangeLevel);
    input->BindAction("Cheat", "ToggleCamera", VK_TAB, this, &SnowbrosCheat::ToggleCamera);
    input->BindAction("Cheat", "SpawnMonster_0", '4', this, &SnowbrosCheat::SpawnMonster);
    input->BindAction("Cheat", "SpawnMonster_1", '5', this, &SnowbrosCheat::SpawnMonster);
    input->BindAction("Cheat", "SpawnMonster_2", '6', this, &SnowbrosCheat::SpawnMonster);
    input->BindAction("Cheat", "SpawnMonster_3", '7', this, &SnowbrosCheat::SpawnMonster);
    input->BindAction("Cheat", "SpawnMonster_4", '8', this, &SnowbrosCheat::SpawnMonster);
    input->BindAction("Cheat", "StartPlayer2", '9', this, &SnowbrosCheat::StartPlayer2);

    input->BindAction(
      "Cheat", "SpawnMonster_Stress_50", VK_F4, this, &SnowbrosCheat::SpawnMonsterStress);
    input->BindAction(
      "Cheat", "SpawnMonster_Stress_100", VK_F5, this, &SnowbrosCheat::SpawnMonsterStress);
    input->BindAction(
      "Cheat", "SpawnMonster_Stress_200", VK_F6, this, &SnowbrosCheat::SpawnMonsterStress);

    input->BindAction("Cheat", "MakeSnowball", VK_F7, this, &SnowbrosCheat::MakeSnowball);

    for (float x = -7.5f; x < 8.f; x++)
        for (float y = -5.5f; y < 6.f; y++)
            _stressTestPositions.push_back({x, y});

    return true;
}

void SnowbrosCheat::SpawnMonster(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
{
    if (buttonEvent != ButtonEventType::Down)
        return;

    auto level = Cast<Level, TilemapLevel>(GetLevel());

    Vector3 position = {0.f, 4.f, 1.f};

    auto enemy = level->SpawnActor<SnowbrosEnemy>(position, Vector3::one * 2, Vector3::zero);

    if (action->GetName() == "SpawnMonster_0")
        enemy->SetEnemyType(SnowbrosEnemyType::Goblin);
    else if (action->GetName() == "SpawnMonster_1")
        enemy->SetEnemyType(SnowbrosEnemyType::Monkey);
    else if (action->GetName() == "SpawnMonster_2")
        enemy->SetEnemyType(SnowbrosEnemyType::Spitter);
    else if (action->GetName() == "SpawnMonster_3")
        enemy->SetEnemyType(SnowbrosEnemyType::Spawn);
    else if (action->GetName() == "SpawnMonster_4")
    {
        Vector3 position = enemy->GetWorldPosition();
        position.y -= 4.f;
        enemy->SetWorldPosition(position);
        enemy->SetEnemyType(SnowbrosEnemyType::Boss);
    }

    enemy->SetDirection(-1.f);
}

void SnowbrosCheat::SpawnMonsterStress(
  Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
{
    int32 count = 0;

    if (action->GetName() == "SpawnMonster_Stress_50")
        count = 50;
    else if (action->GetName() == "SpawnMonster_Stress_100")
        count = 100;
    else if (action->GetName() == "SpawnMonster_Stress_200")
        count = 200;

    if (buttonEvent != ButtonEventType::Down)
        return;

    auto level = Cast<Level, SnowbrosLevel>(GetLevel());
    if (nullptr == level)
        return;

    Reset();
    level->RemoveEnemies();

    for (size_t i = 0; i < count; i++)
    {
        auto spawnPosition = _stressTestPositions[_stressTestPositionIndex];
        auto enemy         = level->SpawnActor<SnowbrosEnemy>(
          {spawnPosition.first, spawnPosition.second, 1.f}, Vector3::one * 2, Vector3::zero);

        enemy->SetEnemyType(SnowbrosEnemyType::Goblin);
        Ptr<AIComponent>      aiComponent = enemy->GetAIComponent();
        Ptr<GoblinBlackboard> blackboard;
        if (nullptr == aiComponent)
            goto end;

        blackboard = aiComponent->GetBlackboard<GoblinBlackboard>();
        if (nullptr == blackboard)
            goto end;

        blackboard->walkSpeedX           = 0.f;
        blackboard->snowballDecPerSecond = 0.f;
        // blackboard->disableSnowballRepulsion = true;

        _stressTestPositionIndex = ++_stressTestPositionIndex % _stressTestPositions.size();

        _stressTestEntries.push_back(enemy);

    end:
        enemy->SetDirection(-1.f);
    }
}

void SnowbrosCheat::ChangePalette(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
{
    if (buttonEvent != ButtonEventType::Down)
        return;

    auto level   = Cast<Level, TilemapLevel>(GetLevel());
    auto tilemap = Cast<Tilemap, IndexedTilemap>(level->GetTilemap());

    if (action->GetName() == "ChangePalette_0")
        tilemap->SetPaletteSet(0);
    else if (action->GetName() == "ChangePalette_1")
        tilemap->SetPaletteSet(1);
    else if (action->GetName() == "ChangePalette_2")
        tilemap->SetPaletteSet(2);
}

void SnowbrosCheat::ChangeLevel(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
{
    if (buttonEvent != ButtonEventType::Down)
        return;

    auto level = Cast<Level, SnowbrosLevel>(GetLevel());
    level->RemoveItems();
    level->RemoveEnemies();
    level->RemoveScorePopups();

    if (action->GetName() == "ChangeLevel_0")
        level->StartStage(0);
    else if (action->GetName() == "ChangeLevel_1")
        level->StartStage(1);
    else if (action->GetName() == "ChangeLevel_2")
        level->StartStage(2);
}

void SnowbrosCheat::ToggleCamera(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
{
    if (buttonEvent != ButtonEventType::Down)
        return;

    auto level  = Cast<Level, SnowbrosLevel>(GetLevel());
    auto camera = level->GetMainCamera();

    static int8 toggle = 0;

    ++toggle %= 2;

    camera->SetViewResolution(
      (toggle + 1) * defaultViewVolume.width, (toggle + 1) * defaultViewVolume.height);

    camera->SetProjectionType(CameraType ::Orthographic);
}

void SnowbrosCheat::StartPlayer2(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
{
    if (buttonEvent != ButtonEventType::Down)
        return;

    auto level            = Cast<Level, SnowbrosLevel>(GetLevel());
    level->_enablePlayer2 = true;
}

void SnowbrosCheat::MakeSnowball(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
{
    if (buttonEvent != ButtonEventType::Down)
        return;

    if (_stressTestEntries.empty())
        return;

    Ptr<SnowbrosEnemy> enemy = Lock(_stressTestEntries[_stressTestEntryIndex]);
    if (nullptr == enemy)
        return;

    Ptr<AIComponent> aiComponent = enemy->GetAIComponent();
    if (nullptr == aiComponent)
        return;

    Ptr<GoblinStateMachine> stateMachine
      = Cast<AIStateMachine, GoblinStateMachine>(aiComponent->GetAIStateMachine());
    if (nullptr == stateMachine)
        return;

    Ptr<GoblinBlackboard> blackboard = stateMachine->GetBlackboard<GoblinBlackboard>();
    if (nullptr == blackboard)
        return;

    blackboard->accTime = 9000.f;
    stateMachine->Transition("Snowball");
    _stressTestEntryIndex = ++_stressTestEntryIndex % _stressTestEntries.size();

    auto kinematic = enemy->FindComponent<PlatformerKinematicComponent>("Kinematic");
    if (nullptr == kinematic)
        return;
}

void SnowbrosCheat::Reset()
{
    _stressTestEntries.clear();
    _stressTestEntryIndex    = 0;
    _stressTestPositionIndex = 0;
}
