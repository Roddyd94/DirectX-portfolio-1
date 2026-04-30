#pragma once
#include "CollisionComponent.h"

class SphereCollisionComponent : public CollisionComponent
{
public:
    SphereCollisionComponent();
    ~SphereCollisionComponent() override;

private:
    float _radius = 0.f;

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

    float GetRadius() const { return _radius; }
    void  SetRadius(float radius) { _radius = radius; }
};
