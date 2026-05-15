#include "pch.h"

#include "TilemapLevel.h"

#include "Tilemap/Tile.h"
#include "Tilemap/Tilemap.h"

bool TilemapLevel::Init(Ptr<World> world, const std::string& path)
{
    Level::Init(world, path);

    return true;
}

void TilemapLevel::Destroy()
{
    Level::Destroy();
    DESTROY(_tilemap);
}

Rect TilemapLevel::GetBoundary() const
{
    return _boundary;
}

void TilemapLevel::SetBoundary(Rect boundary)
{
    _boundary = boundary;
}

Ptr<class Tilemap> TilemapLevel::GetTilemap() const
{
    return _tilemap;
}
