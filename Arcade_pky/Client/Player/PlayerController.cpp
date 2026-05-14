#include "pch.h"

#include "PlayerController.h"

#include "Core/Input/InputComponent.h"
#include "Core/Input/InputSystem.h"

bool PlayerController::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Controller::Init(id, position, scale, rotation);

    _inputComponent = CreateActorComponent<InputComponent>("Input");
    InputSystem::Instance().RegisterInputComponent(GetActorID(), _inputComponent);

    return true;
}

void PlayerController::Destroy()
{
    Controller::Destroy();

    InputSystem::Instance().RemoveInputComponent(GetActorID(), _inputComponent->GetComponentID());
}

void PlayerController::Tick(float deltaTime)
{
    Controller::Tick(deltaTime);
}

void PlayerController::Collision(float deltaTime)
{
    Controller::Collision(deltaTime);
}

Ptr<class InputComponent> PlayerController::GetInputComponent() const
{
    return _inputComponent;
}
