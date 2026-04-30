#pragma once
#include "MeshComponent.h"
#include "Rendering.h"

class StaticMeshComponent : public MeshComponent
{
public:
    StaticMeshComponent();
    ~StaticMeshComponent() override;

protected:
    Weak<class Mesh>   _mesh;
    Weak<class Shader> _shader;

public:
    bool Init(int32      componentID,
      const std::string& name,
      Ptr<class Actor>   owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;
    void Render(float deltaTime) override;

    void SetMesh(Ptr<class Mesh> mesh);
    void SetMesh(const std::string& name);
    void SetShader(Ptr<class Shader> shader);
    void SetShader(const std::string& name);
    void SetMaterial(uint32 slotIndex, const std::string& name);
    void AddTexture(uint32 slotIndex,
      const std::string&   name,
      uint32               registerNum,
      uint32               shaderBufferType = ShaderType::Pixel,
      uint32               textureIndex     = 0);
    void SetBaseColor(uint32 slotIndex, float r, float g, float b, float a);
    void SetOpacity(int slotIndex, float opacity);
};
