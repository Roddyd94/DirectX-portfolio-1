#include "pch.h"

#include "Player.h"

#include "Core/ResourceManager.h"

#include "PlayerController.h"
#include "Core/Level.h"

bool Player::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Pawn::Init(id, position, scale, rotation);

    Ptr<Level> level = GetLevel();

    _playerController
      = level->SpawnActor<PlayerController>(Vector3::zero, Vector3::zero, Vector3::zero);
    _playerController->SetName("Controller");
    _playerController->SetPawn(This<Pawn>());

    _playerComponent = CreateActorComponent<PlayerComponent>("Player");

    return true;
}

Ptr<class PlayerController> Player::GetController() const
{
    return _playerController;
}
