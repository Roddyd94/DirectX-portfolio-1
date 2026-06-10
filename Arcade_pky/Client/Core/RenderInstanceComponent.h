#pragma once
#include "InstanceRendererComponent.h"
#include "SceneComponent.h"

class RenderInstanceComponent : public SceneComponent
{
public:
    RenderInstanceComponent();
    ~RenderInstanceComponent() override = default;

protected:
    Weak<class InstanceRendererComponent> _rendererComponent;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void PreRender(float deltaTime) override;

    Ptr<class InstanceRendererComponent> GetRenderer() const;

    void SetRenderer(Ptr<class InstanceRendererComponent> renderer);

    void SetRenderLayer(const std::string& name);

    virtual void AddBufferData() = 0;
};
