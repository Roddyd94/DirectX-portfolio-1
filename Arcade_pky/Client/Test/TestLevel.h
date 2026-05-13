#pragma once
#include "Core/Level.h"
#include "Core/World.h"
#include "Tilemap/Tilemap.h"

namespace snowbros
{
    class TestLevel : public ::Level
    {
    private:
        Ptr<class Tilemap> _tilemap;

    public:
        bool Init(Ptr<class World> world, const std::string& path) override;

        Ptr<class Tilemap> GetTilemap() const;
    };
} // namespace snowbros
