#include "pch.h"

#include "PlatformerKinematicPlayerComponent.h"

#include "Core/Collision/CollisionManager.h"

#include "Core/Actor.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Player/PlayerComponent.h"
#include "Snowbros/SnowballMorphableEnemyStateMachine.h"
#include "Snowbros/SnowbrosLevel.h"
#include "Snowbros/SnowbrosPlayerState.h"

bool PlatformerKinematicPlayerComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    PlatformerKinematicComponent::Init(componentID, name, owner);

    return true;
}

void PlatformerKinematicPlayerComponent::Destroy()
{
    PlatformerKinematicComponent::Destroy();
}

void PlatformerKinematicPlayerComponent::Tick(float deltaTime)
{
    ActorComponent::Tick(deltaTime);

    Ptr<Actor> actor = GetOwner();

    Vector2 worldPos = actor->GetWorldPosition().ToVector2();
    Vector2 delta    = _velocity * deltaTime;

    switch (_state)
    {
    case PlatformerKinematicState::OnGround:
    {
        if (IsColliderMovingAgainstBoundaryX(delta.x) || IsColliderMovingAgainstWallX(delta.x))
            return;

        worldPos.x += delta.x;

        bool isOnFloor = IsColliderOnFloor(delta);

        if (!isOnFloor)
            ChangeStateTo(PlatformerKinematicState::OnAir);
    }
    break;
    case PlatformerKinematicState::OnStandable:
    {
        if (IsColliderMovingAgainstBoundaryX(delta.x) || IsColliderMovingAgainstWallX(delta.x))
            return;

        worldPos.x += delta.x;

        auto snowballCollider = FindSnowballToStand();

        if (nullptr == snowballCollider)
            ChangeStateTo(PlatformerKinematicState::OnAir);
    }
    break;
    case PlatformerKinematicState::OnAir:
    {
        if (!IsColliderMovingAgainstBoundaryX(delta.x) && !IsColliderMovingAgainstWallX(delta.x))
            worldPos.x += delta.x;

        worldPos.y += delta.y;
        bool isOnFloorPrev = IsColliderOnFloor({delta.x, 0.f});
        bool isOnFloorNext = IsColliderOnFloor(delta);

        bool movedTowardFloor = !isOnFloorPrev && isOnFloorNext;

        if (delta.y < 0 && movedTowardFloor)
        {
            AdjustPositionToFloor(delta);
            _velocity.y = 0.f;
            ChangeStateTo(PlatformerKinematicState::OnGround);

            return;
        }

        auto snowballCollider = FindSnowballToStand();
        if (delta.y < 0 && nullptr != snowballCollider)
        {
            AdjustPositionToSnowball(snowballCollider);
            _velocity.y = 0.f;
            ChangeStateTo(PlatformerKinematicState::OnStandable);

            return;
        }
    }
    break;
    case PlatformerKinematicState::OnSlope:
        break;
    case PlatformerKinematicState::OnLadder:
        break;
    case PlatformerKinematicState::Attached:
    {
        auto attachedTo = Lock(_attachedTo);
        if (attachedTo)
            worldPos = attachedTo->GetWorldPosition().ToVector2();
    }
    break;
    }

    actor->SetWorldPosition(worldPos);
}

void PlatformerKinematicPlayerComponent::AttachTo(Ptr<class Actor> actor)
{
    PlatformerKinematicComponent::AttachTo(actor);

    if (nullptr != actor)
        _state = PlatformerKinematicState::Attached;
}

void PlatformerKinematicPlayerComponent::ChangeStateTo(PlatformerKinematicState::Type state)
{
    _state = state;
    if (_onStateChangedTo[_state])
        _onStateChangedTo[_state]();
}

Ptr<class AABBCollisionComponent> PlatformerKinematicPlayerComponent::FindSnowballToStand()
{
    auto actor  = GetOwner();
    auto player = actor->FindActorComponent<PlayerComponent>("Player");

    auto playerStateMachine = player->GetStateMachine();
    auto playerState
      = Cast<PlayerState, SnowbrosPlayerState>(playerStateMachine->GetCurrentState());

    return playerState->FindSnowballToStand(player);
}

void PlatformerKinematicPlayerComponent::AdjustPositionToSnowball(
  Ptr<class AABBCollisionComponent> snowballCollider)
{
    Ptr<Actor> actor = GetOwner();

    auto collider = Lock(_collider);

    Rect playerColliderBox   = collider->GetBox();
    Rect snowballColliderBox = snowballCollider->GetBox();

    Vector2 colliderCenterBottom = {playerColliderBox.GetCenterX(), playerColliderBox.bottom};

    float targetPositionY
      = snowballColliderBox.top + collider->GetBoxSize().y / 2.f - epsilonTile / 2.f;

    actor->SetWorldPosition({colliderCenterBottom.x, targetPositionY});
}
