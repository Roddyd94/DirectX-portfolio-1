#include "pch.h"

#include "StaticMeshComponent.h"

#include "ResourceManager.h"

#include "Mesh.h"
#include "Shader.h"

StaticMeshComponent::StaticMeshComponent()
{
    _isRendering = true;
}

StaticMeshComponent::~StaticMeshComponent() {}

bool StaticMeshComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    MeshComponent::Init(componentID, name, owner);

    SetShader("StaticMeshShader");

    return true;
}

void StaticMeshComponent::Destroy()
{
    MeshComponent::Destroy();
}

void StaticMeshComponent::Tick(float deltaTime)
{
    MeshComponent::Tick(deltaTime);
}

void StaticMeshComponent::Collision(float deltaTime)
{
    MeshComponent::Collision(deltaTime);
}

void StaticMeshComponent::Render(float deltaTime)
{
    MeshComponent::Render(deltaTime);

    Ptr<Shader> shader = Lock<Shader>(_shader);
    if (shader)
        shader->SetShader();

    Ptr<Mesh> mesh = Lock<Mesh>(_mesh);
    if (mesh)
    {
        size_t slotCount = mesh->GetSlotCount();
        for (size_t i = 0; i < slotCount; ++i)
        {
            _materialSlots[i]->SetMaterial();

            mesh->Render();

            _materialSlots[i]->ResetMaterial();
        }
    }
}

void StaticMeshComponent::SetMesh(Ptr<class Mesh> mesh)
{
    _mesh = mesh;
    _materialSlots.clear();

    size_t slotCount = mesh->GetSlotCount();

    for (size_t i = 0; i < slotCount; ++i)
    {
        const MeshSlot* slot     = mesh->GetSlot(static_cast<uint32>(i));
        auto            material = slot->material;
        _materialSlots.emplace_back(material->Clone());
    }
}

void StaticMeshComponent::SetMesh(const std::string& name)
{
    Ptr<Mesh> foundMesh = MESH_MANAGER->FindMesh(name);
    if (nullptr == foundMesh)
        return;

    SetMesh(foundMesh);
}

void StaticMeshComponent::SetShader(Ptr<class Shader> shader)
{
    _shader = shader;
}

void StaticMeshComponent::SetShader(const std::string& name)
{
    SetShader(FIND_SHADER(name, Shader));
}

void StaticMeshComponent::SetMaterial(uint32 slotIndex, const std::string& name)
{
    _materialSlots[slotIndex] = MATERIAL_MANAGER->CreateMaterialInstance(name);
}

void StaticMeshComponent::AddTexture(uint32 slotIndex,
  const std::string&                        name,
  uint32                                    registerNum,
  uint32                                    shaderBufferType,
  uint32                                    textureIndex)
{
    _materialSlots[slotIndex]->AddTexture(
      name, registerNum, shaderBufferType, textureIndex);
}

void StaticMeshComponent::SetBaseColor(
  uint32 slotIndex, float r, float g, float b, float a)
{
    _materialSlots[slotIndex]->SetBaseColor(r, g, b, a);
}

void StaticMeshComponent::SetOpacity(int slotIndex, float opacity)
{
    _materialSlots[slotIndex]->SetOpacity(opacity);
}
