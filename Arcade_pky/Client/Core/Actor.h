#pragma once
#include "Component.h"
#include "Object.h"
#include "Transform.h"

class Actor : public Object
{
    friend class Level;

public:
    Actor()           = default;
    ~Actor() override = default;

protected:
    std::string                                _name;
    std::unordered_map<std::string, int32>     _componentFinder;
    std::map<int32, Ptr<class ActorComponent>> _actorComponents;
    std::set<std::string>                      _tags;
    Ptr<class SceneComponent>                  _root;
    Weak<class Level>                          _level;
    int32                                      _componentIDCounter = 0;
    int32                                      _id                 = -1;

public:
    virtual bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation);
    void         Destroy() override;

    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);

    bool  HasTag(const std::string& tag) const;
    int32 GetActorID() const { return _id; }

    Ptr<class Level>          GetLevel() const;
    Ptr<class SceneComponent> GetRoot() const;

    Ptr<class SceneComponent> FindSceneComponent(int32 id) const;
    Ptr<class SceneComponent> FindSceneComponent(const std::string& name) const;
    Ptr<class ActorComponent> FindActorComponent(int32 id) const;
    Ptr<class ActorComponent> FindActorComponent(const std::string& name) const;

    const std::string& GetName() const;

    const Transform& GetWorldTransform() const;
    Vector3          GetWorldScale() const;
    Vector3          GetWorldPosition() const;
    Vector3          GetWorldRotation() const;

    const Transform& GetRelativeTransform() const;
    Vector3          GetRelativeScale() const;
    Vector3          GetRelativePosition() const;
    Vector3          GetRelativeRotation() const;

    void AddTag(const std::string& tag);

    void SetLevel(Ptr<class Level> level);
    void SetRoot(const Ptr<class SceneComponent> comp);

    void SetName(const std::string& name);

    void SetWorldTransform(const Transform& transform);
    void SetWorldTransform(Vector3 position, Vector3 scale, Vector3 rotation);
    void SetWorldScale(Vector3 scale);
    void SetWorldPosition(Vector3 position);
    void SetWorldRotation(Vector3 rotation);

    void SetRelativeTransform(const Transform& transform);
    void SetRelativeTransform(Vector3 position, Vector3 scale, Vector3 rotation);
    void SetRelativeScale(Vector3 scale);
    void SetRelativePosition(Vector3 pos);
    void SetRelativeRotation(Vector3 rot);

    void Remove();

public:
    template <typename T>
    Ptr<T> FindComponent(const std::string& name) const
    {
        Ptr<Component> componentFound = FindSceneComponent(name);
        if (componentFound)
            return Cast<Component, T>(componentFound);

        componentFound = FindActorComponent(name);
        if (componentFound)
            return Cast<Component, T>(componentFound);

        return nullptr;
    }

    template <typename T>
    Ptr<T> FindSceneComponent(const std::string& name) const
    {
        Ptr<SceneComponent> componentFound = FindSceneComponent(name);
        if (componentFound)
            return Cast<SceneComponent, T>(componentFound);

        return nullptr;
    }

    template <typename T>
    Ptr<T> FindActorComponent(const std::string& name) const
    {
        Ptr<ActorComponent> componentFound = FindActorComponent(name);
        if (componentFound)
            return Cast<ActorComponent, T>(componentFound);

        return nullptr;
    }

    template <typename... Args>
    void AddTags(Args&&... args)
    {
        (AddTag(std::forward<Args>(args)), ...);
    }

protected:
    template <typename T>
    Ptr<T> CreateSceneComponent(const std::string& name)
    {
        Ptr<T> comp = New<T>();
        if (!comp->Init(_componentIDCounter, name, This<Actor>()))
            DESTROY(comp)

        _componentFinder[name] = _componentIDCounter;
        _componentIDCounter++;

        return comp;
    }

    template <typename T>
    Ptr<T> CreateActorComponent(const std::string& name)
    {
        Ptr<T> comp = New<T>();
        if (!comp->Init(_componentIDCounter, name, This<Actor>()))
        {
            comp->Destroy();
            Delete(comp);
        }

        _componentFinder[name]                = _componentIDCounter;
        _actorComponents[_componentIDCounter] = comp;
        _componentIDCounter++;

        return comp;
    }
};
