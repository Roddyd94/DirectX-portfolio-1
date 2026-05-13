#include "pch.h"

#include "Client.h"

#include "Core/WindowsManager.h"

#include "framework.h"
#include "Core/GameEngine.h"
#include "Editor/EditorEngine.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#define MAX_LOADSTRING 100

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE                 hPrevInstance,
  _In_ LPWSTR                        lpCmdLine,
  _In_ int                           nCmdShow)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WindowsManager::Instance().Init(hInstance, L"Arcade");

#ifdef _EDITOR
    WindowsManager::Instance().OnUpdate = [&]() -> void
    {
        EditorEngine::Instance().Logic();
    };

    int result = EditorEngine::Instance().Init();
    WindowsManager::Instance().Run();
    EditorEngine::Instance().Destroy();
#else
    WindowsManager::Instance().OnUpdate = [&]() -> void
    {
        GameEngine::Instance().Logic();
    };

    int result = GameEngine::Instance().Init();
    WindowsManager::Instance().Run();
    GameEngine::Instance().Destroy();
#endif

    return result;
}
