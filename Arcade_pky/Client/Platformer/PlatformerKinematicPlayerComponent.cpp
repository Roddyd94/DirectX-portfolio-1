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
    Vector2 delta    = _velocity * deltaTime;

    switch (_state)
    {
    case PlatformerKinematicState::OnGround:
    {
        if (IsColliderMoveAgainstBoundaryX(delta) || IsColliderMoveAgainstWall(delta))
            return;

        worldPos.x += delta.x;

        bool isOnFloor = IsColliderOnFloor(delta);
        if (!isOnFloor)
            ChangeStateTo(PlatformerKinematicState::OnAir);
    }
    break;
    case PlatformerKinematicState::OnAir:
    {
        if (!IsColliderMoveAgainstBoundaryX(delta) && !IsColliderMoveAgainstWall(delta))
            worldPos.x += delta.x;

        bool isOnFloorPrev = IsColliderOnFloor({delta.x, 0.f});
        worldPos.y += delta.y;
        bool isOnFloorNext = IsColliderOnFloor(delta);

        if (delta.y < 0 && !isOnFloorPrev && isOnFloorNext)
        {
            AdjustPositionToFloor(worldPos, delta);
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

bool PlatformerKinematicPlayerComponent::IsColliderMoveAgainstWall(Vector2 delta)
{
    if (delta.x == 0)
        return false;

    if (delta.x > 0)
        return TileType::IsWall & GetAdjacentTileType(Direction::RightTop)
            || TileType::IsWall & GetAdjacentTileType(Direction::RightBottom);
    else
        return TileType::IsWall & GetAdjacentTileType(Direction::LeftTop)
            || TileType::IsWall & GetAdjacentTileType(Direction::LeftBottom);
}
