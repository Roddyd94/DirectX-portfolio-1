#pragma once
#include "Core/Controller.h"

class AIController : public Controller
{
public:
    AIController()           = default;
    ~AIController() override = default;
    DELETE_SPECIAL_FUNC(AIController)
private:
    Ptr<class AIComponent> _aiComponent;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;

    Ptr<class AIComponent>   GetAIComponent() const;
    Ptr<class AIStateMachine> GetAIStateMachine() const;
};
