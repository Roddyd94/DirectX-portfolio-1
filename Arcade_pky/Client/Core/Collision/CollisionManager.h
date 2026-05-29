#pragma once
#include "Core/SubManager.h"

#include "CollisionSystem.h"
#include "Core/Types.h"

class CollisionManager : public SubManager
{
public:
    CollisionManager()          = default;
    virtual ~CollisionManager() = default;

private:
    std::vector<std::pair<ColliderType::Type, ComponentIDPair>> _collidersToRemove;
    std::map<ColliderType::Type, std::map<ComponentIDPair, Ptr<class CollisionComponent>>>
      _colliders;

public:
    bool Init() override;
    void Destroy() override;
    void Collision(float deltaTime);

    Ptr<class CollisionComponent> FindCollider(
      ColliderType::Type colliderType, const ComponentIDPair& colliderID);

    bool FindColliders(
      ColliderType::Type colliderType, std::vector<Weak<class CollisionComponent>>& arr);

    void Insert(Ptr<class CollisionComponent> collider);
    void Remove(Ptr<class CollisionComponent> collider);
};
