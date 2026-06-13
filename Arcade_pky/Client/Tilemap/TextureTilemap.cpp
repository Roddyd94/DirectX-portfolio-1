#include "pch.h"

#include "TextureTilemap.h"

#include "TilemapComponent.h"
#include "Core/Texture.h"

bool TextureTilemap::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Tilemap::Init(id, position, scale, rotation);

    _tileComponent = CreateSceneComponent<TextureTilemapComponent>("Tile");
    _tileComponent->AttachToComponent(_root);

    return true;
}

void TextureTilemap::SetTexture(Ptr<Texture> texture)
{
    auto tile = Cast<TilemapComponent, TextureTilemapComponent>(_tileComponent);
    tile->SetTexture(texture);
}

void TextureTilemap::SetTexture(const std::string& name)
{
    auto tile = Cast<TilemapComponent, TextureTilemapComponent>(_tileComponent);
    tile->SetTexture(name);
}

void TextureTilemap::SetTexture(const std::string& name, const std::wstring& fileName)
{
    auto tile = Cast<TilemapComponent, TextureTilemapComponent>(_tileComponent);
    tile->SetTexture(name, fileName);
}
