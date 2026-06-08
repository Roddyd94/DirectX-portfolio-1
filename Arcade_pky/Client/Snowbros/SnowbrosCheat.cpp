#include "pch.h"

#include "SnowbrosCheat.h"

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

    return true;
}

void SnowbrosCheat::ChangePalette(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
{
    auto level   = Cast<Level, TilemapLevel>(GetLevel());
    auto tilemap = Cast<Tilemap, IndexedTilemap>(level->GetTilemap());

    if (action->GetName() == "ChangePalette_0")
        tilemap->SetPaletteSet(0);
    else if (action->GetName() == "ChangePalette_1")
        tilemap->SetPaletteSet(1);
    else if (action->GetName() == "ChangePalette_2")
        tilemap->SetPaletteSet(2);
}
