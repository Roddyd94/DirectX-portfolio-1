#include "pch.h"

#include "AI.h"

#include "AIComponent.h"
#include "AIController.h"
#include "Core/Level.h"

bool AI::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Pawn::Init(id, position, scale, rotation);

    Ptr<Level> level = GetLevel();

    _controller = level->SpawnActor<AIController>(Vector3::zero, Vector3::zero, Vector3::zero);
    _controller->SetName("Controller");
    _controller->SetPawn(This<Pawn>());

    return true;
}

Ptr<class AIController> AI::GetController() const
{
    return Cast<Controller, AIController>(_controller);
}

Ptr<class AIComponent> AI::GetAIComponent() const
{
    return GetController()->GetAIComponent();
}
