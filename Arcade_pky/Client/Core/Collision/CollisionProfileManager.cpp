#include "pch.h"

#include "CollisionProfileManager.h"

bool CollisionProfileManager::Init()
{
    // TODO Read configs and create profiles?
    CreateChannel(ColliderType::Player, CollisionResponse::Ignore);
    CreateChannel(ColliderType::Enemy, CollisionResponse::Ignore);
    CreateChannel(ColliderType::PlayerProjectile, CollisionResponse::Ignore);
    CreateChannel(ColliderType::Item, CollisionResponse::Ignore);
    CreateChannel(ColliderType::Snowball, CollisionResponse::Ignore);

    CreateProfile("Player", ColliderType::Player);
    CreateProfile("Enemy", ColliderType::Enemy);
    CreateProfile("PlayerProjectile", ColliderType::PlayerProjectile);
    CreateProfile("Item", ColliderType::Item);
    CreateProfile("Snowball", ColliderType::Snowball);

    SetProfileResponse("Player", std::make_pair(ColliderType::Enemy, CollisionResponse::Overlap),
      std::make_pair(ColliderType::EnemyProjectile, CollisionResponse::Block),
      std::make_pair(ColliderType::Snowball, CollisionResponse::Block),
      std::make_pair(ColliderType::Item, CollisionResponse::Block));

    SetProfileResponse("Enemy",
      std::make_pair(ColliderType::PlayerProjectile, CollisionResponse::Block),
      std::make_pair(ColliderType::Snowball, CollisionResponse::Block));

    SetProfileResponse("PlayerProjectile",
      std::make_pair(ColliderType::Enemy, CollisionResponse::Block),
      std::make_pair(ColliderType::Snowball, CollisionResponse::Block));

    SetProfileResponse("Item", std::make_pair(ColliderType::Player, CollisionResponse::Block));

    SetProfileResponse(
      "Snowball", std::make_pair(ColliderType::PlayerProjectile, CollisionResponse::Block));

    return true;
}

void CollisionProfileManager::Destroy()
{
    for (auto& [_, profile] : _profiles)
        DESTROY(profile);

    _profiles.clear();
    _channels.clear();
}

Ptr<CollisionProfile> CollisionProfileManager::FindProfile(const std::string& name)
{
    if (auto it = _profiles.find(name); _profiles.end() != it)
        return it->second;

    return nullptr;
}

void CollisionProfileManager::CreateChannel(
  ColliderType::Type colliderType, CollisionResponse::Type defaultResponse)
{
    if (_channels.end() != _channels.find(colliderType))
        return;

    _channels[colliderType] = defaultResponse;
}

void CollisionProfileManager::CreateProfile(
  const std::string& proflieName, ColliderType::Type profileType)
{
    if (_channels.end() == _channels.find(profileType))
        return;

    if (_profiles.end() != _profiles.find(proflieName))
        return;

    Ptr<CollisionProfile> profile = New<CollisionProfile>();

    profile->_name     = proflieName;
    profile->_thisType = profileType;

    for (auto& [channelType, defaultResponse] : _channels)
        profile->_responses[channelType] = defaultResponse;

    _profiles[profile->_name] = profile;
}

void CollisionProfileManager::SetProfileResponse(
  const std::string& proflieName, CollisionChannel channel)
{
    auto& [channelType, response] = channel;
    Ptr<CollisionProfile> profile = FindProfile(proflieName);
    if (nullptr == profile)
        return;

    profile->_responses[channelType] = response;
}
