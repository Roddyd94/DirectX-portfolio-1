#include "pch.h"

#include "Texture.h"

#include "DeviceManager.h"
#include "DirectoryManager.h"

#include "Rendering.h"
#include "Shader.h"

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex.lib")
#endif // _DEBUG

void Texture::Destroy() {}

const TextureInfo* Texture::GetTexture(uint32 index) const
{
    if (_textureInfos.size() <= index)
        return nullptr;

    return &_textureInfos[index];
}

bool Texture::LoadTexture(const std::wstring& fileName)
{
    std::filesystem::path                filePath;
    std::optional<std::filesystem::path> resPath
      = DirectoryManager::Instance().GetCachePath("Resources");

    if (!DirectoryManager::Instance().GetDirectory(
          resPath.value(), "Texture", filePath))
        return false;

    if (!DirectoryManager::Instance().GetFile(filePath, fileName, filePath))
        return false;

    TextureInfo texture;
    if (DirectoryManager::Instance().CheckExtension(filePath, ".DDS"))
    {
        if (FAILED(DirectX::LoadFromDDSFile(filePath.wstring().c_str(),
              DirectX::DDS_FLAGS_NONE, nullptr, texture.image)))
            return false;
    }
    else if (DirectoryManager::Instance().CheckExtension(filePath, ".TGA"))
    {
        if (FAILED(DirectX::LoadFromTGAFile(
              filePath.wstring().c_str(), nullptr, texture.image)))
            return false;
    }
    else
    {
        if (FAILED(DirectX::LoadFromWICFile(filePath.wstring().c_str(),
              DirectX::WIC_FLAGS_NONE, nullptr, texture.image)))
            return false;
    }

    size_t srcIndex = _textureInfos.size();

    texture.fileName = fileName;
    texture.width    = static_cast<uint32>(texture.image.GetImages()->width);
    texture.height   = static_cast<uint32>(texture.image.GetImages()->height);

    _textureInfos.emplace_back(std::move(texture));

    return CreateShaderResourceView(static_cast<uint32>(srcIndex));
}

void Texture::SetShader(
  uint32 registerNum, uint32 shaderBufferType, uint32 textureIndex)
{
    if (shaderBufferType & ShaderType::Vertex)
        DeviceManager::Instance().GetContext()->VSSetShaderResources(
          registerNum, 1, _textureInfos[textureIndex].srv.GetAddressOf());

    if (shaderBufferType & ShaderType::Pixel)
        DeviceManager::Instance().GetContext()->PSSetShaderResources(
          registerNum, 1, _textureInfos[textureIndex].srv.GetAddressOf());
}

void Texture::ResetShader(uint32 registerNum, uint32 shaderBufferType)
{
    ID3D11ShaderResourceView* srv = nullptr;

    if (shaderBufferType & ShaderType::Vertex)
        DeviceManager::Instance().GetContext()->VSSetShaderResources(
          registerNum, 1, &srv);

    if (shaderBufferType & ShaderType::Pixel)
        DeviceManager::Instance().GetContext()->PSSetShaderResources(
          registerNum, 1, &srv);
}

bool Texture::CreateShaderResourceView(uint32 textureIndex)
{
    if (FAILED(DirectX::CreateShaderResourceView(
          DeviceManager::Instance().GetDevice().Get(),
          _textureInfos[textureIndex].image.GetImages(),
          _textureInfos[textureIndex].image.GetImageCount(),
          _textureInfos[textureIndex].image.GetMetadata(),
          _textureInfos[textureIndex].srv.GetAddressOf())))
        return false;

    return true;
}
