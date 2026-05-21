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

    if (this->IsColliderTouchedBoundary(delta) && _onCollidedWithBoundary)
        _onCollidedWithBoundary();

    if (this->IsColliderTouchedBlock(delta) && _onCollidedWithBlock)
        _onCollidedWithBlock();

    if (this->IsColliderMoveAgainstFloor(delta) && _onCollidedWithFloor)
        _onCollidedWithFloor();

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

void PlatformerKinematicComponent::SetVelocity(Vector2 velocity)
{
    _velocity = velocity;
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
        if (tileCenterBottom->IsTopBlock())
            return true;

        return false;
    }

    if ((tileCenterBottom->IsBlock() && !tileCenterBottom->IsTopBlock())
        && (tileLeftBottom->IsTopBlock() || tileRightBottom->IsTopBlock()))
        return false;

    if (tileCenterBottom->IsTopBlock() || tileLeftBottom->IsTopBlock()
        || tileRightBottom->IsTopBlock())
        return true;

    return false;
}

bool PlatformerKinematicComponent::IsColliderBottomOnBlock(Vector2 delta)
{
    Rect colliderBox = _collider->GetBox();
    colliderBox.Move(delta);

    Ptr<Tile> tile = _tilemap->GetTile({colliderBox.GetCenterX(), colliderBox.bottom});

    return tile && tile->IsBlock() && !tile->IsBottomBlock();
}

bool PlatformerKinematicComponent::IsColliderTouchedBlock(Vector2 delta)
{
    Rect colliderBox = _collider->GetBox();
    colliderBox.Move(delta);

    Ptr<Tile> tileLT = _tilemap->GetTile({colliderBox.left, colliderBox.top});
    Ptr<Tile> tileLB = _tilemap->GetTile({colliderBox.left, colliderBox.bottom + epsilonTile});
    Ptr<Tile> tileRT = _tilemap->GetTile({colliderBox.right, colliderBox.top});
    Ptr<Tile> tileRB = _tilemap->GetTile({colliderBox.right, colliderBox.bottom + epsilonTile});

    bool isTileLTBlock = tileLT && tileLT->IsBlock() && !tileLT->IsBottomBlock();
    bool isTileLBBlock = tileLB && tileLB->IsBlock() && !tileLB->IsBottomBlock();
    bool isTileRTBlock = tileRT && tileRT->IsBlock() && !tileRT->IsBottomBlock();
    bool isTileRBBlock = tileRB && tileRB->IsBlock() && !tileRB->IsBottomBlock();

    return isTileLTBlock || isTileLBBlock || isTileRTBlock || isTileRBBlock;
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

bool PlatformerKinematicComponent::IsColliderMoveAgainstFloor(Vector2 delta)
{
    Rect colliderBox = _collider->GetBox();

    Ptr<Tile> tilePrevLB = _tilemap->GetTile({colliderBox.left, colliderBox.bottom});
    Ptr<Tile> tilePrevRB = _tilemap->GetTile({colliderBox.right, colliderBox.bottom});

    colliderBox.Move(delta);

    Ptr<Tile> tileNextLB = _tilemap->GetTile({colliderBox.left, colliderBox.bottom});
    Ptr<Tile> tileNextRB = _tilemap->GetTile({colliderBox.right, colliderBox.bottom});

    bool didLeftSideTouchedTile
      = (tilePrevLB && tileNextLB) ? !tilePrevLB->IsTopBlock() && tileNextLB->IsTopBlock() : false;
    bool didRightSideTouchedTile
      = (tilePrevRB && tileNextRB) ? !tilePrevRB->IsTopBlock() && tileNextRB->IsTopBlock() : false;

    if (didLeftSideTouchedTile || didRightSideTouchedTile)
        return true;

    return false;
}

bool PlatformerKinematicComponent::IsColliderMoveAgainstWallX(float deltaX)
{
    if (deltaX == 0)
        return false;

    if (deltaX > 0)
        return IsTileOnColliderBoundaryBlocked(Direction::RightTop)
            || IsTileOnColliderBoundaryBlocked(Direction::RightBottom);
    else
        return IsTileOnColliderBoundaryBlocked(Direction::LeftTop)
            || IsTileOnColliderBoundaryBlocked(Direction::LeftBottom);
}

bool PlatformerKinematicComponent::IsColliderMoveAgainstBoundaryX(float deltaX)
{
    Rect colliderBox = _collider->GetBox();
    colliderBox.MoveX(deltaX);

    Ptr<TilemapLevel> level    = Cast<Level, TilemapLevel>(GetLevel());
    Rect              boundary = level->GetBoundary();

    if (deltaX > 0)
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

bool PlatformerKinematicComponent::IsTileOnColliderBoundaryBlocked(Direction::Type direction)
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
        return false;

    return tile->IsBlock() && !tile->IsBottomBlock();
}

void PlatformerKinematicComponent::AdjustPositionToFloor(Vector2 delta)
{
    Ptr<Actor> actor = GetOwner();

    Rect colliderBox = _collider->GetBox();
    colliderBox.Move(delta);

    Vector2   colliderCenterBottom = {colliderBox.GetCenterX(), colliderBox.bottom};
    Ptr<Tile> tile                 = _tilemap->GetTile(colliderCenterBottom);

    float targetPositionY = tile->GetWorldPosition().y + tile->GetSize().y / 2
                          + _collider->GetBoxSize().y / 2 - epsilonTile;

    actor->SetWorldPosition({colliderCenterBottom.x, targetPositionY});
}
