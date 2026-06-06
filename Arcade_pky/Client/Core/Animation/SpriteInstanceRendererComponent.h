#pragma once
#include "Core/InstanceRendererComponent.h"

class SpriteInstanceRendererComponent : public InstanceRendererComponent
{
public:
    SpriteInstanceRendererComponent();
    ~SpriteInstanceRendererComponent() override = default;

protected:
    Ptr<class Texture> _texture = nullptr;

public:
    void Render(float deltaTime) override;

    void SetTexture(Ptr<class Texture> texture);
    void SetTexture(const std::string& name);
};
