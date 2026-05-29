#pragma once
#include "Actor.h"
#include "Controller.h"

class Pawn : public Actor
{
public:
    Pawn()           = default;
    ~Pawn() override = default;

protected:
    Ptr<Controller> _controller;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;

public:
    Ptr<class Controller> GetController() const;
    void SetController(Ptr<Controller> controller);

    template <typename T>
    Ptr<T> GetController() const
    {
        return Cast<Controller, T>(_controller);
    }
};
