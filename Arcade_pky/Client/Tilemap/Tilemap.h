#pragma once
#include "Core/Actor.h"

class Tilemap : public Actor
{
public:
    Tilemap()           = default;
    ~Tilemap() override = default;

private:
    Ptr<class TilemapComponent> _tileComponent;

public:
    bool Init(int32 id, Vector3 pos, Vector3 scale, Vector3 rot) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;

    void CreateTile(int32 countX,
      int32               countY,
      const Vector2&      tileSize,
      int32               textureFrameIndex);

    void SetTexture(Ptr<class Texture> texture);
    void SetTexture(const std::string& name);
    void SetTexture(const std::string& name, const std::wstring& fileName);

    void AddTileFrame(const Vector2& start, const Vector2& size);
    void AddTileFrame(float startX, float startY, float sizeX, float sizeY);

    // void ChangeTileTexture(const Vector2& pos);
    // void ChangeTileType(const Vector2& pos);
};
