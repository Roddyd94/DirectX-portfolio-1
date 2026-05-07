#pragma once
#include "Object.h"

enum class ResourceType : uint8
{
    Mesh,
    Texture,
    Shader,
    Material,
    Animation,
    Sound,
    Font,
    End
};

class Resource : public Object
{
public:
    Resource()           = default;
    ~Resource() override = default;

protected:
    std::string _name;
    int32       _id;

public:
    const std::string& GetName() { return _name; }
    int32              GetID() { return _id; }

    void SetName(const std::string& name) { _name = name; }
    void SetID(int32 id) { _id = id; }
};
