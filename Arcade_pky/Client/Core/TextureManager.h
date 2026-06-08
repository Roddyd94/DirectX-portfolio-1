#pragma once
#include "SubManager.h"

class TextureManager : public SubManager
{
public:
    TextureManager()           = default;
    ~TextureManager() override = default;
    DELETE_SPECIAL_FUNC(TextureManager)

private:
    std::unordered_map<std::string, int32>     _textureFinder;
    std::map<int32, Ptr<class Texture>>        _textures;
    std::map<int32, Ptr<class IndexedTexture>> _indexedTextures;

    int32 _idCounter = 0;

public:
    bool Init() override;
    void Destroy() override;

    Ptr<class Texture>        FindTexture(const std::string& name);
    Ptr<class IndexedTexture> FindIndexedTexture(const std::string& name);

    Ptr<class Texture> LoadTexture(
      const std::string& name, const std::wstring& fileName, uint32 flags = 0);

    Ptr<class IndexedTexture> AddIndexedTexture(
      const std::string& name, Ptr<IndexedTexture> texture);
};
