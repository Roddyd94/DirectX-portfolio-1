#include "pch.h"

#include "Controller.h"

bool Controller::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Actor::Init(id, position, scale, rotation);

    SetEnable(false);

    return true;
}

void Controller::Destroy()
{
    Actor::Destroy();
}

void Controller::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Controller::Collision(float deltaTime) {}

Ptr<Pawn> Controller::GetPawn() const
{
    return Lock<Pawn>(_pawn);
}

void Controller::SetPawn(Ptr<Pawn> pawn)
{
    _pawn = pawn;
}
