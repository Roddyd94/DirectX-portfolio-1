#include "pch.h"

#include "Tilemap.h"

#include "TilemapComponent.h"

bool Tilemap::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Actor::Init(id, position, scale, rotation);

    _tileComponent = CreateSceneComponent<TilemapComponent>("Tile");
    SetRoot(_tileComponent);

    return true;
}

void Tilemap::Destroy()
{
    Actor::Destroy();
}

void Tilemap::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Tilemap::Collision(float deltaTime)
{
    Actor::Collision(deltaTime);
}

void Tilemap::CreateTile(
  int32 countX, int32 countY, Vector2 tileSize, int32 textureFrameIndex)
{
    _tileComponent->CreateTile(countX, countY, tileSize, textureFrameIndex);
}

void Tilemap::SetTexture(Ptr<Texture> texture)
{
    _tileComponent->SetTexture(texture);
}

void Tilemap::SetTexture(const std::string& name)
{
    _tileComponent->SetTexture(name);
}

void Tilemap::SetTexture(const std::string& name, const std::wstring& fileName)
{
    _tileComponent->SetTexture(name, fileName);
}

void Tilemap::AddTileFrame(Vector2 start, Vector2 size)
{
    _tileComponent->AddTileFrame(start, size);
}

void Tilemap::AddTileFrame(float startX, float startY, float sizeX, float sizeY)
{
    _tileComponent->AddTileFrame(startX, startY, sizeX, sizeY);
}
