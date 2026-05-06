#pragma once
#include "Component.h"

class ActorComponent : public Component
{
public:
    ActorComponent()           = default;
    ~ActorComponent() override = default;

public:
    bool Init(
      int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
};
