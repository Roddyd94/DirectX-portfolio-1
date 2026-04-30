#pragma once
#include "SceneComponent.h"

class MeshComponent : public SceneComponent
{
public:
    MeshComponent()           = default;
    ~MeshComponent() override = default;

protected:
    std::vector<Ptr<class Material>> _materialSlots;

public:
    bool Init(int32      componentID,
      const std::string& name,
      Ptr<class Actor>   owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;
    void Render(float deltaTime) override;
};
