#pragma once
#include "Core/Rendering.h"
#include "Core/SceneComponent.h"

class TilemapComponent : public SceneComponent
{
public:
    TilemapComponent();
    ~TilemapComponent() override;

protected:
    std::vector<Ptr<class Tile>> _tiles;
    std::vector<SpriteData>      _tileSprites;

    Ptr<class Mesh>                _tileMesh;
    Ptr<class Texture>             _tileTexture;
    Ptr<class TileStructureBuffer> _tileStructureBuffer;
    Ptr<class Shader>              _tileInstanceShader;

    Ptr<class Mesh>                       _tileOutlineMesh;
    Ptr<class ColorConstantBuffer>        _tileOutlineConstantBuffer;
    Ptr<class TileOutlineStructureBuffer> _tileOutlineStructureBuffer;
    Ptr<class Shader>                     _tileOutlineInstanceShader;

    Vector2 _tileSize;
    Vector2 _mapSize;
    Vector2 _tileTextureSize;

    int32 _countX = 0;
    int32 _countY = 0;

    bool _shouldRenderOutline              = false;
    bool _shouldRefreshTileInstance        = false;
    bool _shouldRefreshTileOutlineInstance = false;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;
    void Render(float deltaTime) override;

public:
    int32 GetTileFrameCount() const { return static_cast<int32>(_tileSprites.size()); }

    Vector2 GetTileSize() const { return _tileSize; }

    int32 GetTileCountX() const { return _countX; }
    int32 GetTileCountY() const { return _countY; }

    int32 GetTileIndexX(Vector2 pos) const;
    int32 GetTileIndexY(Vector2 pos) const;
    int32 GetTileIndex(Vector2 pos) const;

    Ptr<class Tile> GetTile(Vector2 pos);
    Ptr<class Tile> GetTile(int32 index);

    std::optional<Vector2> GetTileWorldPos(int32 index);

    void CreateTile(int32 countX, int32 countY, Vector2 tileSize, int32 textureFrameIndex);

    void SetTexture(Ptr<class Texture> texture);
    void SetTexture(const std::string& name);
    void SetTexture(const std::string& name, const std::wstring& fileName);

    void AddTileSprite(Vector2 start, Vector2 size);
    void AddTileSprite(float startX, float startY, float sizeX, float sizeY);

    void RefreshTileInstance(bool refresh) { _shouldRefreshTileInstance = refresh; }
    void RefreshTileOutlineInstance(bool refresh) { _shouldRefreshTileOutlineInstance = refresh; }

private:
    void RenderTile();
    void RenderOutline();
};
