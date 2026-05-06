#include "pch.h"

#include "WindowsManager.h"

#include "GameEngine.h"

#include "Info.h"
#include "Resource.h"

#ifdef _EDITOR
#include "imgui/imgui_impl_win32.h"
#endif // _EDITOR

bool WindowsManager::Init(HINSTANCE inst, const wchar_t* name)
{
    _hInst = inst;
    lstrcpy(_className, name);
    lstrcpy(_titleName, name);

    RegisterWindowClass();

    if (!Create())
        return false;

    _hdc = GetDC(_hWnd);

    return true;
}

void WindowsManager::Destroy() {}

void WindowsManager::RegisterWindowClass()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.lpszMenuName  = 0;
    wcex.hInstance     = _hInst;
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = _className;
    wcex.hIcon         = NULL;
    wcex.hIconSm       = NULL;

    RegisterClassExW(&wcex);
}

bool WindowsManager::Create()
{
    _hWnd = CreateWindowW(_className, _titleName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
      CW_USEDEFAULT, 0, nullptr, nullptr, _hInst, nullptr);

    if (!_hWnd)
        return false;

    uint32 width  = defaultResolution.width;
    uint32 height = defaultResolution.height;

    RECT windowRect = {0, 0, width, height};

    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);
    SetWindowPos(_hWnd, HWND_TOPMOST, 0, 0, windowRect.right - windowRect.left,
      windowRect.bottom - windowRect.top, SWP_NOMOVE | SWP_NOZORDER);

    ShowWindow(_hWnd, SW_SHOW);
    UpdateWindow(_hWnd);

    return true;
}

int WindowsManager::Run()
{
    MSG msg = {};

    while (_isRunning)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            OnUpdate();
        }
    }

    return (int)msg.wParam;
}

LRESULT WindowsManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _EDITOR
    ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
#endif // _EDITOR

    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC         hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
    {
        _isRunning = false;
        PostQuitMessage(0);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
