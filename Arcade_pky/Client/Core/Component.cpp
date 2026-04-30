#include "pch.h"

#include "Component.h"

#include "Actor.h"

bool Component::Init(
  int32 componentID, const std::string& name, Ptr<Actor> owner)
{
    _id    = componentID;
    _name  = name;
    _owner = owner;
    _level = owner->GetLevel();

    SetEnable(true);
    SetActive(true);

    return true;
}

void Component::Destroy()
{
    SetEnable(false);
    SetActive(false);
}

Ptr<class Actor> Component::GetOwner() const
{
    return Lock<Actor>(_owner);
}

Ptr<class Level> Component::GetLevel() const
{
    return Lock<Level>(_level);
}

void Component::SetOwner(Ptr<class Actor> owner)
{
    _owner = owner;
}
