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

    Vector2 tileAbovePosition = tilemap->GetWorldPosition().ToVector2();
    tileAbovePosition += _position;
    tileAbovePosition.y += _size.y;
    Ptr<Tile> tileAbove = tilemap->GetTile(tileAbovePosition);

    bool isTileAboveBlock = tileAbove && tileAbove->_type & TileType::Block;

    return _type & TileType::Block && !isTileAboveBlock;
}

bool Tile::IsBottomBlock() const
{
    Ptr<TilemapComponent> tilemap = Lock(_owner);

    Vector2 tileBelowPosition = tilemap->GetWorldPosition().ToVector2();
    tileBelowPosition += _position;
    tileBelowPosition.y -= _size.y;
    Ptr<Tile> tileBelow = tilemap->GetTile(tileBelowPosition);

    bool isTileBelowBlock = tileBelow && tileBelow->_type & TileType::Block;

    return _type & TileType::Block && !isTileBelowBlock;
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
