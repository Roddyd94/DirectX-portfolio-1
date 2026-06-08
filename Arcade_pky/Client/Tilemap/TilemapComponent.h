#pragma once
#include "Core/Rendering.h"
#include "Core/SceneComponent.h"

class TilemapComponent : public SceneComponent
{
public:
    TilemapComponent()           = default;
    ~TilemapComponent() override = default;

protected:
    std::vector<Ptr<class Tile>> _tiles;
    std::vector<SpriteData>      _tileSprites;

    Ptr<class Mesh>            _tileMesh;
    Ptr<class StructureBuffer> _tileStructureBuffer;
    Ptr<class Shader>          _tileInstanceShader;

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

    virtual Ptr<class Tile> CreateTile();

public:
    int32 GetTileFrameCount() const { return static_cast<int32>(_tileSprites.size()); }

    Vector2 GetTileSize() const { return _tileSize; }

    int32 GetTileCountX() const { return _countX; }
    int32 GetTileCountY() const { return _countY; }

    int32 GetTileIndexX(float worldPositionX) const;
    int32 GetTileIndexY(float worldPositionY) const;
    int32 GetTileIndex(Vector2 worldPosition) const;

    int32 GetTileIndexXLocal(float localPositionX) const;
    int32 GetTileIndexYLocal(float localPositionY) const;
    int32 GetTileIndexLocal(Vector2 localPosition) const;

    Ptr<class Tile> GetTile(int32 index);
    Ptr<class Tile> GetTile(Vector2 worldPosition);
    Ptr<class Tile> GetTileLocal(Vector2 localPosition);

    std::optional<Vector2> GetTileWorldPos(int32 index);

    void CreateTile(int32 countX, int32 countY, Vector2 tileSize, int32 textureFrameIndex);

    void AddTileSprite(Vector2 start, Vector2 size);
    void AddTileSprite(float startX, float startY, float sizeX, float sizeY);

    void RefreshTileInstance(bool refresh) { _shouldRefreshTileInstance = refresh; }
    void RefreshTileOutlineInstance(bool refresh) { _shouldRefreshTileOutlineInstance = refresh; }

protected:
    virtual void AddBufferData(int32 tileIndex) = 0;

    virtual void RenderTile();
    virtual void RenderOutline();
};

class TextureTilemapComponent : public TilemapComponent
{
public:
    TextureTilemapComponent();
    ~TextureTilemapComponent() override;

protected:
    Ptr<class Texture> _tileTexture;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;

    void SetTexture(Ptr<class Texture> texture);
    void SetTexture(const std::string& name);
    void SetTexture(const std::string& name, const std::wstring& fileName);

protected:
    void AddBufferData(int32 tileIndex) override;
    void RenderTile() override;
};

class IndexedTextureTilemapComponent : public TilemapComponent
{
public:
    IndexedTextureTilemapComponent();
    ~IndexedTextureTilemapComponent() override;

protected:
    Ptr<class StructureBuffer> _paletteBuffer = nullptr;
    Ptr<class IndexedTexture>  _tileTexture   = nullptr;

    int32 _paletteSetNumber = 0;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;

    Ptr<class Tile> CreateTile() override;

    void SetTexture(Ptr<class IndexedTexture> texture);
    void SetTexture(const std::string& name);

    void SetPaletteSet(int32 number);

protected:
    void AddBufferData(int32 tileIndex) override;
    void RenderTile() override;
};
