#include "pch.h"

#include "CollisionManager.h"

#include "CollisionComponent.h"
#include "CollisionProfile.h"

bool CollisionManager::Init()
{
    return true;
}

void CollisionManager::Destroy()
{
    _colliders.clear();
}

void CollisionManager::Collision(float deltaTime)
{
    for (auto& [colliderType, colliderID] : _collidersToRemove)
    {
        auto it = _colliders.find(colliderType);
        if (_colliders.end() == it)
            continue;

        it->second.erase(colliderID);
    }
    _collidersToRemove.clear();

    for (auto& [srcColliderType, srcColliders] : _colliders)
    {
        for (auto& [srcColliderID, srcCollider] : srcColliders)
        {
            if (!srcCollider->IsEnable())
                continue;

            if (!srcCollider->IsActive())
                continue;

            Ptr<CollisionProfile> srcProfile = srcCollider->GetProfile();
            if (nullptr == srcProfile)
                continue;

            for (auto& [destColliderType, destColliders] : _colliders)
            {
                if (srcProfile->GetResponseTo(destColliderType) == CollisionResponse::Ignore
                    || srcProfile->GetResponseTo(destColliderType) == CollisionResponse::End)
                    continue;

                for (auto& [destColliderID, destCollider] : destColliders)
                {
                    if (srcColliderID == destColliderID)
                        continue;

                    if (!destCollider->IsEnable())
                        continue;

                    if (!destCollider->IsActive())
                        continue;

                    if (!destCollider->GetProfile())
                        continue;

                    if (srcCollider->Collision(destCollider))
                    {
                        switch (srcCollider->CheckCollisionState(destCollider))
                        {
                        case CollisionState::Enter:
                        case CollisionState::Stay:
                        {
                            srcCollider->Invoke(CollisionState::Stay, destCollider);
                        }
                        break;
                        case CollisionState::Exit:
                        case CollisionState::None:
                        {
                            srcCollider->Invoke(CollisionState::Enter, destCollider);
                        }
                        break;
                        }
                    }
                    else
                    {
                        switch (srcCollider->CheckCollisionState(destCollider))
                        {
                        case CollisionState::Enter:
                        case CollisionState::Stay:
                        {
                            srcCollider->Invoke(CollisionState::Exit, destCollider);
                        }
                        break;
                        }
                    }
                }
            }
        }
    }
}

Ptr<class CollisionComponent> CollisionManager::FindCollider(
  ColliderType::Type colliderType, const ComponentIDPair& colliderID)
{
    auto it = _colliders.find(colliderType);
    if (_colliders.end() == it)
        return nullptr;

    auto& collidersOfType = it->second;
    if (auto it = collidersOfType.find(colliderID); collidersOfType.end() != it)
        return it->second;

    return nullptr;
}

bool CollisionManager::FindColliders(
  ColliderType::Type colliderType, std::vector<Weak<class CollisionComponent>>& arr)
{
    auto it = _colliders.find(colliderType);
    if (_colliders.end() == it)
        return false;

    arr.clear();
    for (auto& [_, collider] : it->second)
        arr.push_back(collider);
}

void CollisionManager::Insert(Ptr<class CollisionComponent> collider)
{
    _colliders[collider->GetColliderType()][collider->GetColliderID()] = collider;
}

void CollisionManager::Remove(Ptr<class CollisionComponent> collider)
{
    _collidersToRemove.push_back({collider->GetColliderType(), collider->GetColliderID()});
}
