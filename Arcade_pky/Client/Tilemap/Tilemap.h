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
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;

    void CreateTile(int32 countX, int32 countY, Vector2 tileSize, int32 textureFrameIndex);

    void SetTexture(Ptr<class Texture> texture);
    void SetTexture(const std::string& name);
    void SetTexture(const std::string& name, const std::wstring& fileName);

    void AddTileSprite(Vector2 start, Vector2 size);
    void AddTileSprite(float startX, float startY, float sizeX, float sizeY);

    Ptr<class Tile> GetTile(int32 index);
    Ptr<class Tile> GetTile(Vector2 worldPosition);
    Ptr<class Tile> GetTileLocal(Vector2 localPosition);

    // void ChangeTileTexture(Vector2 pos);
    // void ChangeTileType(Vector2 pos);
};
