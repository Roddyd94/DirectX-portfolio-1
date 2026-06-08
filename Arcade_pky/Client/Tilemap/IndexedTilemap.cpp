#include "pch.h"

#include "IndexedTilemap.h"

#include "TilemapComponent.h"

bool IndexedTilemap::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Tilemap::Init(id, position, scale, rotation);

    _tileComponent = CreateSceneComponent<IndexedTextureTilemapComponent>("Tile");
    SetRoot(_tileComponent);

    return true;
}

void IndexedTilemap::SetTexture(Ptr<class IndexedTexture> texture)
{
    auto tile = Cast<TilemapComponent, IndexedTextureTilemapComponent>(_tileComponent);
    tile->SetTexture(texture);
}

void IndexedTilemap::SetTexture(const std::string& name)
{
    auto tile = Cast<TilemapComponent, IndexedTextureTilemapComponent>(_tileComponent);
    tile->SetTexture(name);
}

void IndexedTilemap::SetPaletteSet(int32 number)
{
    auto tile = Cast<TilemapComponent, IndexedTextureTilemapComponent>(_tileComponent);
    tile->SetPaletteSet(number);
    tile->RefreshTileInstance(true);
}
