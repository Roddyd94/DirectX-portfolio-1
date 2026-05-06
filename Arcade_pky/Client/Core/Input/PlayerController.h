#pragma once
#include "Core/Controller.h"

class PlayerController : public Controller
{
public:
    PlayerController()           = default;
    ~PlayerController() override = default;
    DELETE_SPECIAL_FUNC(PlayerController)

private:
    Ptr<class InputComponent> _inputComponent;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;

    Ptr<class InputComponent> GetInputComponent() const;
};
