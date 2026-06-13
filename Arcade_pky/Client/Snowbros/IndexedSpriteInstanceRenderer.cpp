#include "pch.h"

#include "IndexedSpriteInstanceRenderer.h"

#include "IndexedSpriteInstanceRendererComponent.h"

bool IndexedSpriteInstanceRenderer::Init(
  int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    InstanceRenderer::Init(id, position, scale, rotation);

    _renderer = CreateSceneComponent<IndexedSpriteInstanceRendererComponent>("Renderer");
    _renderer->SetRenderLayer("Default");

    _renderer->SetBuffer("IndexedAnimation2DInstance");
    _renderer->SetShader("IndexedSpriteInstanceShader");

    SetRoot(_renderer);

    return true;
}

void IndexedSpriteInstanceRenderer::SetTexture(Ptr<class IndexedTexture> texture)
{
    auto renderer
      = Cast<InstanceRendererComponent, IndexedSpriteInstanceRendererComponent>(_renderer);
    renderer->SetTexture(texture);
}
