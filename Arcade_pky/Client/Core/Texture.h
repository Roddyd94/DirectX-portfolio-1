#pragma once
#include "DirectXTex.h"
#include "Resource.h"

struct TextureInfo
{
    DirectX::ScratchImage            image;
    std::wstring                     fileName;
    ComPtr<ID3D11ShaderResourceView> srv;
    uint32                           width  = 0;
    uint32                           height = 0;
};

class Texture : public Resource
{
public:
    Texture()           = default;
    ~Texture() override = default;

protected:
    std::vector<TextureInfo> _textureInfos;

public:
    void Destroy() override;

    const TextureInfo* GetTexture(uint32 index) const;

    bool LoadTexture(const std::wstring& fileName);
    void SetShader(
      int32 registerNum, uint32 shaderBufferType, uint32 textureIndex);
    void ResetShader(int32 registerNum, uint32 shaderBufferType);

protected:
    bool CreateShaderResourceView(uint32 textureIndex);
};
