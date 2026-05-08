#include "pch.h"

#include "TilemapComponent.h"

#include "Core/ResourceManager.h"

#include "Tile.h"
#include "TileInstanceShader.h"
#include "TileOutlineInstanceShader.h"
#include "TileOutlineStructureBuffer.h"
#include "TileStructureBuffer.h"
#include "Core/Actor.h"
#include "Core/ColorConstantBuffer.h"
#include "Core/Level.h"
#include "Core/Mesh.h"
#include "Core/Texture.h"
#include "Core/TransformConstantBuffer.h"

TilemapComponent::TilemapComponent()
{
    _isRendering = true;
}

TilemapComponent::~TilemapComponent() {}

bool TilemapComponent::Init(int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    SceneComponent::Init(componentID, name, owner);

    _tileMesh            = MESH_TEXTURE_RECT;
    _tileStructureBuffer = FIND_STRUCTURE_BUFFER("Tile", TileStructureBuffer);
    _tileInstanceShader  = FIND_SHADER("TileInstanceShader", TileInstanceShader);

#ifdef _DEBUG
    _shouldRenderOutline        = true;
    _tileOutlineMesh            = MESH_LINE_RECT;
    _tileOutlineConstantBuffer  = CONSTANT_BUFFER_COLOR;
    _tileOutlineStructureBuffer = FIND_STRUCTURE_BUFFER("TileOutline", TileOutlineStructureBuffer);
    _tileOutlineInstanceShader
      = FIND_SHADER("TileOutlineInstanceShader", TileOutlineInstanceShader);
#endif // _DEBUG

    SetRenderLayer("Background");

    return true;
}

void TilemapComponent::Destroy()
{
    SceneComponent::Destroy();

    for (auto& tile : _tiles)
        DESTROY(tile);

    _tiles.clear();
}

void TilemapComponent::Tick(float deltaTime)
{
    SceneComponent::Tick(deltaTime);
}

void TilemapComponent::Collision(float deltaTime)
{
    SceneComponent::Collision(deltaTime);
}

void TilemapComponent::Render(float deltaTime)
{
    SceneComponent::Render(deltaTime);

    RenderTile();
    RenderOutline();
}

int32 TilemapComponent::GetTileIndexX(Vector2 pos) const
{
    Ptr<Actor> owner = GetOwner();
    if (nullptr == owner)
        return -1;

    float startX = owner->GetWorldPosition().x - (_tileSize.x * 0.5f);

    float localX = pos.x - startX;

    if (localX < 0.f)
        return -1;

    int32 indexX = static_cast<int32>(localX / _tileSize.x);
    if (indexX >= _countX)
        return -1;

    return indexX;
}

int32 TilemapComponent::GetTileIndexY(Vector2 pos) const
{
    Ptr<Actor> owner = GetOwner();
    if (nullptr == owner)
        return -1;

    float startY = owner->GetWorldPosition().y - (_tileSize.y * 0.5f);

    float localY = pos.y - startY;

    if (localY < 0.f)
        return -1;

    int32 indexY = static_cast<int32>(localY / _tileSize.y);
    if (indexY >= _countY)
        return -1;

    return indexY;
}

int32 TilemapComponent::GetTileIndex(Vector2 pos) const
{
    int32 indexX = GetTileIndexX(pos);
    int32 indexY = GetTileIndexY(pos);

    if (indexX == -1 || indexY == -1)
        return -1;

    return indexY * _countX + indexX;
}

Ptr<class Tile> TilemapComponent::GetTile(Vector2 pos)
{
    int32 index = GetTileIndex(pos);
    return GetTile(index);
}

Ptr<class Tile> TilemapComponent::GetTile(int32 index)
{
    if (index == -1)
        return nullptr;

    return _tiles[index];
}

std::optional<Vector2> TilemapComponent::GetTileWorldPos(int32 index)
{
    Ptr<Actor> owner = GetOwner();
    if (!owner)
        return std::nullopt;

    Vector2 ownerPos;
    ownerPos.x = owner->GetWorldPosition().x;
    ownerPos.y = owner->GetWorldPosition().y;

    return _tiles[index]->GetPosition() + ownerPos;
}

void TilemapComponent::CreateTile(
  int32 countX, int32 countY, Vector2 tileSize, int32 textureFrameIndex)
{
    _countX = countX;
    _countY = countY;

    _tileSize = tileSize;

    _mapSize.x = _countX * _tileSize.x;
    _mapSize.y = _countY * _tileSize.y;

    _tiles.resize(countX * countY);

    for (int32 i = 0; i < _countY; ++i)
    {
        for (int32 j = 0; j < _countX; ++j)
        {
            Ptr<Tile> tile = New<Tile>();
            tile->_type    = TileType::Background;

            tile->_position.x = j * _tileSize.x;
            tile->_position.y = i * _tileSize.y;

            tile->_size        = _tileSize;
            tile->_center      = tile->_position + tileSize * 0.5f;
            tile->_spriteIndex = textureFrameIndex;

            _tiles[i * _countX + j] = tile;
        }
    }

    RefreshTileInstance(true);
    RefreshTileOutlineInstance(true);
}

void TilemapComponent::SetTexture(Ptr<class Texture> texture)
{
    _tileTexture = texture;

    _tileTextureSize.x = static_cast<float>(_tileTexture->GetWidth());
    _tileTextureSize.y = static_cast<float>(_tileTexture->GetHeight());

    RefreshTileInstance(true);
    RefreshTileOutlineInstance(true);
}

void TilemapComponent::SetTexture(const std::string& name)
{
    Ptr<Texture> foundTexture = TEXTURE_MANAGER->FindTexture(name);
    if (!foundTexture)
        return;

    SetTexture(foundTexture);
}

void TilemapComponent::SetTexture(const std::string& name, const std::wstring& fileName)
{
    if (!TEXTURE_MANAGER->LoadTexture(name, fileName))
        return;

    SetTexture(name);
}

void TilemapComponent::AddTileSprite(Vector2 start, Vector2 size)
{
    AddTileSprite(start.x, start.y, size.x, size.y);
}

void TilemapComponent::AddTileSprite(float startX, float startY, float sizeX, float sizeY)
{
    SpriteData sprite;
    sprite.start.x = startX;
    sprite.start.y = startY;
    sprite.size.x  = sizeX;
    sprite.size.y  = sizeY;

    _tileSprites.push_back(sprite);

    RefreshTileInstance(true);
    RefreshTileOutlineInstance(true);
}

void TilemapComponent::RenderTile()
{
    Ptr<Level> level = GetLevel();
    if (nullptr == level)
        return;

    if (!_tileTexture)
        return;

    if (_shouldRefreshTileInstance)
    {
        Ptr<Actor> owner = GetOwner();

        Vector2 ownerPos;
        ownerPos.x = owner->GetWorldPosition().x;
        ownerPos.y = owner->GetWorldPosition().y;

        _tileStructureBuffer->Clear();

        for (int32 i = 0; i < _countY; ++i)
        {
            for (int32 j = 0; j < _countX; ++j)
            {
                int32     index = i * _countX + j;
                Ptr<Tile> tile  = _tiles[index];

                int32 spriteIndex = tile->GetSpriteIndex();
                if (spriteIndex < 0 || spriteIndex >= _tileSprites.size())
                    continue;

                std::optional<Vector2> worldPos = GetTileWorldPos(index);

                Vector2 uvLT, uvRB;
                uvLT.x = _tileSprites[spriteIndex].start.x / _tileTextureSize.x;
                uvLT.y = _tileSprites[spriteIndex].start.y / _tileTextureSize.y;
                uvRB.x = uvLT.x + _tileSprites[spriteIndex].size.x / _tileTextureSize.x;
                uvRB.y = uvLT.y + _tileSprites[spriteIndex].size.y / _tileTextureSize.y;

                _tileStructureBuffer->AddData(worldPos.value(), uvLT, uvRB, _tileSize);
            }
        }

        _tileStructureBuffer->Update();
        RefreshTileInstance(false);
    }

    _tileTexture->SetShaderResource(0, ShaderType::Pixel, 0);
    SHADER_MANAGER->SetSampler(SamplerType::Point);

    auto transformBuffer = Lock(_transformConstantBuffer);

    transformBuffer->SetWorldMatrix(Matrix());
    transformBuffer->SetViewMatrix(level->GetViewMatrix());
    transformBuffer->SetProjMatrix(level->GetProjMatrix());
    transformBuffer->Update();

    _tileStructureBuffer->Bind();
    _tileInstanceShader->SetShader();

    _tileMesh->RenderInstancing(_tileStructureBuffer->GetElementCount());
}

void TilemapComponent::RenderOutline()
{
    if (!_shouldRenderOutline)
        return;

    Ptr<Level> level = GetLevel();
    if (nullptr == level)
        return;

    if (_shouldRefreshTileOutlineInstance)
    {
        for (int32 i = 0; i < _countY; ++i)
        {
            for (int32 j = 0; j < _countX; ++j)
            {
                int32     index = i * _countX + j;
                Ptr<Tile> tile  = _tiles[index];

                int32 spriteIndex = tile->GetSpriteIndex();
                if (spriteIndex < 0 || spriteIndex >= _tileSprites.size())
                    continue;

                std::optional<Vector2> worldPos = GetTileWorldPos(index);

                Vector4 color;
                switch (tile->GetTileType())
                {
                case TileType::Platform:
                    color = Vector4(0.f, 1.f, 0.f, 1.f);
                    break;
                case TileType::Wall:
                    color = Vector4(1.f, 0.f, 0.f, 1.f);
                    break;
                case TileType::Floor:
                    color = Vector4(1.f, 1.f, 0.f, 1.f);
                    break;
                default:
                    color = Vector4(0.f, 0.f, 0.f, 0.f);
                    break;
                }

                _tileOutlineStructureBuffer->AddData(color, worldPos.value(), _tileSize);
            }
        }

        _tileOutlineStructureBuffer->Update();
        RefreshTileOutlineInstance(false);
    }

    auto transformBuffer = Lock(_transformConstantBuffer);
    transformBuffer->SetWorldMatrix(Matrix());
    transformBuffer->SetViewMatrix(level->GetViewMatrix());
    transformBuffer->SetProjMatrix(level->GetProjMatrix());
    transformBuffer->Update();

    _tileOutlineStructureBuffer->Bind();
    _tileOutlineInstanceShader->SetShader();

    _tileOutlineMesh->RenderInstancing(_tileOutlineStructureBuffer->GetElementCount());
}
