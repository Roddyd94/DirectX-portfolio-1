#pragma once
#include "Core/Object.h"

namespace TileType
{
    enum Type : uint16
    {
        Background,
        Ceiling = 0,
        Platform,
        Wall,
        Floor = Platform | Wall,
        SlopeLeft,
        SlopeRight = 8,
        End
    };
}

class Tile : public Object
{
    friend class TilemapComponent;

public:
    Tile()           = default;
    ~Tile() override = default;

private:
    Vector2 _position;
    Vector2 _size;
    Vector2 _center;

    uint32         _spriteIndex = -1;
    TileType::Type _type        = TileType::Background;

public:
    void Destroy() override;

    Vector2 GetPosition() const;
    Vector2 GetSize() const;
    Vector2 GetCenter() const;

    TileType::Type GetTileType() const;
    uint32         GetSpriteIndex() const;

    bool IsWall() const;
    bool IsPlatform() const;

    void SetPosition(Vector2 position);
    void SetSize(Vector2 size);
    void SetCenter(Vector2 center);

    void SetTileType(TileType::Type type);
    void SetSprite(uint32 index);
};
