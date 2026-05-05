#include "pch.h"

#include "Tile.h"

void Tile::Destroy() {}

Vector2 Tile::GetPosition() const
{
    return _position;
}

Vector2 Tile::GetSize() const
{
    return _size;
}

Vector2 Tile::GetCenter() const
{
    return _center;
}

TileType Tile::GetTileType() const
{
    return _type;
}

uint32 Tile::GetTextureFrameIndex() const
{
    return _frameIndex;
}

void Tile::SetPosition(Vector2 position)
{
    _position = position;
}

void Tile::SetSize(Vector2 size)
{
    _size = size;
}

void Tile::SetCenter(Vector2 center)
{
    _center = center;
}

void Tile::SetTileType(TileType type)
{
    _type = type;
}

void Tile::SetTextureFrame(uint32 frameIndex)
{
    _frameIndex = frameIndex;
}
