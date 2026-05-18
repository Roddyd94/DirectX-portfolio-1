#pragma once
#include "Core/Object.h"

namespace TileType
{
    enum Type : uint16
    {
        Background,
        Block,
        TriangleLeftTop,
        TriangleLeftBottom,
        TriangleRightTop,
        TriangleRightBottom,
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
    Weak<class TilemapComponent> _owner;
    Vector2                      _position;
    Vector2                      _size;
    Vector2                      _center;

    uint32         _spriteIndex = -1;
    TileType::Type _type        = TileType::Background;

public:
    void Destroy() override;

    Vector2 GetWorldPosition() const;
    Vector2 GetPosition() const;
    Vector2 GetSize() const;
    Vector2 GetCenter() const;

    TileType::Type GetType() const;
    uint32         GetSpriteIndex() const;

    bool IsBackground() const;
    bool IsBlock() const;
    bool IsTopBlock() const;
    bool IsBottomBlock() const;

    void SetPosition(Vector2 position);
    void SetSize(Vector2 size);
    void SetCenter(Vector2 center);

    void SetTileType(TileType::Type type);
    void SetSprite(uint32 index);
};
