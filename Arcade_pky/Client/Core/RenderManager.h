#pragma once
#include "Singleton.h"
#include "Types.h"

struct RenderLayer
{
    std::string                                     name;
    std::map<ComponentIDPair, Weak<class SceneComponent>> renderersByIDPair;
    std::vector<Weak<class SceneComponent>>               renderers;
    int32                                           order;
    bool                                            refreshRenders;
};

class RenderManager : public Singleton<RenderManager>
{
    DECLARE_SINGLETON(RenderManager)

private:
    std::unordered_map<std::string, int32>                     _layerIDsByName;
    std::vector<std::tuple<std::string, ActorID, ComponentID>> _renderersToRemove;
    std::map<int32, RenderLayer>                               _layers;

    Ptr<class RenderStateManager> _renderStateManager;
    Ptr<class BlendState>         _alphaBlend;
    Ptr<class DepthStencilState>  _depthStencilState;

    bool _refreshLayer;
    bool _shouldSortY = true;

public:
    bool Init();
    void Destroy();

    void Tick(float deltaTime);
    void Render(float deltaTime);

    Ptr<class BlendState>        GetAlphaBlend();
    Ptr<class DepthStencilState> GetDepthStencilState();

    RenderLayer* FindLayer(const std::string& name);

    void SetYSort(bool sortY);
    void AddRenderComponent(int32 actorID, Ptr<class SceneComponent> comp);

    void RemoveRenderComponent(
      const std::string& layerName, int32 actorID, int32 componentID);
    void RefreshLayer();

private:
    void CreateRenderLayer(const std::string& name, int32 order);
};
