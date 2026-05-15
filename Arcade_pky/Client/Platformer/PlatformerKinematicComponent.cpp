#include "pch.h"

#include "PlatformerKinematicComponent.h"

#include "Common/LogManager.h"

#include "Core/Actor.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Tilemap/Tile.h"
#include "Tilemap/Tilemap.h"
#include "Tilemap/TilemapLevel.h"

bool PlatformerKinematicComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    ActorComponent::Init(componentID, name, owner);

    return true;
}

void PlatformerKinematicComponent::Destroy()
{
    ActorComponent::Destroy();
}

void PlatformerKinematicComponent::Tick(float deltaTime)
{
    ActorComponent::Tick(deltaTime);

    Ptr<Actor> actor = GetOwner();

    Vector2 worldPos = actor->GetWorldPosition().ToVector2();
    worldPos += _velocity * deltaTime;

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

void PlatformerKinematicComponent::AttachTo(Ptr<class Actor> actor)
{
    if (nullptr == actor)
        _attachedTo = nullptr;

    if (GetOwner()->GetActorID() == actor->GetActorID())
        return;

    _attachedTo = actor;
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

void PlatformerKinematicComponent::AdjustPositionToFloor(Vector2& worldPos2D, Vector2 delta)
{
    Rect colliderBox = _collider->GetBox();
    colliderBox.Move(delta);

    Vector2   colliderCenterBottom = {worldPos2D.x, colliderBox.bottom};
    Ptr<Tile> tile                 = _tilemap->GetTile(colliderCenterBottom);

    float targetPositionY
      = tile->GetWorldPosition().y + tile->GetSize().y / 2 + _collider->GetBoxSize().y / 2;
    worldPos2D.y = targetPositionY - epsilonTile;
}

bool PlatformerKinematicComponent::IsColliderOnFloor(Vector2 delta)
{
    Rect colliderBox = _collider->GetBox();
    colliderBox.Move(delta);

    Ptr<Tile> tileCenterBottom
      = _tilemap->GetTile({_collider->GetWorldPosition().x, colliderBox.bottom});
    Ptr<Tile> tileLeftBottom  = _tilemap->GetTile({colliderBox.left, colliderBox.bottom});
    Ptr<Tile> tileRightBottom = _tilemap->GetTile({colliderBox.right, colliderBox.bottom});

    if (nullptr == tileLeftBottom || nullptr == tileRightBottom)
    {
        if (tileCenterBottom->IsFloor())
            return true;

        return false;
    }

    if (!tileCenterBottom->IsFloor() && tileCenterBottom->IsCeiling()
        && (tileLeftBottom->IsFloor() || tileRightBottom->IsFloor()))
        return false;

    if (tileCenterBottom->IsFloor() || tileLeftBottom->IsFloor() || tileRightBottom->IsFloor())
        return true;

    return false;
}

bool PlatformerKinematicComponent::IsColliderTouchedWall(Vector2 delta)
{
    if (std::abs(delta.x) < FLT_EPSILON)
        return false;

    Rect colliderBox = _collider->GetBox();
    colliderBox.Move(delta);

    Vector2 tilePositionUpper = {0.f, colliderBox.top};
    Vector2 tilePositionLower = {0.f, colliderBox.bottom + epsilonTile};

    if (delta.x > 0)
    {
        tilePositionUpper.x = colliderBox.right;
        tilePositionLower.x = colliderBox.right;
    }
    else
    {
        tilePositionUpper.x = colliderBox.left;
        tilePositionLower.x = colliderBox.left;
    }

    Ptr<Tile> tileUpper = _tilemap->GetTile(tilePositionUpper);
    Ptr<Tile> tileLower = _tilemap->GetTile(tilePositionLower);

    return (tileUpper && tileUpper->IsWall()) || (tileLower && tileLower->IsWall());
}

bool PlatformerKinematicComponent::IsColliderTouchedBoundary(Vector2 delta)
{
    Rect colliderBox = _collider->GetBox();
    colliderBox.Move(delta);

    Ptr<TilemapLevel> level    = Cast<Level, TilemapLevel>(GetLevel());
    Rect              boundary = level->GetBoundary();

    return colliderBox.left < boundary.left || colliderBox.right > boundary.right
        || colliderBox.top > boundary.top || colliderBox.bottom < boundary.bottom;
}

bool PlatformerKinematicComponent::IsColliderMoveAgainstBoundaryX(Vector2 delta)
{
    Rect colliderBox = _collider->GetBox();
    colliderBox.Move(delta);

    Ptr<TilemapLevel> level    = Cast<Level, TilemapLevel>(GetLevel());
    Rect              boundary = level->GetBoundary();

    if (delta.x > 0)
        return colliderBox.right > boundary.right;
    else
        return colliderBox.left < boundary.left;

    return false;
}

bool PlatformerKinematicComponent::IsPositionOutOfBoundary(Vector2 position)
{
    Ptr<TilemapLevel> level    = Cast<Level, TilemapLevel>(GetLevel());
    Rect              boundary = level->GetBoundary();

    return position.x < boundary.left || position.x > boundary.right || position.y > boundary.top
        || position.y < boundary.bottom;
}
