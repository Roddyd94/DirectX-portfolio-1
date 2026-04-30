#include "pch.h"

#include "TestLevel.h"

#include "TestActor.h"
#include "Core/Camera.h"

bool TestLevel::Init(Ptr<World> world, const std::string& path)
{
    Level::Init(world, path);

    Vector3 position = Vector3{0.f, 0.f, 1.f};
    Vector3 scale    = Vector3{1.f, 1.f, 1.f};
    Vector3 rotation = Vector3{0.f, 0.f, 0.f};

    Ptr<TestActor> testActor = SpawnActor<TestActor>(position, scale, rotation);

    position.x += 0.5f;
    position.y += 0.5f;
    Ptr<TestActor> testActor1 = SpawnActor<TestActor>(position, scale, rotation);

    return true;
}
