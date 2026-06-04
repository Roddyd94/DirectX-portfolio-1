#include "pch.h"

#include "InstanceRendererComponent.h"

#include "ResourceManager.h"

#include "Level.h"
#include "Mesh.h"
#include "Texture.h"
#include "TransformConstantBuffer.h"

InstanceRendererComponent::InstanceRendererComponent()
{
    _shouldRender = true;
}

bool InstanceRendererComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    SceneComponent::Init(componentID, name, owner);

    _mesh = MESH_TEXTURE_RECT;

    return true;
}

void InstanceRendererComponent::Destroy()
{
    SceneComponent::Destroy();
}

void InstanceRendererComponent::Tick(float deltaTime)
{
    SceneComponent::Tick(deltaTime);
}

void InstanceRendererComponent::Collision(float deltaTime)
{
    SceneComponent::Collision(deltaTime);
}

void InstanceRendererComponent::Render(float deltaTime)
{
    SceneComponent::Render(deltaTime);

    if (!_texture)
        return;

    if (0 == _structureBuffer->GetElementCount())
        return;

    _structureBuffer->Update();

    _texture->SetShaderResource(0, ShaderType::Pixel, 0);
    SHADER_MANAGER->SetSampler(SamplerType::Point);

    auto level = GetLevel();
    auto transformBuffer = Lock(_transformConstantBuffer);

    transformBuffer->SetWorldMatrix(Matrix());
    transformBuffer->SetViewMatrix(level->GetViewMatrix());
    transformBuffer->SetProjMatrix(level->GetProjMatrix());
    transformBuffer->Update();

    _structureBuffer->Bind();
    _shader->SetShader();

    _mesh->RenderInstancing(_structureBuffer->GetElementCount());

    _structureBuffer->Clear();
}

void InstanceRendererComponent::SetTexture(Ptr<class Texture> texture)
{
    _texture = texture;
}

void InstanceRendererComponent::SetTexture(const std::string& name)
{
    SetTexture(FIND_TEXTURE(name));
}

void InstanceRendererComponent::SetBuffer(Ptr<class StructureBuffer> buffer)
{
    buffer->Clear();
    _structureBuffer = buffer;
}

void InstanceRendererComponent::SetBuffer(const std::string& name)
{
    SetBuffer(FIND_STRUCTURE_BUFFER(name, StructureBuffer));
}

void InstanceRendererComponent::SetShader(Ptr<class Shader> shader)
{
    _shader = shader;
}

void InstanceRendererComponent::SetShader(const std::string& name)
{
    SetShader(FIND_SHADER(name, Shader));
}
