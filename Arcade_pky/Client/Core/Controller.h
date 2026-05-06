#pragma once
#include "Actor.h"

class Controller : public Actor
{
public:
    Controller()           = default;
    ~Controller() override = default;
    DELETE_SPECIAL_FUNC(Controller)

private:
    Weak<class Pawn> _pawn;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;

public:
    Ptr<class Pawn> GetPawn() const;

    void SetPawn(Ptr<class Pawn> pawn);
};
