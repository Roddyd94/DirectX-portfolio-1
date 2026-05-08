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

TileType::Type Tile::GetTileType() const
{
    return _type;
}

uint32 Tile::GetSpriteIndex() const
{
    return _spriteIndex;
}

bool Tile::IsWall() const
{
    return _type & TileType::Wall;
}

bool Tile::IsPlatform() const
{
    return _type & TileType::Platform;
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

void Tile::SetTileType(TileType::Type type)
{
    _type = type;
}

void Tile::SetSprite(uint32 index)
{
    _spriteIndex = index;
}
