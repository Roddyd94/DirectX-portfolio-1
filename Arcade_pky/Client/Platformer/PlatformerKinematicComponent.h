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
    std::function<void()> _onCollidedWithBlock;
    std::function<void()> _onCollidedWithFloor;
    std::function<void()> _onCollidedWithBoundary;

    Weak<class AABBCollisionComponent> _collider;
    Weak<class Tilemap>                _tilemap;
    Weak<class Actor>                  _attachedTo;

    Vector2 _velocity;

public:
    inline static const float gravity           = -10.f;
    inline static const float terminalVelocityY = 10.f;
    inline static const float epsilonTile       = 0.2f;
    inline static const float correctionToTile       = 0.0625f;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    virtual void AttachTo(Ptr<class Actor> actor);

    Vector2 GetVelocity() const;

    void SetTilemap(Ptr<class Tilemap> tilemap);
    void SetCollider(Ptr<class AABBCollisionComponent> collider);
    void SetVelocity(Vector2 velocity);
    void SetVelocityX(float velocityX);
    void SetVelocityY(float velocityY);
    void MoveX(float deltaX);

    void AddForce(Vector2 force);
    void AddGravity(float deltaTime);

    bool IsColliderOnFloor(Vector2 delta = Vector2::zero);
    bool IsColliderOnFirstFloor();
    bool IsColliderBottomOnBlock(Vector2 delta = Vector2::zero);
    bool IsColliderTouchedBlock(Vector2 delta = Vector2::zero);
    bool IsColliderTouchedBoundary(Vector2 delta = Vector2::zero);

    bool IsColliderMovingAgainstFloor(Vector2 delta);
    bool IsColliderMovingAgainstWallX(float deltaX);
    bool IsColliderMovingAgainstBoundaryX(float deltaX);
    bool IsPositionOutOfBoundary(Vector2 position);

    void AdjustPositionToFloor(Vector2 delta = Vector2::zero);
    void AdjustPositionX(Vector2& position);

protected:
    bool IsTileOnColliderBoundaryBlocked(Direction::Type direction);

public:
    template <typename T>
    void RegisterOnCollideWithBlock(T* obj, void (T::*memFunc)())
    {
        _onCollidedWithBlock = std::bind(memFunc, obj);
    }

    template <typename T>
    void RegisterOnCollideWithBlock(T&& func)
    {
        _onCollidedWithBlock = std::forward<T>(func);
    }

    template <typename T>
    void RegisterOnCollideWithFloor(T* obj, void (T::*memFunc)())
    {
        _onCollidedWithFloor = std::bind(memFunc, obj);
    }

    template <typename T>
    void RegisterOnCollideWithFloor(T&& func)
    {
        _onCollidedWithFloor = std::forward<T>(func);
    }

    template <typename T>
    void RegisterOnCollideWithBoundary(T* obj, void (T::*memFunc)())
    {
        _onCollidedWithBoundary = std::bind(memFunc, obj);
    }

    template <typename T>
    void RegisterOnCollideWithBoundary(T&& func)
    {
        _onCollidedWithBoundary = std::forward<T>(func);
    }
};
