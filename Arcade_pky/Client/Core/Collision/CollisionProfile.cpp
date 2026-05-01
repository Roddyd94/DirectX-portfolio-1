#include "pch.h"

#include "CollisionProfile.h"

void CollisionProfile::Destroy() {}

const std::string& CollisionProfile::GetName() const
{
    return _name;
}

const ColliderType::Type CollisionProfile::GetColliderType() const
{
    return _thisType;
}

const CollisionResponse::Type CollisionProfile::GetResponseTo(
  ColliderType::Type otherType)
{
    return _responses[otherType];
}

void CollisionProfile::SetProfileResponse(CollisionChannel channel)
{
    auto& [otherType, response] = channel;
    _responses[otherType]       = response;
}
