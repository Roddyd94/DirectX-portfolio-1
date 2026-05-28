#pragma once
#include "CollisionComponent.h"
#include "CollisionSystem.h"

class PointCollisionComponent : public CollisionComponent
{
public:
    PointCollisionComponent();
    ~PointCollisionComponent() override;
    
public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;
    void Render(float deltaTime) override;

    bool Collision(Weak<CollisionComponent> other) override;
};
