#pragma once
#include "Core/Pawn.h"

class Player : public Pawn
{
public:
    Player()           = default;
    ~Player() override = default;

protected:
    Ptr<class PlayerComponent>  _playerComponent;
    Ptr<class PlayerController> _playerController;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
};
