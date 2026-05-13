#pragma once
#ifdef _EDITOR
#include "EditorUI.h"
#include "Core/Singleton.h"

class EditorEngine : public Singleton<EditorEngine>
{
    DECLARE_SINGLETON(EditorEngine)

private:
    std::unordered_map<std::string, Ptr<EditorUI>> _editorUIs;

public:
    bool Init();
    void Destroy();
    void Logic();

private:
    void Tick(float deltaTime);
    void Collision(float deltaTime);
    void Render(float deltaTime);

public:
    template <typename T>
    Ptr<T> FindEditorUI(const std::string& name)
    {
        if (auto it = _editorUIs.find(name); _editorUIs.end() != it)
            return Cast<EditorUI, T>(it->second);

        return nullptr;
    }

private:
    template <typename T>
    Ptr<T> CreateEditorUI(const std::string& name)
    {
        Ptr<T> editorUI = FindEditorUI(name);
        if (nullptr != editorUI)
            return;

        editorUI = New<T>();
        if (!editorUI->Init(name))
        {
            DESTROY(editorUI);
            return;
        }

        _editorUIs[name] = editorUI;
    }
};
#endif // _EDITOR