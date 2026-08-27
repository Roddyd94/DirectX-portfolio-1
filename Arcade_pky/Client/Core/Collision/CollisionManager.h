#pragma once
#include "Core/SubManager.h"

#include "CollisionSystem.h"
#include "Core/Types.h"

#include <array>

class CollisionManager : public SubManager
{
public:
    CollisionManager()          = default;
    virtual ~CollisionManager() = default;

private:
    std::vector<std::pair<ColliderType::Type, ComponentIDPair>> _collidersToRemove;
    std::map<ColliderType::Type, std::map<ComponentIDPair, Ptr<class CollisionComponent>>>
      _colliders;

private:
    static inline int32 _collidedCount = 0;
    static inline int32 _enterCount    = 0;
    static inline int32 _stayCount     = 0;
    static inline int32 _exitCount     = 0;

    std::array<int32, 4> _lastCollisionCounts = {};

public:
    bool Init() override;
    void Destroy() override;
    void Collision(float deltaTime);

    const std::array<int32, 4>& GetLastCollisionCounts() const;

    Ptr<class CollisionComponent> FindCollider(
      ColliderType::Type colliderType, const ComponentIDPair& colliderID);

    bool FindColliders(
      ColliderType::Type colliderType, std::vector<Weak<class CollisionComponent>>& arr);

    void Insert(Ptr<class CollisionComponent> collider);
    void Remove(Ptr<class CollisionComponent> collider);
};
