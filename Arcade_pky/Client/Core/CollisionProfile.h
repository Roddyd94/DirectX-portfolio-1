#pragma once
#include "CollisionSystem.h"
#include "Object.h"

using CollisionChannel = std::pair<ColliderType::Type, CollisionResponse::Type>;

class CollisionProfile : public Object
{
    friend class CollisionProfileManager;

public:
    CollisionProfile()           = default;
    ~CollisionProfile() override = default;

private:
    std::string             _name;
    CollisionResponse::Type _responses[ColliderType::End] = {};
    ColliderType::Type      _thisType                     = ColliderType::End;

public:
    void Destroy() override;

    const std::string&            GetName() const;
    const ColliderType::Type      GetColliderType() const;
    const CollisionResponse::Type GetResponseTo(ColliderType::Type otherType);

    void SetProfileResponse(CollisionChannel channel);

    template <typename... Args>
    void SetProfileResponse(Args... channels)
    {
        (SetProfileResponse(channels), ...);
    }
};
