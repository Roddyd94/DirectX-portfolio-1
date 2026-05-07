#pragma once
#include "Core/Rendering.h"
#include "Core/Resource.h"

class Animation2DSpriteSheet : public Resource
{
public:
    Animation2DSpriteSheet()           = default;
    ~Animation2DSpriteSheet() override = default;

private:
    std::vector<SpriteData> _sprites;
    Ptr<class Texture>      _texture;

public:
    bool Init(Ptr<class Texture> texture);
    void Destroy() override;

    Ptr<class Texture> GetTexture() const;
    const SpriteData&  GetSpriteData(int32 index) const;

    int32 GetSpriteCount() const;

    void SetTexture(Ptr<class Texture> texture);
    void SetTexture(const std::string& name);
    void SetTexture(const std::string& name, const std::wstring& fileName);

    void AddSpriteData(Vector2 start, Vector2 size);
    void AddSpriteData(float startX, float startY, float sizeX, float sizeY);
};

struct Animation2DSprite
{
    Ptr<Animation2DSpriteSheet> spriteSheet;
    int32                       spriteIndex;
};
