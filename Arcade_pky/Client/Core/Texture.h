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

    bool LoadTexture(const std::wstring& fileName, uint32_t flags = 0);
    void SetShaderResource(int32 registerNum, uint32 shaderBufferType, uint32 textureIndex);
    void ResetShaderResource(int32 registerNum, uint32 shaderBufferType);

protected:
    bool CreateShaderResourceView();
};

class IndexedTexture : public Resource
{
public:
    IndexedTexture()           = default;
    ~IndexedTexture() override = default;

protected:
    std::vector<byte> _data;

    int32 _width        = 0;
    int32 _height       = 0;
    int32 _bitsPerPixel = 4;

public:
    void Destroy() override;

    uint32 GetWidth() const { return _width; }
    uint32 GetHeight() const { return _height; }

    bool LoadTexture(
      const char* data, size_t dataLength, int32 width, int32 height, int32 bitsPerPixel);
    void SetShaderResource();
};
