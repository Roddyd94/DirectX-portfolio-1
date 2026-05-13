#include "pch.h"

#include "PlatformerKinematicComponent.h"

#include "Common/LogManager.h"

#include "Core/Actor.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Test/PlayerComponent.h"
#include "Test/PlayerState.h"
#include "Test/PlayerStateGround.h"
#include "Test/PlayerStateMidair.h"
#include "Test/TestLevel.h"
#include "Tilemap/Tile.h"
#include "Tilemap/Tilemap.h"

bool PlatformerKinematicComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    ActorComponent::Init(componentID, name, owner);

    Ptr<Actor> actor = GetOwner();
    _playerComponent = actor->FindActorComponent<PlayerComponent>("Player");

    return true;
}

void PlatformerKinematicComponent::Destroy()
{
    ActorComponent::Destroy();
}

void PlatformerKinematicComponent::Tick(float deltaTime)
{
    ActorComponent::Tick(deltaTime);

    Ptr<Actor> actor    = GetOwner();
    Vector2    worldPos = actor->GetWorldPosition().ToVector2();

    PlayerStateType playerState = _playerComponent->GetStateType();
    switch (playerState)
    {
    case PlayerStateType::Ground:
    {
        if (IsColliderTouchedBoundaryX(worldPos, _velocity.x)
            || IsColliderTouchedWall(worldPos, _velocity.x))
            break;

        worldPos.x += _velocity.x * deltaTime;

        bool isOnFloor = IsColliderOnFloor(worldPos);
        if (!isOnFloor)
            _playerComponent->Transition(New<PlayerStateMidair>(false));
    }
    break;
    case PlayerStateType::Midair:
    {
        bool isOnFloorPrev = IsColliderOnFloor(worldPos);
        worldPos.y += _velocity.y * deltaTime;
        bool isOnFloorNext = IsColliderOnFloor(worldPos);

        if (!IsColliderTouchedBoundaryX(worldPos, _velocity.x)
            && !IsColliderTouchedWall(worldPos, _velocity.x))
            worldPos.x += _velocity.x * deltaTime;

        if (_velocity.y < 0 && !isOnFloorPrev && isOnFloorNext)
        {
            AdjustPositionToFloor(worldPos);
            _velocity.y = 0.f;
            _playerComponent->Transition(PlayerState::ground);
        }
    }
    break;
    case PlayerStateType::Snowball:
        // TODO PlayerComponent에 눈덩이 관련 넣기
        break;
    default:
        return;
    }

    actor->SetWorldPosition(worldPos);
}

Vector2 PlatformerKinematicComponent::GetVelocity() const
{
    return _velocity;
}

void PlatformerKinematicComponent::SetTilemap(Ptr<class Tilemap> tilemap)
{
    _tilemap = tilemap;
}

void PlatformerKinematicComponent::SetCollider(Ptr<class AABBCollisionComponent> collider)
{
    _collider = collider;
}

void PlatformerKinematicComponent::MoveX(float speed)
{
    _velocity.x = speed;
}

void PlatformerKinematicComponent::AddForce(Vector2 force)
{
    _velocity += force;
}

void PlatformerKinematicComponent::AddGravity(float deltaTime)
{
    _velocity.y += gravity * deltaTime;
    _velocity.y = std::max(-terminalVelocityY, _velocity.y);
}

void PlatformerKinematicComponent::AdjustPositionToFloor(Vector2& worldPos2D)
{
    float colliderExtentY = _collider->GetBoxSize().y / 2.f;

    Vector2 colliderBottomCenter = worldPos2D;
    colliderBottomCenter.y -= colliderExtentY;
    Ptr<Tile> tile = _tilemap->GetTile(colliderBottomCenter);

    float tileTopPositionY = tile->GetWorldPosition().y + tile->GetSize().y / 2;
    worldPos2D.y           = tileTopPositionY + colliderExtentY - epsilonTile;
}

bool PlatformerKinematicComponent::IsColliderOnFloor(Vector2 worldPos2D)
{
    float colliderExtentX = _collider->GetBoxSize().x / 2.f;
    float colliderExtentY = _collider->GetBoxSize().y / 2.f;

    Vector2 nextBottomCenter = worldPos2D;
    nextBottomCenter.y -= colliderExtentY;
    Ptr<Tile> tileBottom = _tilemap->GetTile(nextBottomCenter);

    Vector2 nextBottomLeft = nextBottomCenter;
    nextBottomLeft.x -= colliderExtentX;
    Ptr<Tile> tileBottomLeft = _tilemap->GetTile(nextBottomLeft);

    Vector2 nextBottomRight = nextBottomCenter;
    nextBottomRight.x += colliderExtentX;
    Ptr<Tile> tileBottomRight = _tilemap->GetTile(nextBottomRight);

    if (nullptr == tileBottomLeft || nullptr == tileBottomRight)
    {
        if (tileBottom->IsFloor())
            return true;

        return false;
    }

    if (!tileBottom->IsFloor() && tileBottom->IsCeiling()
        && (tileBottomLeft->IsFloor() || tileBottomRight->IsFloor()))
        return false;

    if (tileBottom->IsFloor() || tileBottomLeft->IsFloor() || tileBottomRight->IsFloor())
        return true;

    return false;
}

bool PlatformerKinematicComponent::IsColliderTouchedWall(Vector2 worldPos2D, float deltaX)
{
    float colliderExtentX = _collider->GetBoxSize().x / 2.f;

    Vector2 colliderBoundary = worldPos2D;
    float   direction        = deltaX > 0 ? 1.f : -1.f;
    colliderBoundary.x += colliderExtentX * direction;

    Ptr<Tile> tile = _tilemap->GetTile(colliderBoundary);

    return tile->IsWall();
}

bool PlatformerKinematicComponent::IsColliderTouchedBoundaryX(Vector2 worldPos2D, float deltaX)
{
    float colliderExtentX = _collider->GetBoxSize().x / 2.f;

    Vector2 colliderBoundary = worldPos2D;
    bool    isMovingRight    = deltaX > 0;
    float   direction        = isMovingRight ? 1.f : -1.f;
    colliderBoundary.x += colliderExtentX * direction;

    if (isMovingRight)
        return colliderBoundary.x > TestLevel::boundaryEnd.x;
    else
        return colliderBoundary.x < TestLevel::boundaryStart.x;

    return false;
}
