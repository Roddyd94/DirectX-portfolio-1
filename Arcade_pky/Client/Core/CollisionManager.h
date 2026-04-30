#pragma once
#include "SubManager.h"

#include "Types.h"

class CollisionManager : public SubManager
{
public:
    CollisionManager()          = default;
    virtual ~CollisionManager() = default;

private:
    std::vector<ComponentIDPair>                             _collidersToRemove;
    std::map<ComponentIDPair, Ptr<class CollisionComponent>> _colliders;

public:
    bool Init() override;
    void Destroy() override;
    void Collision(float deltaTime);

    Ptr<class CollisionComponent> FindCollider(
      const ComponentIDPair& colliderID);

    void Insert(const ComponentIDPair& colliderID,
      Ptr<class CollisionComponent>    collider);
    void Remove(const ComponentIDPair& colliderID);
};
