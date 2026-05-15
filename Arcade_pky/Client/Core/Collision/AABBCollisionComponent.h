#pragma once
#include "CollisionComponent.h"
#include "CollisionSystem.h"

class AABBCollisionComponent : public CollisionComponent
{
public:
    AABBCollisionComponent();
    ~AABBCollisionComponent() override;

protected:
    Vector2 _boxSize;
    Rect    _box;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;
    void Render(float deltaTime) override;

    bool Collision(Weak<CollisionComponent> other) override;
    void AttachToComponent(Ptr<SceneComponent> comp) override;

    Vector2 GetBoxSize() const { return _boxSize; }
    Rect    GetBox() const { return _box; }

    void SetBoxSize(Vector2 size) { SetBoxSize(size.x, size.y); }
    void SetBoxSize(float x, float y)
    {
        _boxSize.x = x;
        _boxSize.y = y;
        SetWorldScale(_boxSize);
    }
};
