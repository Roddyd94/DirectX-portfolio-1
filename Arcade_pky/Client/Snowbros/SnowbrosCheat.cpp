#include "pch.h"

#include "SnowbrosCheat.h"

#include "SnowbrosEnemy.h"
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
    input->BindAction("Cheat", "SpawnMonster_0", '4', this, &SnowbrosCheat::SpawnMonster);
    input->BindAction("Cheat", "SpawnMonster_1", '5', this, &SnowbrosCheat::SpawnMonster);
    input->BindAction("Cheat", "SpawnMonster_2", '6', this, &SnowbrosCheat::SpawnMonster);

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

    enemy->SetDirection(-1);
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
