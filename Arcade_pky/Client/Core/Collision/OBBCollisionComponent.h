#pragma once
#include "CollisionComponent.h"

class OBBCollisionComponent : public CollisionComponent
{
public:
    OBBCollisionComponent();
    ~OBBCollisionComponent() override;

protected:
    Vector2 _boxSize;
    OBB2D   _box;

public:
    bool Init(int32      componentID,
      const std::string& name,
      Ptr<class Actor>   owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;
    void Render(float deltaTime) override;

    bool Collision(Weak<CollisionComponent> other) override;
    void AttachToComponent(Ptr<SceneComponent> comp) override;

    const OBB2D&   GetBox() const { return _box; }
    Vector2 GetBoxSize() const { return _boxSize; }

    void SetBoxSize(Vector2 size) { SetBoxSize(size.x, size.y); }
    void SetBoxSize(float x, float y)
    {
        _boxSize.x = x;
        _boxSize.y = y;
    }
};
