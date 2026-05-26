#include "pch.h"

#include "AI.h"

#include "AIComponent.h"
#include "AIController.h"
#include "Core/Level.h"

bool AI::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Pawn::Init(id, position, scale, rotation);

    Ptr<Level> level = GetLevel();

    _aiController = level->SpawnActor<AIController>(Vector3::zero, Vector3::zero, Vector3::zero);
    _aiController->SetName("Controller");
    _aiController->SetPawn(This<Pawn>());

    _aiComponent = CreateActorComponent<AIComponent>("AI");

    return true;
}

Ptr<class AIController> AI::GetController() const
{
    return _aiController;
}
