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

    Vector2 delta = _velocity * deltaTime;

    Ptr<Actor> actor    = GetOwner();
    Vector2    worldPos = actor->GetWorldPosition().ToVector2();
    worldPos += delta;

    for (auto& [tileType, callback] : _onCollidedWithTileCallbacks)
    {
        if (!this->IsColliderTouchedTile(tileType, delta))
            continue;

        if (callback)
            callback();
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

bool PlatformerKinematicComponent::IsColliderTouchedTile(TileType::Type tileType, Vector2 delta)
{
    Rect colliderBox = _collider->GetBox();
    colliderBox.Move(delta);

    Ptr<Tile> tileLT = _tilemap->GetTile({colliderBox.left, colliderBox.top});
    Ptr<Tile> tileLB = _tilemap->GetTile({colliderBox.left, colliderBox.bottom + epsilonTile});
    Ptr<Tile> tileRT = _tilemap->GetTile({colliderBox.right, colliderBox.top});
    Ptr<Tile> tileRB = _tilemap->GetTile({colliderBox.right, colliderBox.bottom + epsilonTile});

    return (tileLT && tileLT->GetType() == tileType) || (tileLB && tileLB->GetType() == tileType)
        || (tileRT && tileRT->GetType() == tileType) || (tileRB && tileRB->GetType() == tileType);
}

bool PlatformerKinematicComponent::IsColliderTouchedWall(Vector2 delta)
{
    return IsColliderTouchedTile(TileType::IsWall, delta);
}

bool PlatformerKinematicComponent::IsColliderTouchedFloor(Vector2 delta)
{
    return IsColliderTouchedTile(TileType::IsFloor, delta);
}

bool PlatformerKinematicComponent::IsColliderTouchedCeiling(Vector2 delta)
{
    return IsColliderTouchedTile(TileType::IsCeiling, delta);
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

TileType::Type PlatformerKinematicComponent::GetAdjacentTileType(Direction::Type direction)
{
    Ptr<Tile> tile             = nullptr;
    Rect      colliderBox      = _collider->GetBox();
    Vector2   colliderPosition = _collider->GetWorldPosition().ToVector2();

    switch (direction)
    {
    case Direction::Top:
        tile = _tilemap->GetTile({colliderPosition.x, colliderBox.top});
        break;
    case Direction::RightTop:
        tile = _tilemap->GetTile({colliderBox.right, colliderBox.top});
        break;
    case Direction::Right:
        tile = _tilemap->GetTile({colliderBox.right, colliderPosition.y});
        break;
    case Direction::RightBottom:
        tile = _tilemap->GetTile({colliderBox.right, colliderBox.bottom + epsilonTile});
        break;
    case Direction::Bottom:
        tile = _tilemap->GetTile({colliderPosition.x, colliderBox.bottom + epsilonTile});
        break;
    case Direction::LeftBottom:
        tile = _tilemap->GetTile({colliderBox.left, colliderBox.bottom + epsilonTile});
        break;
    case Direction::Left:
        tile = _tilemap->GetTile({colliderBox.left, colliderPosition.y});
        break;
    case Direction::LeftTop:
        tile = _tilemap->GetTile({colliderBox.left, colliderBox.top});
        break;
    }

    if (nullptr == tile)
        return TileType::End;

    return tile->GetType();
}
