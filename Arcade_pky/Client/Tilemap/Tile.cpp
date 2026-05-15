#include "pch.h"

#include "Tile.h"

#include "TilemapComponent.h"

void Tile::Destroy() {}

Vector2 Tile::GetWorldPosition() const
{
    Vector3 tilemapPosition = Lock(_owner)->GetWorldPosition();
    return _position + Vector2{tilemapPosition.x, tilemapPosition.y};
}

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

TileType::Type Tile::GetType() const
{
    return _type;
}

uint32 Tile::GetSpriteIndex() const
{
    return _spriteIndex;
}

bool Tile::IsWall() const
{
    return _type & TileType::IsWall;
}

bool Tile::IsFloor() const
{
    return _type & TileType::IsFloor;
}

bool Tile::IsCeiling() const
{
    return _type & TileType::IsCeiling;
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
