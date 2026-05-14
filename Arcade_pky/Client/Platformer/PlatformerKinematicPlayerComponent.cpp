#include "pch.h"

#include "PlatformerKinematicPlayerComponent.h"

#include "Core/Actor.h"

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

    switch (_state)
    {
    case PlatformerKinematicState::OnGround:
    {
        if (IsColliderTouchedBoundaryX(worldPos, _velocity.x)
            || IsColliderTouchedWall(worldPos, _velocity.x))
            return;

        worldPos.x += _velocity.x * deltaTime;

        bool isOnFloor = IsColliderOnFloor(worldPos);
        if (!isOnFloor)
            ChangeStateTo(PlatformerKinematicState::OnAir);
    }
    break;
    case PlatformerKinematicState::OnAir:
    {
        if (!IsColliderTouchedBoundaryX(worldPos, _velocity.x)
            && !IsColliderTouchedWall(worldPos, _velocity.x))
            worldPos.x += _velocity.x * deltaTime;

        bool isOnFloorPrev = IsColliderOnFloor(worldPos);
        worldPos.y += _velocity.y * deltaTime;
        bool isOnFloorNext = IsColliderOnFloor(worldPos);

        if (_velocity.y < 0 && !isOnFloorPrev && isOnFloorNext)
        {
            AdjustPositionToFloor(worldPos);
            _velocity.y = 0.f;
            ChangeStateTo(PlatformerKinematicState::OnGround);
        }
    }
    break;
    case PlatformerKinematicState::OnSlope:
        break;
    case PlatformerKinematicState::OnLadder:
        break;
    case PlatformerKinematicState::Attached:
    {
        if (_attachedTo)
            worldPos = _attachedTo->GetWorldPosition().ToVector2();
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
    _onStateChangedTo[_state]();
}
