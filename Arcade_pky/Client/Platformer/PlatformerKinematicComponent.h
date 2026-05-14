#pragma once
#include "Types.h"
#include "Core/ActorComponent.h"

class PlatformerKinematicComponent : public ActorComponent
{
public:
    PlatformerKinematicComponent()           = default;
    ~PlatformerKinematicComponent() override = default;

private:
    std::function<void()>             _onStateChangedTo[PlatformerKinematicState::End];
    Ptr<class AABBCollisionComponent> _collider   = nullptr;
    Ptr<class Tilemap>                _tilemap    = nullptr;
    Ptr<class Actor>                  _attachedTo = nullptr;

    Vector2 _velocity;
    bool    _useGravity = true;

    PlatformerKinematicState::Type _state = PlatformerKinematicState::OnAir;

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
    void ChangeStateTo(PlatformerKinematicState::Type state);
    void SetUseGravity(bool useGravity);
    void AttachTo(Ptr<class Actor> actor);

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
    void RegisterOnStateChangedCallback(
      PlatformerKinematicState::Type state, T* obj, void (T::*memFunc)())
    {
        _onStateChangedTo[state] = std::bind(memFunc, obj);
    }

    template <typename T>
    void RegisterOnStateChangedCallback(PlatformerKinematicState::Type state, T&& func)
    {
        _onStateChangedTo[state] = std::forward<T>(func);
    }
};
