#include "pch.h"

#include "PlayerStateSnowball.h"

#include "PlayerStateMidair.h"
#include "SnowbrosPlayerBlackboard.h"
#include "Types.h"
#include "Core/Animation/SpriteComponent.h"
#include "Core/Collision/CollisionComponent.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Player/Player.h"
#include "Player/PlayerComponent.h"
#include "Player/PlayerController.h"

const Ptr<PlayerStateSnowball> PlayerStateSnowball::instance = New<PlayerStateSnowball>();

PlayerStateSnowball::PlayerStateSnowball()
{
    _stateType = SnowbrosPlayerStateType::Snowball;
}

Ptr<PlayerState> PlayerStateSnowball::HandleInput(Ptr<class PlayerComponent> playerComponent,
  Ptr<class InputAction>                                                     action,
  ButtonEventType::Type                                                      buttonEvent)
{
    // if (action)

    return nullptr;
}

void PlayerStateSnowball::Enter(Ptr<class PlayerComponent> playerComponent)
{
    Ptr<Player> player = playerComponent->GetPlayer();

    Ptr<PlatformerKinematicComponent> kinematic
      = player->FindActorComponent<PlatformerKinematicComponent>("Kinematic");
    kinematic->SetVelocity(Vector2::zero);

    auto controller = player->GetController();
    controller->SetActiveContext("Snowball");

    Ptr<SpriteComponent> sprite = player->FindSceneComponent<SpriteComponent>("Root");
    sprite->ChangeAnimation("player_shoved");
}

void PlayerStateSnowball::Exit(Ptr<class PlayerComponent> playerComponent) {}

void PlayerStateSnowball::Tick(Ptr<class PlayerComponent> playerComponent, float deltaTime)
{
    Ptr<Player> player = playerComponent->GetPlayer();

    auto blackboard = GetBlackboard(playerComponent);
    auto kinematic  = player->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

    auto snowballCollider = Lock(blackboard->bindTargetSnowball);

    if (nullptr == snowballCollider)
    {
        blackboard->bindTargetSnowball.reset();
        playerComponent->Transition(New<PlayerStateMidair>(true));
        return;
    }

    Vector2 otherPosition = snowballCollider->GetWorldPosition().ToVector2();
    player->SetWorldPosition(otherPosition);
}
