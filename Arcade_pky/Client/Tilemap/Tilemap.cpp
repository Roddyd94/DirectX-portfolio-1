#include "pch.h"

#include "Tilemap.h"

#include "TilemapComponent.h"

bool Tilemap::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Actor::Init(id, position, scale, rotation);

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

void Tilemap::CreateTile(int32 countX, int32 countY, Vector2 tileSize, int32 textureFrameIndex)
{
    _tileComponent->CreateTile(countX, countY, tileSize, textureFrameIndex);
}

void Tilemap::AddTileSprite(Vector2 start, Vector2 size)
{
    _tileComponent->AddTileSprite(start, size);
}

void Tilemap::AddTileSprite(float startX, float startY, float sizeX, float sizeY)
{
    _tileComponent->AddTileSprite(startX, startY, sizeX, sizeY);
}

Ptr<class Tile> Tilemap::GetTile(int32 index)
{
    return _tileComponent->GetTile(index);
}

Ptr<class Tile> Tilemap::GetTile(Vector2 worldPosition)
{
    return _tileComponent->GetTile(worldPosition);
}

Ptr<class Tile> Tilemap::GetTileLocal(Vector2 localPosition)
{
    return _tileComponent->GetTileLocal(localPosition);
}

int32 Tilemap::GetTileIndexX(float worldPositionX) const
{
    return _tileComponent->GetTileIndexX(worldPositionX);
}

int32 Tilemap::GetTileIndexY(float worldPositionY) const
{
    return _tileComponent->GetTileIndexY(worldPositionY);
}

int32 Tilemap::GetTileIndex(Vector2 worldPosition) const
{
    return _tileComponent->GetTileIndex(worldPosition);
}
