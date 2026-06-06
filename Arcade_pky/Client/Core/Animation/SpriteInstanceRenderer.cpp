#include "pch.h"

#include "SpriteInstanceRenderer.h"

#include "Core/ResourceManager.h"

#include "SpriteInstanceRendererComponent.h"

bool SpriteInstanceRenderer::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    InstanceRenderer::Init(id, position, scale, rotation);

    _renderer = CreateSceneComponent<SpriteInstanceRendererComponent>("Renderer");
    _renderer->SetRenderLayer("Default");

    _renderer->SetBuffer("Animation2DInstance");
    _renderer->SetShader("SpriteInstanceShader");

    return true;
}

void SpriteInstanceRenderer::SetTexture(Ptr<class Texture> texture)
{
    auto renderer = Cast<InstanceRendererComponent, SpriteInstanceRendererComponent>(_renderer);
    renderer->SetTexture(texture);
}

void SpriteInstanceRenderer::SetTexture(const std::string& name)
{
    SetTexture(FIND_TEXTURE(name));
}
