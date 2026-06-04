#include "pch.h"

#include "InstanceRenderer.h"

#include "InstanceRendererComponent.h"

bool InstanceRenderer::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Actor::Init(id, position, scale, rotation);

    _renderer = CreateSceneComponent<InstanceRendererComponent>("Renderer");
    _renderer->SetRenderLayer("Default");

    SetRoot(_renderer);
    SetName("InstanceRenderer");

    return true;
}

void InstanceRenderer::Destroy()
{
    Actor::Destroy();
    DESTROY(_renderer);
}

Ptr<class InstanceRendererComponent> InstanceRenderer::GetRendererComponent() const
{
    return _renderer;
}

void InstanceRenderer::SetRendererComponent(Ptr<class InstanceRendererComponent> renderer)
{
    _renderer = renderer;
}

void InstanceRenderer::SetTexture(Ptr<class Texture> texture)
{
    _renderer->SetTexture(texture);
}

void InstanceRenderer::SetTexture(const std::string& name)
{
    _renderer->SetTexture(name);
}

void InstanceRenderer::SetBuffer(Ptr<class StructureBuffer> buffer)
{
    _renderer->SetBuffer(buffer);
}

void InstanceRenderer::SetBuffer(const std::string& name)
{
    _renderer->SetBuffer(name);
}

void InstanceRenderer::SetShader(Ptr<class Shader> shader)
{
    _renderer->SetShader(shader);
}

void InstanceRenderer::SetShader(const std::string& name)
{
    _renderer->SetShader(name);
}

void InstanceRenderer::SetRenderLayer(const std::string& name)
{
    _renderer->SetRenderLayer(name);
}
