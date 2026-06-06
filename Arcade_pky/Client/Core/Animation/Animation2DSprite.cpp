#include "pch.h"

#include "Animation2DSprite.h"

#include "Core/ResourceManager.h"

#include "Core/Texture.h"

void Animation2DSpriteSheet::Destroy() {}

const SpriteData& Animation2DSpriteSheet::GetSpriteData(int32 index) const
{
    if (index < 0)
        return _sprites[0];
    if (index >= _sprites.size())
        return _sprites[_sprites.size() - 1];

    return _sprites[index];
}

int32 Animation2DSpriteSheet::GetSpriteCount() const
{
    return _sprites.size();
}

void Animation2DSpriteSheet::AddSpriteData(Vector2 start, Vector2 size)
{
    AddSpriteData(start.x, start.y, size.x, size.y);
}

void Animation2DSpriteSheet::AddSpriteData(float startX, float startY, float sizeX, float sizeY)
{
    SpriteData sprite;
    sprite.start = {startX, startY};
    sprite.size  = {sizeX, sizeY};

    _sprites.emplace_back(sprite);
}

bool TextureAnimation2DSpriteSheet::Init(Ptr<class Texture> texture)
{
    _texture = texture;

    return true;
}

Ptr<class Texture> TextureAnimation2DSpriteSheet::GetTexture() const
{
    return Lock(_texture);
}

void TextureAnimation2DSpriteSheet::SetTexture(Ptr<class Texture> texture)
{
    _texture = texture;
}

void TextureAnimation2DSpriteSheet::SetTexture(const std::string& name)
{
    Ptr<Texture> texture = TEXTURE_MANAGER->FindTexture(name);
    SetTexture(texture);
}

void TextureAnimation2DSpriteSheet::SetTexture(
  const std::string& name, const std::wstring& fileName)
{
    Ptr<Texture> texture = TEXTURE_MANAGER->LoadTexture(name, fileName);
    SetTexture(texture);
}

bool IndexedTextureAnimation2DSpriteSheet::Init(Ptr<class IndexedTexture> texture)
{
    _texture = texture;

    return true;
}

Ptr<class IndexedTexture> IndexedTextureAnimation2DSpriteSheet::GetTexture() const
{
    return _texture;
}

void IndexedTextureAnimation2DSpriteSheet::SetTexture(Ptr<class IndexedTexture> texture)
{
    _texture = texture;
}
