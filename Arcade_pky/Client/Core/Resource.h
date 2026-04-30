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
    Resource() = default;
    ~Resource() override = default;

protected:
    std::string _name;

public:
    const std::string& GetName() { return _name; }

    void SetName(const std::string& name) { _name = name; }
};
