#pragma once
#include "Core/Level.h"

class TilemapLevel : public Level
{
public:
    TilemapLevel()           = default;
    ~TilemapLevel() override = default;
    DELETE_SPECIAL_FUNC(TilemapLevel)

protected:
    Ptr<class Tilemap> _tilemap;

public:
    Rect _boundary{-8.f, 6.f, 8.f, -7.f};

public:
    bool Init(Ptr<class World> world, const std::string& path) override;
    void Destroy() override;

    Rect GetBoundary() const;
    void SetBoundary(Rect boundary);

    Ptr<class Tilemap> GetTilemap() const;
};
