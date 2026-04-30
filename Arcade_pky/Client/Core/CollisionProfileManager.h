#pragma once
#include "SubManager.h"

#include "CollisionProfile.h"

class CollisionProfileManager : public SubManager
{
public:
    CollisionProfileManager()           = default;
    ~CollisionProfileManager() override = default;
    DELETE_SPECIAL_FUNC(CollisionProfileManager)

private:
    std::map<ColliderType::Type, CollisionResponse::Type>  _channels;
    std::unordered_map<std::string, Ptr<CollisionProfile>> _profiles;

public:
    bool Init() override;
    void Destroy() override;

    Ptr<CollisionProfile> FindProfile(const std::string& name);

    void CreateChannel(
      ColliderType::Type channel, CollisionResponse::Type response);
    void CreateProfile(
      const std::string& proflieName, ColliderType::Type channel);
    void SetProfileResponse(
      const std::string& proflieName, CollisionChannel channel);

    template <typename... Args>
    void SetProfileResponse(
      const std::string& profileName, CollisionChannel channel, Args... args)
    {
        Ptr<CollisionProfile> profile = FindProfile(profileName);
        if (nullptr == profile)
            return;

        profile->SetProfileResponse(channel, args...);
    }
};
