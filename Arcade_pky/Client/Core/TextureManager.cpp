#include "pch.h"

#include "TextureManager.h"

#include "Texture.h"

bool TextureManager::Init()
{
    // TODO: Read Metadata and Load Textures

    // TODO: Remove this after implementing parser
    LoadTexture("Apeach", L"apeach.png");

    return true;
}

void TextureManager::Destroy()
{
    for (auto& [_, texture] : _textures)
        DESTROY(texture)
}

Ptr<Texture> TextureManager::FindTexture(const std::string& name)
{
    if (auto it = _textures.find(name); _textures.end() != it)
        return it->second;

    return nullptr;
}

Ptr<Texture> TextureManager::LoadTexture(
  const std::string& name, const std::wstring& fileName)
{
    Ptr<Texture> texture = FindTexture(name);
    if (texture)
        return texture;

    texture = New<Texture>();
    if (!texture->LoadTexture(fileName))
    {
        DESTROY(texture)
        return nullptr;
    }

    texture->SetName(name);
    _textures[name] = texture;

    return _textures[name];
}
