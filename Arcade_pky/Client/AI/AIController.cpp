#include "pch.h"

#include "AIController.h"

#include "AIComponent.h"

bool AIController::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Controller::Init(id, position, scale, rotation);

    _aiComponent = CreateActorComponent<AIComponent>("AI");
    _aiComponent->SetController(This<AIController>());

    return true;
}

void AIController::Destroy()
{
    DESTROY(_aiComponent);
}

void AIController::Tick(float deltaTime)
{
    Controller::Tick(deltaTime);
    _aiComponent->Tick(deltaTime);
}

void AIController::Collision(float deltaTime) {}

Ptr<class AIComponent> AIController::GetAIComponent() const
{
    return _aiComponent;
}

Ptr<class AIStateMachine> AIController::GetAIStateMachine() const
{
    if (!_aiComponent)
        return nullptr;

    return _aiComponent->GetAIStateMachine();
}
