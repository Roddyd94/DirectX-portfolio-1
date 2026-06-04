#include "pch.h"

#include "SpriteInstanceRenderer.h"

#include "Core/InstanceRendererComponent.h"

bool SpriteInstanceRenderer::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    InstanceRenderer::Init(id, position, scale, rotation);

    _renderer->SetBuffer("Animation2DInstance");
    _renderer->SetShader("SpriteInstanceShader");

    return true;
}
