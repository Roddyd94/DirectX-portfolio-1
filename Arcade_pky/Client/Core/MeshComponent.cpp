#include "pch.h"

#include "MeshComponent.h"

#include "Level.h"
#include "Material.h"
#include "TransformConstantBuffer.h"

bool MeshComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    SceneComponent::Init(componentID, name, owner);

    return true;
}

void MeshComponent::Destroy()
{
    SceneComponent::Destroy();

    for (auto& material : _materialSlots)
        DESTROY(material)
}

void MeshComponent::Tick(float deltaTime)
{
    SceneComponent::Tick(deltaTime);
}

void MeshComponent::Collision(float deltaTime)
{
    SceneComponent::Collision(deltaTime);
}

void MeshComponent::Render(float deltaTime)
{
    SceneComponent::Render(deltaTime);

    Ptr<Level> level = Lock<Level>(_level);
    if (nullptr == level)
        return;

    Matrix view = level->GetViewMatrix();
    Matrix proj = level->GetProjMatrix();

    auto cBuffer = Lock<TransformConstantBuffer>(_transformConstantBuffer);
    cBuffer->SetWorldMatrix(_matrix.world);
    cBuffer->SetProjMatrix(proj);
    cBuffer->SetViewMatrix(view);
    cBuffer->Update();
}
