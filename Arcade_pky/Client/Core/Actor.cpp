#include "pch.h"

#include "Actor.h"

#include "ActorComponent.h"
#include "Level.h"
#include "SceneComponent.h"

bool Actor::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    _id = id;

    _root = CreateSceneComponent<SceneComponent>("DefaultRoot");

    SetRelativePosition(position);
    SetRelativeScale(scale);
    SetRelativeRotation(rotation);

    SetEnable(true);
    SetActive(true);

    return true;
}

void Actor::Destroy()
{
    DESTROY(_root)

    for (auto& [_, actorComponents] : _actorComponents)
        DESTROY(actorComponents)

    _actorComponents.clear();
    _componentFinder.clear();
}

void Actor::Tick(float deltaTime)
{
    _root->Tick(deltaTime);

    for (auto& [_, actorComponent] : _actorComponents)
    {
        Ptr<ActorComponent> comp = actorComponent;
        if (nullptr == comp)
            continue;

        if (!comp->IsActive() || !comp->IsEnable())
            continue;

        comp->Tick(deltaTime);
    }
}

void Actor::Collision(float deltaTime) {}

bool Actor::HasTag(const std::string& tag) const
{
    return _tags.contains(tag);
}

Ptr<Level> Actor::GetLevel() const
{
    return Lock<Level>(_level);
}

Ptr<SceneComponent> Actor::GetRoot() const
{
    return _root;
}

Ptr<SceneComponent> Actor::FindSceneComponent(int32 id) const
{
    if (_root->GetComponentID() == id)
        return _root;

    return _root->FindComponent(id);
}

Ptr<SceneComponent> Actor::FindSceneComponent(const std::string& name) const
{
    if (_root->GetName() == name)
        return _root;

    if (auto it = _componentFinder.find(name); _componentFinder.end() != it)
        return FindSceneComponent(it->second);

    return nullptr;
}

Ptr<ActorComponent> Actor::FindActorComponent(int32 id) const
{
    if (auto it = _actorComponents.find(id); _actorComponents.end() != it)
        return it->second;

    return nullptr;
}

Ptr<ActorComponent> Actor::FindActorComponent(const std::string& name) const
{
    if (auto it = _componentFinder.find(name); _componentFinder.end() != it)
        return FindActorComponent(it->second);

    return nullptr;
}

const std::string& Actor::GetName() const
{
    return _name;
}

const Transform& Actor::GetWorldTransform() const
{
    return _root->GetWorldTransform();
}

Vector3 Actor::GetWorldScale() const
{
    return _root->GetWorldScale();
}

Vector3 Actor::GetWorldPosition() const
{
    return _root->GetWorldPosition();
}

Vector3 Actor::GetWorldRotation() const
{
    return _root->GetWorldRotation();
}

const Transform& Actor::GetRelativeTransform() const
{
    return _root->GetRelativeTransform();
}

Vector3 Actor::GetRelativeScale() const
{
    return _root->GetRelativeScale();
}

Vector3 Actor::GetRelativePosition() const
{
    return _root->GetRelativePosition();
}

Vector3 Actor::GetRelativeRotation() const
{
    return _root->GetRelativeRotation();
}

void Actor::AddTag(const std::string& tag)
{
    _tags.insert(tag);

    Ptr<Level> level = GetLevel();
    if (nullptr == level)
        return;

    level->AddTag(tag, _id);
}

void Actor::SetLevel(Ptr<Level> level)
{
    _level = level;
}

void Actor::SetRoot(const Ptr<SceneComponent> comp)
{
    if (_root)
    {
        comp->SetRelativeTransform(_root->GetRelativeTransform());

        for (auto& [_, child] : _root->_children)
        {
            if (child)
                child->AttachToComponent(comp);
        }

        _root->_children.clear();
        DESTROY(_root)

        _root = comp;
    }
    else
    {
        _root = comp;
    }
}

void Actor::SetName(const std::string& name)
{
    _name = name;
}

void Actor::SetWorldTransform(const Transform& transform)
{
    _root->SetWorldTransform(transform);
}

void Actor::SetWorldTransform(Vector3 position, Vector3 scale, Vector3 rotation)
{
    _root->SetWorldTransform(position, scale, rotation);
}

void Actor::SetWorldScale(Vector3 scale)
{
    _root->SetWorldScale(scale);
}

void Actor::SetWorldPosition(Vector3 position)
{
    _root->SetWorldScale(position);
}

void Actor::SetWorldRotation(Vector3 rotation)
{
    _root->SetWorldRotation(rotation);
}

void Actor::SetRelativeTransform(const Transform& transform)
{
    _root->SetRelativeTransform(transform);
}

void Actor::SetRelativeTransform(Vector3 position, Vector3 scale, Vector3 rotation)
{
    _root->SetRelativeTransform(position, scale, rotation);
}

void Actor::SetRelativeScale(Vector3 scale)
{
    _root->SetRelativeScale(scale);
}

void Actor::SetRelativePosition(Vector3 position)
{
    _root->SetRelativePosition(position);
}

void Actor::SetRelativeRotation(Vector3 rotation)
{
    _root->SetRelativeRotation(rotation);
}

void Actor::Remove()
{
    Ptr<Level> level = Lock<Level>(_level);

    if (level)
        level->RemoveActor(_id);
}
