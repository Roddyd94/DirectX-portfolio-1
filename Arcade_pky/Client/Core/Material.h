#pragma once
#include "Resource.h"

struct MaterialTextureInfo
{
    Weak<class Texture> texture;

    uint32 registerNum;
    uint32 shaderBufferType;
    uint32 textureIndex;
};

class Material : public Resource
{
    friend class MaterialManager;

public:
    Material()          = default;
    virtual ~Material() = default;

protected:
    std::vector<MaterialTextureInfo>   _textureInfos;
    Weak<class Shader>                 _pixelShader;
    Weak<class MaterialConstantBuffer> _constantBuffer;
    Color                              _baseColor   = Color(1.f, 1.f, 1.f, 1.f);
    uint8                              _samplerType = SamplerType::Point;

public:
    void Destroy() override;

    void SetBaseColor(float r, float g, float b, float a);
    void SetBaseColor(Color color);
    void SetOpacity(float opacity);
    void SetSamplerType(uint8 type);

    void AddTexture(Ptr<class Texture> texture,
      uint32                           registerNum,
      uint32                           shaderBufferType = ShaderType::Pixel,
      uint32                           textureIndex     = 0);
    void AddTexture(const std::string& name,
      uint32                           registerNum,
      uint32                           shaderBufferType = ShaderType::Pixel,
      uint32                           textureIndex     = 0);
    void SetPixelShader(const std::string& name);
    void SetMaterial();
    void ResetMaterial();

    Ptr<Material> Clone();
};
