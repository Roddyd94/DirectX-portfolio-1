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

Rect Tile::GetRect() const
{
    Vector2 worldPosition = GetWorldPosition();
    Vector2 leftBottom    = worldPosition - _size * 0.5f;
    Vector2 rightTop      = worldPosition + _size * 0.5f;

    return {leftBottom, rightTop};
}

TileType::Type Tile::GetType() const
{
    return _type;
}

uint32 Tile::GetSpriteIndex() const
{
    return _spriteIndex;
}

bool Tile::IsBackground() const
{
    return _type & TileType::Background;
}

bool Tile::IsBlock() const
{
    return _type & TileType::Block;
}

bool Tile::IsTopBlock() const
{
    Ptr<TilemapComponent> tilemap = Lock(_owner);

    Vector2 tileAbovePosition = _position;
    tileAbovePosition.y += _size.y;
    Ptr<Tile> tileAbove = tilemap->GetTileLocal(tileAbovePosition);

    bool isTileAboveBlock = tileAbove && tileAbove->_type & TileType::Block;

    return _type & TileType::Block && !isTileAboveBlock;
}

bool Tile::IsBottomBlock() const
{
    Ptr<TilemapComponent> tilemap = Lock(_owner);

    Vector2 tileBelowPosition = _position;
    tileBelowPosition.y -= _size.y;
    Ptr<Tile> tileBelow = tilemap->GetTileLocal(tileBelowPosition);

    bool isTileBelowBlock = tileBelow && tileBelow->_type & TileType::Block;

    return _type & TileType::Block && !isTileBelowBlock;
}

bool Tile::IsLeftmostBlock() const
{
    Ptr<TilemapComponent> tilemap = Lock(_owner);

    int32 indexX = tilemap->GetTileIndexXLocal(_position.x);
    return 0 == indexX;
}

bool Tile::IsRightmostBlock() const
{
    Ptr<TilemapComponent> tilemap = Lock(_owner);

    int32 indexX = tilemap->GetTileIndexXLocal(_position.x);
    return (tilemap->GetTileCountX() - 1) == indexX;
}

bool Tile::IsFirstFloor() const
{
    Ptr<TilemapComponent> tilemap = Lock(_owner);

    int32 indexY = tilemap->GetTileIndexYLocal(_position.y);
    return false;
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
