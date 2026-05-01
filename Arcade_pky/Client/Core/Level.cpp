#include "pch.h"

#include "Level.h"

#include "CameraManager.h"

#include "Actor.h"
#include "Camera.h"
#include "World.h"

#ifdef _HAS_COLLISION_MODULE
#include "Core/Collision/CollisionManager.h"
#endif // _HAS_COLLISION_MODULE

bool Level::Init(Ptr<World> world, const std::string& path)
{
    _world = world;

    //_tagManager = New<TagManager>();
    //_tagManager->Init();

    _cameraManager = New<CameraManager>();
    _cameraManager->Init(This<Level>());

#ifdef _HAS_COLLISION_MODULE
    _collisionManager = New<CollisionManager>();
    _collisionManager->Init();
#endif // _HAS_COLLISION_MODULE

    //_uiManager = New<UIManager>();
    //_uiManager->Init(This<Level>());

    Vector3 position = Vector3{0.f, 0.f, -10.f};
    Vector3 scale    = Vector3{1.f, 1.f, 1.f};
    Vector3 rotation = Vector3{0.f, 0.f, 0.f};

    Ptr<Camera> camera = SpawnActor<Camera>(position, scale, rotation);

    return true;
}

void Level::Destroy()
{
    for (auto& [_, actor] : _actors)
        DESTROY(actor)

    _actors.clear();

    // TODO
    // DESTROY(_tagManager);
    // DESTROY(_uiManager);

#ifdef _HAS_COLLISION_MODULE
    DESTROY(_collisionManager);
#endif // _HAS_COLLISION_MODULE
    DESTROY(_cameraManager);
}

void Level::Tick(float deltaTime)
{
    for (auto actorID : _actorsToRemove)
    {
        Ptr<Actor> actor = FindActor<Actor>(actorID);
        if (nullptr == actor)
            continue;

        // TODO DeleteTag(actor);

        DESTROY(actor)
        _actors.erase(actorID);
    }

    _actorsToRemove.clear();

    for (auto& [actorID, actor] : _actors)
    {
        if (!actor->IsActive())
        {
            RemoveActor(actorID);
            continue;
        }

        if (!actor->IsEnable())
            continue;

        actor->Tick(deltaTime);
    }

    // TODO _uiManager->Tick(deltaTime);
}

void Level::Collision(float deltaTime)
{
#ifdef _HAS_COLLISION_MODULE
    _collisionManager->Collision(deltaTime);
#endif // _HAS_COLLISION_MODULE

    // TODO _uiManager->MouseEvent();

    for (auto& [actorID, actor] : _actors)
    {
        if (!actor->IsActive())
        {
            RemoveActor(actorID);
            continue;
        }

        if (!actor->IsEnable())
            continue;

        actor->Collision(deltaTime);
    }
}

Ptr<Actor> Level::FindActor(int32 actorID)
{
    if (auto it = _actors.find(actorID); _actors.end() != it)
        return it->second;

    return nullptr;
}

void Level::FindActors(
  const std::string& tag, OUT std::vector<Ptr<Actor>>& outArray)
{
    std::vector<int32> actorIDs;
    // TODO _tagManager->GetActorIDs(tag, actorIDs);

    if (actorIDs.size() < 1)
        return;

    for (auto actorID : actorIDs)
    {
        Ptr<Actor> actor = FindActor(actorID);
        if (nullptr == actor)
            continue;

        outArray.push_back(actor);
    }
}

const Matrix& Level::GetViewMatrix() const
{
    return _cameraManager->GetViewMatrix();
}

const Matrix& Level::GetProjMatrix() const
{
    return _cameraManager->GetProjMatrix();
}

Ptr<World> Level::GetWorld() const
{
    return Lock(_world);
}
#ifdef _HAS_COLLISION_MODULE
Ptr<CollisionProfileManager> Level::GetCollisionProfileManager() const
{
    return Lock(_world)->GetCollisionProfileManager();
}
#endif // _HAS_COLLISION_MODULE
Ptr<CameraComponent> Level::GetMainCamera() const
{
    return _cameraManager->GetMainCamera();
}

Vector3 Level::GetCameraWorldPosition() const
{
    return _cameraManager->GetCameraWorldPosition();
}

void Level::RemoveActor(int32 actorID)
{
    _actorsToRemove.push_back(actorID);
}

void Level::SetMainCamera(Ptr<CameraComponent> camera)
{
    _cameraManager->SetMainCamera(camera);
}

#ifdef _HAS_COLLISION_MODULE
Ptr<CollisionComponent> Level::FindCollider(const ComponentIDPair& colliderID)
{
    return _collisionManager->FindCollider(colliderID);
}

void Level::AddCollision(
  const ComponentIDPair& colliderID, Ptr<CollisionComponent> component)
{
    _collisionManager->Insert(colliderID, component);
}

void Level::RemoveCollision(const ComponentIDPair& colliderID)
{
    _collisionManager->Remove(colliderID);
}
#endif // _HAS_COLLISION_MODULE
