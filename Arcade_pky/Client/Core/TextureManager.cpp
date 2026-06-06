#include "pch.h"

#include "TextureManager.h"

#include "Texture.h"

bool TextureManager::Init()
{
    // TODO: Read Metadata and Load Textures

    // TODO: Remove this after implementing parser

    return true;
}

void TextureManager::Destroy()
{
    for (auto& [_, texture] : _textures)
        DESTROY(texture)

    _textures.clear();
    _textureFinder.clear();
}

Ptr<Texture> TextureManager::FindTexture(const std::string& name)
{
    auto itFinder = _textureFinder.find(name);
    if (_textureFinder.end() == itFinder)
        return nullptr;

    int32 id        = itFinder->second;
    auto  itTexture = _textures.find(id);
    if (_textures.end() == itTexture)
        return nullptr;

    return itTexture->second;
}

Ptr<Texture> TextureManager::LoadTexture(
  const std::string& name, const std::wstring& fileName, uint32 flags)
{
    Ptr<Texture> texture = FindTexture(name);
    if (texture)
        return texture;

    texture = New<Texture>();
    if (!texture->LoadTexture(fileName, flags))
    {
        DESTROY(texture)
        return nullptr;
    }

    texture->SetName(name);
    texture->SetID(_idCounter);
    _textures[_idCounter] = texture;
    _textureFinder[name]  = _idCounter;
    _idCounter++;

    return texture;
}
