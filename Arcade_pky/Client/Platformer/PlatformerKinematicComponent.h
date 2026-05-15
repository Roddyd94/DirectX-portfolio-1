#pragma once
#include "Types.h"
#include "Core/ActorComponent.h"
#include "Core/Types.h"
#include "Tilemap/Tile.h"

class PlatformerKinematicComponent : public ActorComponent
{
public:
    PlatformerKinematicComponent()           = default;
    ~PlatformerKinematicComponent() override = default;

protected:
    std::map<TileType::Type, std::function<void()>> _onCollidedWithTileCallbacks;

    Ptr<class AABBCollisionComponent> _collider   = nullptr;
    Ptr<class Tilemap>                _tilemap    = nullptr;
    Ptr<class Actor>                  _attachedTo = nullptr;

    Vector2 _velocity;

public:
    inline static const float gravity           = -10.f;
    inline static const float terminalVelocityY = 5.f;
    inline static const float epsilonTile       = 0.01f;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    virtual void AttachTo(Ptr<class Actor> actor);

    Vector2 GetVelocity() const;

    void SetTilemap(Ptr<class Tilemap> tilemap);
    void SetCollider(Ptr<class AABBCollisionComponent> collider);

    void MoveX(float speed);
    void AddForce(Vector2 force);
    void AddGravity(float deltaTime);

protected:
    void AdjustPositionToFloor(Vector2& worldPos2D, Vector2 delta);
    bool IsColliderOnFloor(Vector2 delta);
    bool IsColliderTouchedTile(TileType::Type tileType, Vector2 delta = Vector2::zero);
    bool IsColliderTouchedWall(Vector2 delta);
    bool IsColliderTouchedFloor(Vector2 delta);
    bool IsColliderTouchedCeiling(Vector2 delta);
    bool IsColliderTouchedBoundary(Vector2 delta);
    bool IsColliderMoveAgainstBoundaryX(Vector2 delta);
    bool IsPositionOutOfBoundary(Vector2 position);

    TileType::Type GetAdjacentTileType(Direction::Type direction);

public:
    template <typename T>
    void RegisterOnCollidedTile(TileType::Type tileType, T* obj, void (T::*memFunc)())
    {
        _onCollidedWithTileCallbacks[tileType] = std::bind(memFunc, obj, std::placeholders::_1);
    }

    template <typename T>
    void RegisterOnCollidedTile(TileType::Type tileType, T&& func)
    {
        _onCollidedWithTileCallbacks[tileType] = std::forward<T>(func);
    }
};
