#include "pch.h"

#include "SnowbrosCheat.h"

#include "SnowbrosEnemy.h"
#include "SnowbrosLevel.h"
#include "Core/CameraComponent.h"
#include "Core/Input/InputAction.h"
#include "Core/Input/InputComponent.h"
#include "Tilemap/IndexedTilemap.h"
#include "Tilemap/TilemapLevel.h"

bool SnowbrosCheat::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Actor::Init(id, position, scale, rotation);

    auto input = CreateActorComponent<InputComponent>("Input");
    InputSystem::Instance().RegisterInputComponent(GetActorID(), input);

    input->BindAction("Cheat", "ChangePalette_0", '1', this, &SnowbrosCheat::ChangePalette);
    input->BindAction("Cheat", "ChangePalette_1", '2', this, &SnowbrosCheat::ChangePalette);
    input->BindAction("Cheat", "ChangePalette_2", '3', this, &SnowbrosCheat::ChangePalette);
    input->BindAction("Cheat", "ChangeLevel_0", 'I', this, &SnowbrosCheat::ChangeLevel);
    input->BindAction("Cheat", "ChangeLevel_1", 'O', this, &SnowbrosCheat::ChangeLevel);
    input->BindAction("Cheat", "ChangeLevel_2", 'P', this, &SnowbrosCheat::ChangeLevel);
    input->BindAction("Cheat", "ToggleCamera", VK_TAB, this, &SnowbrosCheat::ToggleCamera);
    input->BindAction("Cheat", "SpawnMonster_0", '4', this, &SnowbrosCheat::SpawnMonster);
    input->BindAction("Cheat", "SpawnMonster_1", '5', this, &SnowbrosCheat::SpawnMonster);
    input->BindAction("Cheat", "SpawnMonster_2", '6', this, &SnowbrosCheat::SpawnMonster);
    input->BindAction("Cheat", "SpawnMonster_3", '7', this, &SnowbrosCheat::SpawnMonster);
    input->BindAction("Cheat", "SpawnMonster_4", '8', this, &SnowbrosCheat::SpawnMonster);

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
