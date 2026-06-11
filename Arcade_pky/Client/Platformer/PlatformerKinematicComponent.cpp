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

    if (this->IsColliderTouchedBoundaryX(delta) && _onCollidedWithBoundaryX)
        _onCollidedWithBoundaryX();

    if (this->IsColliderTouchedBoundaryY(delta) && _onCollidedWithBoundaryY)
        _onCollidedWithBoundaryY();

    if (this->IsColliderTouchedBlock(delta) && _onCollidedWithBlock)
        _onCollidedWithBlock();

    if (this->IsColliderMovingAgainstFloor(delta) && _onCollidedWithFloor)
        _onCollidedWithFloor();

    if (!_canEscapeBoundaryX)
        AdjustPositionX(worldPos);

    actor->SetWorldPosition(worldPos);
}

Vector2 PlatformerKinematicComponent::GetVelocity() const
{
    return _velocity;
}

Ptr<class Tilemap> PlatformerKinematicComponent::GetTilemap() const
{
    auto level = Cast<Level, TilemapLevel>(GetLevel());

    return level->GetTilemap();
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
        _attachedTo.reset();

    if (GetOwner()->GetActorID() == actor->GetActorID())
        return;

    _attachedTo = actor;
}

void PlatformerKinematicComponent::SetVelocityX(float velocityX)
{
    _velocity.x = velocityX;
}

void PlatformerKinematicComponent::SetVelocityY(float velocityY)
{
    _velocity.y = velocityY;
}

void PlatformerKinematicComponent::SetCanEscapeBoundaryX(bool canEscapeBoundaryX)
{
    _canEscapeBoundaryX = canEscapeBoundaryX;
}

void PlatformerKinematicComponent::MoveX(float deltaX)
{
    Ptr<Actor> actor    = GetOwner();
    Vector2    worldPos = actor->GetWorldPosition().ToVector2();
    worldPos.x += deltaX;

    AdjustPositionX(worldPos);
    actor->SetWorldPosition(worldPos);
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
    auto collider    = Lock(_collider);
    Rect colliderBox = collider->GetBox();
    colliderBox.Move(delta);

    auto tilemap = GetTilemap();

    Ptr<Tile> tileCenterBottom = tilemap->GetTile({colliderBox.GetCenterX(), colliderBox.bottom});
    Ptr<Tile> tileLeftBottom   = tilemap->GetTile({colliderBox.left, colliderBox.bottom});
    Ptr<Tile> tileRightBottom  = tilemap->GetTile({colliderBox.right, colliderBox.bottom});

    if (tileCenterBottom && tileCenterBottom->IsTopBlock()
        || (tileLeftBottom && tileLeftBottom->IsTopBlock())
        || (tileRightBottom && tileRightBottom->IsTopBlock()))
    {
        Ptr<Tile> aTile = tileCenterBottom ? tileCenterBottom
                                           : (tileLeftBottom ? tileLeftBottom : tileRightBottom);

        Rect tileRect = aTile->GetRect();
        return std::abs(tileRect.top - colliderBox.bottom) < epsilonTile;
    }

    return false;
}

bool PlatformerKinematicComponent::IsColliderOnFirstFloor()
{
    auto collider = Lock(_collider);
    auto tilemap  = GetTilemap();

    int32 tileIndexY = tilemap->GetTileIndexY(collider->GetWorldPosition().y);

    return tileIndexY <= 1;
}

bool PlatformerKinematicComponent::IsColliderBottomOnBlock(Vector2 delta)
{
    auto collider = Lock(_collider);
    auto tilemap  = GetTilemap();

    Rect colliderBox = collider->GetBox();
    colliderBox.Move(delta);

    Ptr<Tile> tile = tilemap->GetTile({colliderBox.GetCenterX(), colliderBox.bottom});

    return tile && tile->IsBlock() && !tile->IsBottomBlock();
}

bool PlatformerKinematicComponent::IsColliderTouchedBlock(Vector2 delta)
{
    auto collider = Lock(_collider);
    auto tilemap  = GetTilemap();

    Rect colliderBox = collider->GetBox();
    colliderBox.Move(delta);

    Ptr<Tile> tileLT = tilemap->GetTile({colliderBox.left, colliderBox.top});
    Ptr<Tile> tileLB = tilemap->GetTile({colliderBox.left, colliderBox.bottom + epsilonTile});
    Ptr<Tile> tileRT = tilemap->GetTile({colliderBox.right, colliderBox.top});
    Ptr<Tile> tileRB = tilemap->GetTile({colliderBox.right, colliderBox.bottom + epsilonTile});

    bool isTileLTBlock = tileLT && tileLT->IsBlock() && !tileLT->IsBottomBlock();
    bool isTileLBBlock = tileLB && tileLB->IsBlock() && !tileLB->IsBottomBlock();
    bool isTileRTBlock = tileRT && tileRT->IsBlock() && !tileRT->IsBottomBlock();
    bool isTileRBBlock = tileRB && tileRB->IsBlock() && !tileRB->IsBottomBlock();

    return isTileLTBlock || isTileLBBlock || isTileRTBlock || isTileRBBlock;
}

bool PlatformerKinematicComponent::IsColliderTouchedBoundary(Vector2 delta)
{
    auto collider = Lock(_collider);
    auto tilemap  = GetTilemap();

    Rect colliderBox = collider->GetBox();
    colliderBox.Move(delta);

    Ptr<TilemapLevel> level    = Cast<Level, TilemapLevel>(GetLevel());
    Rect              boundary = level->GetBoundary();

    return colliderBox.left < boundary.left || colliderBox.right > boundary.right
        || colliderBox.top > boundary.top || colliderBox.bottom < boundary.bottom;
}

bool PlatformerKinematicComponent::IsColliderTouchedBoundaryX(Vector2 delta)
{
    auto collider = Lock(_collider);
    auto tilemap  = GetTilemap();

    Rect colliderBox = collider->GetBox();
    colliderBox.Move(delta);

    Ptr<TilemapLevel> level    = Cast<Level, TilemapLevel>(GetLevel());
    Rect              boundary = level->GetBoundary();

    return colliderBox.left < boundary.left || colliderBox.right > boundary.right;
}

bool PlatformerKinematicComponent::IsColliderTouchedBoundaryY(Vector2 delta)
{
    auto collider = Lock(_collider);
    auto tilemap  = GetTilemap();

    Rect colliderBox = collider->GetBox();
    colliderBox.Move(delta);

    Ptr<TilemapLevel> level    = Cast<Level, TilemapLevel>(GetLevel());
    Rect              boundary = level->GetBoundary();

    return colliderBox.top > boundary.top || colliderBox.bottom < boundary.bottom;
}

bool PlatformerKinematicComponent::IsColliderMovingAgainstFloor(Vector2 delta)
{
    auto collider = Lock(_collider);
    auto tilemap  = GetTilemap();

    Rect colliderBox = collider->GetBox();
    colliderBox.Move({delta.x, 0.f});

    Ptr<Tile> tilePrevLB = tilemap->GetTile({colliderBox.left, colliderBox.bottom});
    Ptr<Tile> tilePrevRB = tilemap->GetTile({colliderBox.right, colliderBox.bottom});

    colliderBox.Move({0.f, delta.y});

    Ptr<Tile> tileNextLB = tilemap->GetTile({colliderBox.left, colliderBox.bottom});
    Ptr<Tile> tileNextRB = tilemap->GetTile({colliderBox.right, colliderBox.bottom});

    bool didLeftSideTouchedTile
      = (tilePrevLB && tileNextLB) ? !tilePrevLB->IsTopBlock() && tileNextLB->IsTopBlock() : false;
    bool didRightSideTouchedTile
      = (tilePrevRB && tileNextRB) ? !tilePrevRB->IsTopBlock() && tileNextRB->IsTopBlock() : false;

    if (didLeftSideTouchedTile || didRightSideTouchedTile)
        return true;

    return false;
}

bool PlatformerKinematicComponent::IsColliderMovingAgainstWallX(float deltaX)
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

bool PlatformerKinematicComponent::IsColliderMovingAgainstBoundaryX(float deltaX)
{
    auto collider = Lock(_collider);

    Rect colliderBox = collider->GetBox();
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
    auto collider = Lock(_collider);
    auto tilemap  = GetTilemap();

    Ptr<Tile> tile             = nullptr;
    Rect      colliderBox      = collider->GetBox();
    Vector2   colliderPosition = collider->GetWorldPosition().ToVector2();

    switch (direction)
    {
    case Direction::Top:
        tile = tilemap->GetTile({colliderPosition.x, colliderBox.top});
        break;
    case Direction::RightTop:
        tile = tilemap->GetTile({colliderBox.right, colliderBox.top});
        break;
    case Direction::Right:
        tile = tilemap->GetTile({colliderBox.right, colliderPosition.y});
        break;
    case Direction::RightBottom:
        tile = tilemap->GetTile({colliderBox.right, colliderBox.bottom + epsilonTile});
        break;
    case Direction::Bottom:
        tile = tilemap->GetTile({colliderPosition.x, colliderBox.bottom + epsilonTile});
        break;
    case Direction::LeftBottom:
        tile = tilemap->GetTile({colliderBox.left, colliderBox.bottom + epsilonTile});
        break;
    case Direction::Left:
        tile = tilemap->GetTile({colliderBox.left, colliderPosition.y});
        break;
    case Direction::LeftTop:
        tile = tilemap->GetTile({colliderBox.left, colliderBox.top});
        break;
    }

    if (nullptr == tile)
        return false;

    return tile->IsBlock() && !tile->IsBottomBlock();
}

void PlatformerKinematicComponent::AdjustPositionToFloor(Vector2 delta)
{
    Ptr<Actor> actor = GetOwner();

    auto collider = Lock(_collider);
    auto tilemap  = GetTilemap();

    Rect colliderBox = collider->GetBox();
    colliderBox.Move(delta);

    Vector2   colliderCenterBottom = {colliderBox.GetCenterX(), colliderBox.bottom};
    Ptr<Tile> tile                 = tilemap->GetTile(colliderCenterBottom);

    if (nullptr == tile)
        return;

    float targetPositionY = tile->GetWorldPosition().y + tile->GetSize().y / 2.f
                          + collider->GetBoxSize().y / 2.f - correctionToTile / 2.f;

    actor->SetWorldPosition({colliderCenterBottom.x, targetPositionY});
}

void PlatformerKinematicComponent::AdjustPositionX(Vector2& position)
{
    auto collider = Lock(_collider);

    Ptr<TilemapLevel> level    = Cast<Level, TilemapLevel>(GetLevel());
    Rect              boundary = level->GetBoundary();

    auto colliderHalfSize = collider->GetBoxSize() / 2;

    if (position.x + colliderHalfSize.x >= boundary.right)
        position.x = boundary.right - colliderHalfSize.x;
    if (position.x - colliderHalfSize.x <= boundary.left)
        position.x = boundary.left + colliderHalfSize.x;
}
