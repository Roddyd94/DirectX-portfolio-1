#pragma once
#include "Object.h"
#include "Types.h"
#include "Collision/CollisionSystem.h"

class Level : public Object
{
public:
    Level()           = default;
    ~Level() override = default;
    DELETE_SPECIAL_FUNC(Level)

private:
    std::vector<int32>                _actorsToRemove;
    std::map<int32, Ptr<class Actor>> _actors;
    Weak<class World>                 _world;
    Ptr<class CameraManager>          _cameraManager;
#ifdef _HAS_COLLISION_MODULE
    Ptr<class CollisionManager> _collisionManager;
#endif // _HAS_COLLISION_MODULE
    Ptr<class TagManager> _tagManager;
    // Ptr<class UIManager>              _uiManager;
    int32 _actorIDCounter = 0;

public:
    virtual bool Init(Ptr<class World> world, const std::string& path);
    void         Destroy() override;

    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    //  TODO UI virtual void RenderUI(float deltaTime);

    Ptr<class Actor> FindActor(int32 actorID);
    void FindActors(const std::string& tag, OUT std::vector<Ptr<class Actor>>& outArray);

    const Matrix& GetViewMatrix() const;
    const Matrix& GetProjMatrix() const;

    Ptr<class World>           GetWorld() const;
    Ptr<class CameraComponent> GetMainCamera() const;
    Vector3                    GetCameraWorldPosition() const;
#ifdef _HAS_COLLISION_MODULE
    Ptr<class CollisionManager> GetCollisionManager() const;
    Ptr<class CollisionProfileManager> GetCollisionProfileManager() const;
#endif // _HAS_COLLISION_MODULE

    // TODO UI const Matrix& GetUIProjMatrix() const;

    void AddTag(const std::string& tag, int32 actorID);
    void DeleteTag(Ptr<class Actor> actor);

    void RemoveActor(int32 actorID);
    void SetMainCamera(Ptr<class CameraComponent> camera);

#ifdef _HAS_COLLISION_MODULE
    Ptr<class CollisionComponent> FindCollider(
      ColliderType::Type colliderType, const ComponentIDPair& colliderID);
    void AddCollision(Ptr<class CollisionComponent> collider);
    void RemoveCollision(Ptr<class CollisionComponent> collider);
#endif // _HAS_COLLISION_MODULE

public:
    template <typename T>
    Ptr<T> FindActor(int32 actorID)
    {
        if (auto it = _actors.find(actorID); _actors.end() != it)
            return Cast<Actor, T>(it->second);

        return nullptr;
    }

    template <typename T>
    Ptr<T> SpawnActor(Vector3 position, Vector3 scale, Vector3 rotation)
    {
        Ptr<T> actor = New<T>();
        actor->SetLevel(This<Level>());

        int32 actorID = _actorIDCounter++;
        if (!actor->Init(actorID, position, scale, rotation))
        {
            DESTROY(actor)
            return nullptr;
        }

        _actors[actorID] = actor;

        return actor;
    }

    // TODO UI
    // template <typename T>
    // Ptr<T> CreateWidget(const std::string& name)
    //{
    //    return _uiManager->CreateWidget<T>(name);
    //}
};
