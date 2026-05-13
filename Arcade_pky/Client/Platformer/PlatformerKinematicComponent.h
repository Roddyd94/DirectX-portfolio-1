#pragma once
#include "Core/ActorComponent.h"

class PlatformerKinematicComponent : public ActorComponent
{
public:
    PlatformerKinematicComponent()           = default;
    ~PlatformerKinematicComponent() override = default;

private:
    std::vector<std::function<void()>> _onLandCallbacks;
    Ptr<class AABBCollisionComponent>  _collider = nullptr;
    Ptr<class Tilemap>                 _tilemap  = nullptr;
    Ptr<class PlayerComponent>         _playerComponent;

    Vector2 _velocity;
    bool    _useGravity = true;

public:
    inline static const float gravity           = -10.f;
    inline static const float terminalVelocityY = 5.f;
    inline static const float epsilonTile       = 0.01f;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    Vector2 GetVelocity() const;

    void SetTilemap(Ptr<class Tilemap> tilemap);
    void SetCollider(Ptr<class AABBCollisionComponent> collider);

    void MoveX(float speed);
    void AddForce(Vector2 force);
    void AddGravity(float deltaTime);

private:
    void AdjustPositionToFloor(Vector2& worldPos2D);
    bool IsColliderOnFloor(Vector2 worldPos2D);
    bool IsColliderTouchedWall(Vector2 worldPos2D, float deltaX);
    bool IsColliderTouchedBoundaryX(Vector2 worldPos2D, float deltaX);

public:
    template <typename T>
    void RegisterOnLandCallback(T* obj, void (T::*memFunc)())
    {
        std::function<void()> callback = std::bind(memFunc, obj);
        _onLandCallbacks.push_back(callback);
    }

    template <typename T>
    void RegisterOnLandCallback(T&& func)
    {
        std::function<void()> callback = std::forward<T>(func);
        _onLandCallbacks.push_back(callback);
    }
};
