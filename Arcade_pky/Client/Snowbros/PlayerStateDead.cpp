#include "pch.h"

#include "PlayerStateDead.h"

#include "Types.h"
#include "Player/Player.h"
#include "Player/PlayerComponent.h"
#include "Player/PlayerController.h"

PlayerStateDead::PlayerStateDead()
{
    _stateType = SnowbrosPlayerStateType::Dead;
}

Ptr<PlayerState> PlayerStateDead::HandleInput(Ptr<class PlayerComponent> playerComponent,
  Ptr<InputAction>                                                       action,
  ButtonEventType::Type                                                  buttonEvent)
{
    return nullptr;
}

void PlayerStateDead::Enter(Ptr<class PlayerComponent> playerComponent)
{
    Ptr<Player> player = playerComponent->GetPlayer();

    auto controller = player->GetController();
    controller->SetActiveContext("Dead");
}

void PlayerStateDead::Exit(Ptr<class PlayerComponent> playerComponent) {}

void PlayerStateDead::Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime) {}
