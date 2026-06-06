#pragma once
#include "RenderInstanceComponent.h"
#include "SceneComponent.h"
#include "StructureBuffer.h"
#include "Types.h"

class InstanceRendererComponent : public SceneComponent
{
public:
    InstanceRendererComponent()           = default;
    ~InstanceRendererComponent() override = default;

protected:
    Ptr<class Mesh>            _mesh            = nullptr;
    Ptr<class StructureBuffer> _structureBuffer = nullptr;
    Ptr<class Shader>          _shader          = nullptr;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;
    void Tick(float deltaTime) override;

    void Collision(float deltaTime) override;
    void Render(float deltaTime) override;

    void SetBuffer(Ptr<class StructureBuffer> buffer);
    void SetBuffer(const std::string& name);

    void SetShader(Ptr<class Shader> shader);
    void SetShader(const std::string& name);

public:
    template <typename T, typename... Args>
    void AddData(Args&&... args)
    {
        Ptr<T> buffer = Cast<StructureBuffer, T>(_structureBuffer);
        if (nullptr == buffer)
            return;

        buffer->AddData(std::forward<Args>(args)...);
    }
};
