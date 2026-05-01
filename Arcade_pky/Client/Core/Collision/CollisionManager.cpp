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
    for (auto colliderID : _collidersToRemove)
        _colliders.erase(colliderID);

    _collidersToRemove.clear();

    for (auto& [srcColliderID, srcCollider] : _colliders)
    {
        if (!srcCollider->IsEnable())
            continue;

        if (!srcCollider->IsActive())
            continue;

        Ptr<CollisionProfile> srcProfile = srcCollider->GetProfile();
        if (nullptr == srcProfile)
            continue;

        for (auto& [destColliderID, destCollider] : _colliders)
        {
            if (!destCollider->IsEnable())
                continue;

            if (!destCollider->IsActive())
                continue;

            Ptr<CollisionProfile> destProfile = destCollider->GetProfile();
            if (nullptr == destProfile)
                continue;

            if (srcProfile->GetResponseTo(destProfile->GetColliderType())
                  == CollisionResponse::Ignore
                || srcProfile->GetResponseTo(destProfile->GetColliderType())
                     == CollisionResponse::End)
                continue;

            if (destProfile->GetResponseTo(srcProfile->GetColliderType())
                  == CollisionResponse::Ignore
                || destProfile->GetResponseTo(srcProfile->GetColliderType())
                     == CollisionResponse::End)
                continue;

            if (srcCollider->Collision(destCollider))
            {
                switch (srcCollider->CheckCollisionState(destColliderID))
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
                switch (srcCollider->CheckCollisionState(destColliderID))
                {
                case CollisionState::Enter:
                case CollisionState::Stay:
                {
                    srcCollider->Invoke(CollisionState::Exit, destCollider);
                }
                break;
                case CollisionState::Exit:
                case CollisionState::None:
                {
                    srcCollider->Invoke(CollisionState::None, destCollider);
                }
                break;
                }
            }
        }
    }
}

Ptr<class CollisionComponent> CollisionManager::FindCollider(
  const ComponentIDPair& colliderID)
{
    if (auto it = _colliders.find(colliderID); _colliders.end() == it)
        return it->second;

    return nullptr;
}

void CollisionManager::Insert(
  const ComponentIDPair& colliderID, Ptr<class CollisionComponent> collider)
{
    _colliders[colliderID] = collider;
}

void CollisionManager::Remove(const ComponentIDPair& colliderID)
{
    _collidersToRemove.push_back(colliderID);
}
