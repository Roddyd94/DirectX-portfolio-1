#pragma once
#include "Core/Rendering.h"
#include "Core/Resource.h"

class Animation2DSpriteSheet : public Resource
{
public:
    Animation2DSpriteSheet()           = default;
    ~Animation2DSpriteSheet() override = default;

protected:
    std::vector<SpriteData> _sprites;

public:
    void Destroy() override;

    const SpriteData& GetSpriteData(int32 index) const;

    int32 GetSpriteCount() const;

    void AddSpriteData(Vector2 start, Vector2 size);
    void AddSpriteData(float startX, float startY, float sizeX, float sizeY);
};

class TextureAnimation2DSpriteSheet : public Animation2DSpriteSheet
{
public:
    TextureAnimation2DSpriteSheet()           = default;
    ~TextureAnimation2DSpriteSheet() override = default;

protected:
    Weak<class Texture> _texture;

public:
    bool Init(Ptr<class Texture> texture);

    Ptr<class Texture> GetTexture() const;

    void SetTexture(Ptr<class Texture> texture);
    void SetTexture(const std::string& name);
    void SetTexture(const std::string& name, const std::wstring& fileName);
};

class IndexedTextureAnimation2DSpriteSheet : public Animation2DSpriteSheet
{
public:
    IndexedTextureAnimation2DSpriteSheet()           = default;
    ~IndexedTextureAnimation2DSpriteSheet() override = default;

protected:
    Ptr<class IndexedTexture> _texture;

public:
    bool Init(Ptr<class IndexedTexture> texture);

    Ptr<class IndexedTexture> GetTexture() const;

    void SetTexture(Ptr<class IndexedTexture> texture);
};

struct Animation2DSprite
{
    Ptr<Animation2DSpriteSheet> spriteSheet;
    int32                       spriteIndex;
};
