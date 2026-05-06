#include "pch.h"

#include "Pawn.h"

#include "Level.h"

bool Pawn::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    return Actor::Init(id, position, scale, rotation);
}

void Pawn::Destroy()
{
    Actor::Destroy();

    Ptr<Level> level = GetLevel();
    if (!level)
        return;

    level->RemoveActor(_controller->GetActorID());
}

void Pawn::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Pawn::Collision(float deltaTime)
{
    Actor::Collision(deltaTime);
}

void Pawn::SetController(Ptr<Controller> controller)
{
    if (_controller)
        DESTROY(_controller);

    _controller = controller;
}
