#include "pch.h"

#include "SpriteInstanceRendererComponent.h"

#include "Core/ResourceManager.h"

#include "Core/Texture.h"

SpriteInstanceRendererComponent::SpriteInstanceRendererComponent()
{
    _shouldRender = true;
}

void SpriteInstanceRendererComponent::Render(float deltaTime)
{
    if (!_texture)
        return;

    _texture->SetShaderResource(0, ShaderType::Pixel, 0);
    SHADER_MANAGER->SetSampler(SamplerType::Point);

    InstanceRendererComponent::Render(deltaTime);
}

void SpriteInstanceRendererComponent::SetTexture(Ptr<class Texture> texture)
{
    _texture = texture;
}

void SpriteInstanceRendererComponent::SetTexture(const std::string& name)
{
    SetTexture(FIND_TEXTURE(name));
}
