#pragma once
#include "Core/Level.h"
#include "Core/World.h"
#include "Tilemap/Tilemap.h"

class TestLevel : public ::Level
{
private:
    Ptr<class Tilemap> _tilemap;

public:
    inline static const Vector2 boundaryStart{-8.f, -7.f};
    inline static const Vector2 boundaryEnd{8.f, 7.f};

public:
    bool Init(Ptr<class World> world, const std::string& path) override;

    Ptr<class Tilemap> GetTilemap() const;
};
