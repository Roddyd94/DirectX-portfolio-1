#include "pch.h"

#include "TestLevel.h"

#include "TestActor.h"
#include "Core/Camera.h"
#include "Tilemap/Tilemap.h"

bool TestLevel::Init(Ptr<World> world, const std::string& path)
{
    Level::Init(world, path);

    Vector3 position = Vector3{0.f, 0.f, 1.f};
    Vector3 scale    = Vector3{1.f, 1.f, 1.f};
    Vector3 rotation = Vector3{0.f, 0.f, 0.f};

    Ptr<TestActor> testActor = SpawnActor<TestActor>(position, scale, rotation);

    position.x = -7.5f;
    position.y = -6.5f;

    Ptr<Tilemap> tilemap = SpawnActor<Tilemap>(position, scale, rotation);
    tilemap->SetTexture("Tile", L"TileSet.png");
    tilemap->CreateTile(16, 14, {1.f, 1.f}, 0);
    tilemap->AddTileFrame(0.f, 0.f, 64.f, 64.f);
    tilemap->AddTileFrame(0.f, 64.f, 64.f, 64.f);
    tilemap->AddTileFrame(0.f, 128.f, 64.f, 64.f);
    tilemap->AddTileFrame(0.f, 192.f, 64.f, 64.f);
    tilemap->AddTileFrame(0.f, 256.f, 64.f, 64.f);

    return true;
}
