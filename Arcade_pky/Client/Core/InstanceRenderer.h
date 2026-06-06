#pragma once
#include "Actor.h"

class InstanceRenderer : public Actor
{
public:
    InstanceRenderer()           = default;
    ~InstanceRenderer() override = default;

protected:
    Ptr<class InstanceRendererComponent> _renderer = nullptr;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void Destroy() override;

    Ptr<class InstanceRendererComponent> GetRendererComponent() const;
    void SetRendererComponent(Ptr<class InstanceRendererComponent> renderer);

    void SetBuffer(Ptr<class StructureBuffer> buffer);
    void SetBuffer(const std::string& name);

    void SetShader(Ptr<class Shader> shader);
    void SetShader(const std::string& name);

    void SetRenderLayer(const std::string& name);
};
