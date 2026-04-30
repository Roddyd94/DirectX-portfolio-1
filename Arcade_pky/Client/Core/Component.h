#pragma once
#include "Object.h"

class Component : public Object
{
    friend class Actor;

public:
    Component()          = default;
    virtual ~Component() = default;

protected:
    std::string       _name;
    Weak<class Actor> _owner;
    Weak<class Level> _level;
    int32             _id = -1;

public:
    virtual bool Init(
      int32 componentID, const std::string& name, Ptr<class Actor> owner);
    void Destroy() override;

    virtual void Tick(float deltaTime) = 0;

    const std::string& GetName() const { return _name; }
    int32              GetComponentID() const { return _id; }
    Ptr<class Actor>   GetOwner() const;
    Ptr<class Level>   GetLevel() const;

    void SetName(const std::string& name) { _name = name; }
    void SetComponentID(int32 id) { _id = id; }
    void SetOwner(Ptr<class Actor> owner);
};
