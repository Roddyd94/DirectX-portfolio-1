#pragma once
#include "Core/Object.h"

enum class TileType
{
    Background,
    Wall,
    Platform,
    SlopeLeft,
    SlopeRight,
    End
};

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

    uint32   _frameIndex = -1;
    TileType _type       = TileType::End;

public:
    void Destroy() override;

    const Vector2& GetPosition() const;
    const Vector2& GetSize() const;
    const Vector2& GetCenter() const;

    TileType GetTileType() const;
    uint32   GetTextureFrameIndex() const;

    void SetPosition(const Vector2& position);
    void SetSize(const Vector2& size);
    void SetCenter(const Vector2& center);

    void SetTileType(TileType type);
    void SetTextureFrame(uint32 frameIndex);
};
