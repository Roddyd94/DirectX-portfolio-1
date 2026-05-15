#pragma once
#include "Types.h"
#include "Core/ActorComponent.h"

class PlatformerKinematicComponent : public ActorComponent
{
public:
    PlatformerKinematicComponent()           = default;
    ~PlatformerKinematicComponent() override = default;

protected:
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
    bool IsColliderTouchedWall(Vector2 delta);
    bool IsColliderTouchedBoundary(Vector2 delta);
    bool IsColliderMoveAgainstBoundaryX(Vector2 delta);
    bool IsPositionOutOfBoundary(Vector2 position);
};
