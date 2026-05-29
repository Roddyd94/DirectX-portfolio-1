#include "pch.h"

#include "RenderManager.h"

#include "RenderStateManager.h"

#include "Actor.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "SceneComponent.h"

bool RenderManager::Init()
{
    CreateRenderLayer("Background", 1'000);
    CreateRenderLayer("Default", 5'000);
    CreateRenderLayer("Item", 2'000);
    CreateRenderLayer("EnemyBehind", 3'000);
    CreateRenderLayer("Snowball", 3'500);
    CreateRenderLayer("Enemy", 4'000);
    CreateRenderLayer("EnemyProjectile", 5'000);
    CreateRenderLayer("PlayerProjectile", 6'000);
    CreateRenderLayer("Player", 6'000);

#ifdef _DEBUG
    CreateRenderLayer("Collider", INT_MAX);
#endif // _DEBUG

    _renderStateManager = New<RenderStateManager>();
    if (!_renderStateManager->Init())
        return false;

    _alphaBlend        = _renderStateManager->FindRenderState<BlendState>("AlphaBlend");
    _depthStencilState = _renderStateManager->FindRenderState<DepthStencilState>("DepthStencil");

    return true;
}

void RenderManager::Destroy()
{
    DESTROY(_renderStateManager)
}

void RenderManager::Tick(float deltaTime)
{
    for (auto& [layerName, actorID, componentID] : _renderersToRemove)
    {
        auto itFinder = _layerIDsByName.find(layerName);
        if (_layerIDsByName.end() == itFinder)
            continue;

        int32 layerOrder = itFinder->second;
        auto  itLayer    = _layers.find(layerOrder);
        if (_layers.end() == itLayer)
            continue;

        itLayer->second.renderersByIDPair.erase({actorID, componentID});
        itLayer->second.refreshRenders = true;
    }
    _renderersToRemove.clear();

    // 아직 헷갈리면, 주석 지우지 말 것
    // 렌더러의 레이어가 바뀌었을 때, 재배치
    if (_refreshLayer)
    {
        for (auto& [order, layer] : _layers)
        {
            // Layer에 속한 렌더러
            auto itRenderer = layer.renderersByIDPair.begin();
            while (itRenderer != layer.renderersByIDPair.end())
            {
                Ptr<SceneComponent> renderer = Lock<SceneComponent>(itRenderer->second);
                if (nullptr == renderer)
                {
                    itRenderer++;
                    continue;
                }

                Ptr<Actor> owner = renderer->GetOwner();
                if (nullptr == owner)
                {
                    itRenderer++;
                    continue;
                }

                // 렌더러가 속한 레이어?
                RenderLayer* rendererLayer = FindLayer(renderer->GetRenderLayerName());
                if (nullptr == rendererLayer)
                {
                    itRenderer++;
                    continue;
                }

                if (rendererLayer->order == order)
                {
                    itRenderer++;
                    continue;
                }

                ComponentIDPair key = {owner->GetActorID(), renderer->GetComponentID()};

                rendererLayer->renderersByIDPair[key] = renderer;
                itRenderer                            = layer.renderersByIDPair.erase(itRenderer);

                rendererLayer->refreshRenders = true;
                layer.refreshRenders          = true;
            }
        }
    }
}

void RenderManager::Render(float deltaTime)
{
    _alphaBlend->SetState();
    _depthStencilState->SetState();

    for (auto& [order, layer] : _layers)
    {
        if (layer.refreshRenders)
        {
            layer.renderers.clear();

            for (auto& [key, weak] : layer.renderersByIDPair)
                layer.renderers.push_back(weak);

            layer.refreshRenders = false;
        }

        if (_shouldSortY)
        {
            std::sort(layer.renderers.begin(), layer.renderers.end(),
              [](const Weak<SceneComponent>& lhs, const Weak<SceneComponent>& rhs)
              {
                  Ptr<SceneComponent> rendererLHS = Lock<SceneComponent>(lhs);
                  Ptr<SceneComponent> rendererRHS = Lock<SceneComponent>(rhs);

                  if (!rendererLHS || !rendererRHS)
                      return false;

                  return rendererLHS->GetWorldPosition().y < rendererRHS->GetWorldPosition().y;
              });
        }

        for (auto& weak : layer.renderers)
        {
            Ptr<SceneComponent> renderer = Lock<SceneComponent>(weak);
            if (nullptr == renderer)
                continue;

            if (!renderer->IsEnable())
                continue;

            if (!renderer->IsActive())
                continue;

            renderer->Render(deltaTime);
        }
    }

    _alphaBlend->ResetState();
    _depthStencilState->ResetState();
}

Ptr<class BlendState> RenderManager::GetAlphaBlend()
{
    return _alphaBlend;
}

Ptr<class DepthStencilState> RenderManager::GetDepthStencilState()
{
    return _depthStencilState;
}

RenderLayer* RenderManager::FindLayer(const std::string& name)
{
    auto itOrder = _layerIDsByName.find(name);
    if (_layerIDsByName.end() == itOrder)
        return nullptr;

    auto itLayer = _layers.find(itOrder->second);
    if (_layers.end() == itLayer)
        return nullptr;

    return &(itLayer->second);
}

void RenderManager::SetYSort(bool sortY)
{
    _shouldSortY = sortY;
}

void RenderManager::AddRenderComponent(int32 actorID, Ptr<SceneComponent> comp)
{
    RenderLayer* layer = FindLayer(comp->GetRenderLayerName());
    if (nullptr == layer)
        return;

    layer->renderersByIDPair[{actorID, comp->GetComponentID()}] = comp;
    layer->refreshRenders                                       = true;
}

void RenderManager::RemoveRenderComponent(
  const std::string& layerName, int32 actorID, int32 componentID)
{
    std::tuple<std::string, int32, int32> key = {layerName, actorID, componentID};
    _renderersToRemove.push_back(key);
}

void RenderManager::RefreshLayer()
{
    _refreshLayer = true;
}

void RenderManager::CreateRenderLayer(const std::string& name, int32 order)
{
    RenderLayer* foundlayer = FindLayer(name);
    if (foundlayer)
        return;

    RenderLayer layer;
    layer.name  = name;
    layer.order = order;

    _layerIDsByName[name] = order;
    _layers[order]        = layer;
}
