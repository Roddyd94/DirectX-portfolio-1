#include "pch.h"
#ifdef _EDITOR
#include "EditorEngine.h"

#include "Core/DeviceManager.h"
#include "Core/TimeManager.h"
#include "Core/WindowsManager.h"

#include "EditorUI.h"
#include "Core/GameEngine.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

bool EditorEngine::Init()
{
    GameEngine::Instance().Init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplWin32_Init(WindowsManager::Instance().GetHWND());
    ImGui_ImplDX11_Init(DEVICE.Get(), CONTEXT.Get());

    return true;
}

void EditorEngine::Destroy()
{
    GameEngine::Instance().Destroy();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void EditorEngine::Logic()
{
    float deltaTime = TimeManager::Instance().Tick();

    Tick(deltaTime);
    Collision(deltaTime);
    Render(deltaTime);
}

void EditorEngine::Tick(float deltaTime)
{
    GameEngine::Instance().Tick(deltaTime);

    for (auto& [_, editorUI] : _editorUIs)
        editorUI->Tick(deltaTime);
}

void EditorEngine::Collision(float deltaTime)
{
    GameEngine::Instance().Collision(deltaTime);
}

void EditorEngine::Render(float deltaTime)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    GameEngine::Instance().Render(deltaTime);

    for (auto& [_, editorUI] : _editorUIs)
    {
        if (!editorUI->IsEnable())
            continue;

        editorUI->Render(deltaTime);
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    DeviceManager::Instance().Render();
}
#endif // _EDITOR
