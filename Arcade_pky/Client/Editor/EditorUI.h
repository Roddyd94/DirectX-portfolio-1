#pragma once
#include "Core/Object.h"

#ifdef _DEBUG
#pragma comment(lib, "imgui-win32-dx11.lib")
#endif // _DEBUG

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
