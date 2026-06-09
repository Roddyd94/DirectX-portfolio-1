#include "pch.h"

#include "InstanceRendererComponent.h"

#include "ResourceManager.h"

#include "Level.h"
#include "Mesh.h"
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

    if (0 == _structureBuffer->GetElementCount())
        return;

    _structureBuffer->Update();

    auto level           = GetLevel();
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

void InstanceRendererComponent::SetMesh(Ptr<class Mesh> mesh)
{
    _mesh = mesh;
}

void InstanceRendererComponent::SetMesh(const std::string& name)
{
    SetMesh(FIND_MESH(name));
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
