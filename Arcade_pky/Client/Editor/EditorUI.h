#pragma once
#include "Core/Object.h"

class EditorUI : public Object
{
public:
    EditorUI()           = default;
    ~EditorUI() override = default;

protected:
    std::string _name;

public:
    virtual bool Init(const std::string& name);
    void         Destroy() override = 0;

    virtual void Tick(float deltaTime)   = 0;
    virtual void Render(float deltaTime) = 0;
};
