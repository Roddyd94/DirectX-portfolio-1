#pragma once
#include "DirectXTex.h"
#include "Resource.h"

class Texture : public Resource
{
public:
    Texture()           = default;
    ~Texture() override = default;

protected:
    DirectX::ScratchImage            _image;
    ComPtr<ID3D11ShaderResourceView> _srv;
    uint32                           _width  = 0;
    uint32                           _height = 0;

public:
    void Destroy() override;

    uint32 GetWidth() const { return _width; }
    uint32 GetHeight() const { return _height; }

    bool LoadTexture(const std::wstring& fileName);
    void SetShaderResource(int32 registerNum, uint32 shaderBufferType, uint32 textureIndex);
    void ResetShaderResource(int32 registerNum, uint32 shaderBufferType);

protected:
    bool CreateShaderResourceView();
};
