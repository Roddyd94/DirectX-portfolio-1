#include "pch.h"

#include "RenderInstanceComponent.h"

#include "RenderManager.h"

#include "Actor.h"
#include "InstanceRenderer.h"
#include "Level.h"

RenderInstanceComponent::RenderInstanceComponent()
{
    _shouldRender = true;
}

bool RenderInstanceComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    SceneComponent::Init(componentID, name, owner);

    return true;
}

void RenderInstanceComponent::Destroy()
{
    SceneComponent::Destroy();
}

void RenderInstanceComponent::PreRender(float deltaTime)
{
    SceneComponent::PreRender(deltaTime);

    AddBufferData();
}

Ptr<class InstanceRendererComponent> RenderInstanceComponent::GetRenderer() const
{
    return Lock(_rendererComponent);
}

void RenderInstanceComponent::SetRenderer(Ptr<class InstanceRendererComponent> renderer)
{
    _rendererComponent = renderer;
}

void RenderInstanceComponent::SetRenderLayer(const std::string& name)
{
    _renderLayerName = name;

    auto level = GetLevel();
    if (nullptr == level)
        return;

    auto renderer = level->FindInstanceRenderer(name);
    if (nullptr == renderer)
        return;

    _rendererComponent = renderer->GetRendererComponent();

    int32 actorID = GetOwner()->GetActorID();
    RenderManager::Instance().AddRenderComponent(actorID, This<SceneComponent>());
    RenderManager::Instance().RefreshLayer();
}
