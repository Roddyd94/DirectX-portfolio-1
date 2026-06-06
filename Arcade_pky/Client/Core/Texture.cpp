#include "pch.h"

#include "Texture.h"

#include "DeviceManager.h"
#include "DirectoryManager.h"
#include "ResourceManager.h"

#include "IndexedTextureBuffer.h"
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
    _data.resize(dataLength);
    memcpy_s(_data.data(), dataLength, data, dataLength);

    _width  = width;
    _height = height;

    _bitsPerPixel = bitsPerPixel;

    return true;
}

void IndexedTexture::SetShaderResource()
{
    auto buffer = FIND_STRUCTURE_BUFFER("IndexedTexture", IndexedTextureBuffer);

    buffer->Clear();
    buffer->AddData(reinterpret_cast<char*>(_data.data()), _data.size());
    buffer->SetSize(_width, _height);
    buffer->SetStride(_bitsPerPixel);

    buffer->Update();
    buffer->Bind();
}
