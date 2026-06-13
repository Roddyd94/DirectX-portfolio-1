#include "pch.h"

#include "Texture.h"

#include "DeviceManager.h"
#include "DirectoryManager.h"
#include "ResourceManager.h"
#include "Common/LogManager.h"

#include "IndexedTextureInfoConstantBuffer.h"
#include "Rendering.h"
#include "Shader.h"

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex.lib")
#endif // _DEBUG

void Texture::Destroy() {}

bool Texture::LoadTexture(const std::wstring& fileName, uint32_t flags)
{
    std::filesystem::path                filePath;
    std::optional<std::filesystem::path> resPath
      = DirectoryManager::Instance().GetCachePath("Resources");

    if (!DirectoryManager::Instance().GetDirectory(resPath.value(), "Texture", filePath))
        return false;

    if (!DirectoryManager::Instance().GetFile(filePath, fileName, filePath))
        return false;

    if (DirectoryManager::Instance().CheckExtension(filePath, ".DDS"))
    {
        if (FAILED(DirectX::LoadFromDDSFile(
              filePath.wstring().c_str(), static_cast<DirectX::DDS_FLAGS>(flags), nullptr, _image)))
            return false;
    }
    else if (DirectoryManager::Instance().CheckExtension(filePath, ".TGA"))
    {
        if (FAILED(DirectX::LoadFromTGAFile(filePath.wstring().c_str(), nullptr, _image)))
            return false;
    }
    else
    {
        if (FAILED(DirectX::LoadFromWICFile(
              filePath.wstring().c_str(), static_cast<DirectX::WIC_FLAGS>(flags), nullptr, _image)))
            return false;
    }

    _width  = static_cast<uint32>(_image.GetImages()->width);
    _height = static_cast<uint32>(_image.GetImages()->height);

    return CreateShaderResourceView();
}

void Texture::SetShaderResource(int32 registerNum, uint32 shaderBufferType, uint32 textureIndex)
{
    if (shaderBufferType & ShaderType::Vertex)
        DeviceManager::Instance().GetContext()->VSSetShaderResources(
          registerNum, 1, _srv.GetAddressOf());

    if (shaderBufferType & ShaderType::Pixel)
        DeviceManager::Instance().GetContext()->PSSetShaderResources(
          registerNum, 1, _srv.GetAddressOf());
}

void Texture::ResetShaderResource(int32 registerNum, uint32 shaderBufferType)
{
    ID3D11ShaderResourceView* srv = nullptr;

    if (shaderBufferType & ShaderType::Vertex)
        DeviceManager::Instance().GetContext()->VSSetShaderResources(registerNum, 1, &srv);

    if (shaderBufferType & ShaderType::Pixel)
        DeviceManager::Instance().GetContext()->PSSetShaderResources(registerNum, 1, &srv);
}

bool Texture::CreateShaderResourceView()
{
    if (FAILED(DirectX::CreateShaderResourceView(DeviceManager::Instance().GetDevice().Get(),
          _image.GetImages(), _image.GetImageCount(), _image.GetMetadata(), _srv.GetAddressOf())))
        return false;

    return true;
}

void IndexedTexture::Destroy() {}

bool IndexedTexture::LoadTexture(
  const char* data, size_t dataLength, int32 width, int32 height, int32 bitsPerPixel)
{
    D3D11_TEXTURE2D_DESC desc;
    desc.Width     = width / (8 / bitsPerPixel);
    desc.Height    = height;
    desc.Format    = DXGI_FORMAT_R8_UINT;
    desc.MipLevels = desc.ArraySize = 1;
    desc.SampleDesc.Count           = 1;
    desc.SampleDesc.Quality         = 0;
    desc.Usage                      = D3D11_USAGE_DEFAULT;
    desc.BindFlags                  = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags             = 0;
    desc.MiscFlags                  = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem                = data;
    initData.SysMemPitch            = desc.Width;

    if (FAILED(DEVICE->CreateTexture2D(&desc, &initData, _texture.GetAddressOf())))
        return false;

    _width  = width;
    _height = height;

    _bitsPerPixel = bitsPerPixel;

    return CreateShaderResourceView();
}

void IndexedTexture::SetShaderResource(
  int32 registerNum, uint32 shaderBufferType, uint32 textureIndex)
{
    if (shaderBufferType & ShaderType::Vertex)
        DeviceManager::Instance().GetContext()->VSSetShaderResources(
          registerNum, 1, _srv.GetAddressOf());

    if (shaderBufferType & ShaderType::Pixel)
        DeviceManager::Instance().GetContext()->PSSetShaderResources(
          registerNum, 1, _srv.GetAddressOf());

    auto buffer = FIND_CONSTANT_BUFFER("IndexedTextureInfo", IndexedTextureInfoConstantBuffer);

    buffer->SetSize(_width, _height);
    buffer->SetBitsPerPixel(_bitsPerPixel);

    buffer->Update();
}

void IndexedTexture::ResetShaderResource(int32 registerNum, uint32 shaderBufferType)
{
    ID3D11ShaderResourceView* srv = nullptr;

    if (shaderBufferType & ShaderType::Vertex)
        DeviceManager::Instance().GetContext()->VSSetShaderResources(registerNum, 1, &srv);

    if (shaderBufferType & ShaderType::Pixel)
        DeviceManager::Instance().GetContext()->PSSetShaderResources(registerNum, 1, &srv);
}

bool IndexedTexture::CreateShaderResourceView()
{
    D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
    desc.Format                          = DXGI_FORMAT_R8_UINT;
    desc.ViewDimension                   = D3D11_SRV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MostDetailedMip       = 0;
    desc.Texture2D.MipLevels             = 1;

    if (FAILED(DEVICE->CreateShaderResourceView(_texture.Get(), &desc, _srv.GetAddressOf())))
        return false;

    return true;
}
