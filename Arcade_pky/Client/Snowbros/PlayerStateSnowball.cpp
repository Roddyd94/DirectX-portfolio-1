#include "pch.h"

#include "PlayerStateSnowball.h"

#include "Core/TimeManager.h"

#include "PlayerStateMidair.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosPlayerBlackboard.h"
#include "Types.h"
#include "AI/AIComponent.h"
#include "Core/Animation/SpriteInstanceComponent.h"
#include "Core/Collision/CollisionComponent.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Platformer/PlatformerKinematicPlayerComponent.h"
#include "Platformer/PlatformerMovementComponent.h"
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
    Ptr<Actor> player = playerComponent->GetOwner();

    auto blackboard = GetBlackboard(playerComponent);
    auto movement   = player->FindActorComponent<PlatformerMovementComponent>("Movement");

    if (action->GetName() == "Jump")
    {
        switch (buttonEvent)
        {
        case ButtonEventType::Down:
        {
            Ptr<PlatformerKinematicPlayerComponent> kinematic
              = player->FindActorComponent<PlatformerKinematicPlayerComponent>("Kinematic");

            movement->Jump();
            kinematic->ChangeStateTo(PlatformerKinematicState::OnAir);
            blackboard->jumpedFromSnowball = true;

            return New<PlayerStateMidair>(true);
        }
        break;
        }
    }

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

    auto sprite = player->FindSceneComponent<SpriteInstanceComponent>("Sprite");
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
        return;

    Vector2 otherPosition = snowballCollider->GetWorldPosition().ToVector2();
    player->SetWorldPosition(otherPosition);

    auto pawn = Cast<Actor, SnowbrosEnemy>(snowballCollider->GetOwner());
    if (nullptr == pawn)
        return;

    auto ai = pawn->GetAIComponent();
    if (nullptr == ai)
        return;

    auto snowballStateMachine = ai->GetAIStateMachine();
    if (nullptr == snowballStateMachine)
        return;

    auto snowballState = snowballStateMachine->GetCurrentState();
    if (nullptr == snowballState)
        return;

    if (snowballState->GetName() == "SnowballCrashing")
    {
        blackboard->bindTargetSnowball.reset();

        auto movement = player->FindActorComponent<PlatformerMovementComponent>("Movement");
        auto kinematic
          = player->FindActorComponent<PlatformerKinematicPlayerComponent>("Kinematic");

        movement->Jump();
        kinematic->ChangeStateTo(PlatformerKinematicState::OnAir);
        playerComponent->Transition(New<PlayerStateMidair>(true));
    }
}
