#pragma once
#include "Singleton.h"

class WindowsManager : public Singleton<WindowsManager>
{
    friend class GameEngine;
    DECLARE_SINGLETON(WindowsManager)
private:
    HINSTANCE _hInst          = nullptr;
    HWND      _hWnd           = nullptr;
    HDC       _hdc            = nullptr;
    TCHAR     _titleName[256] = {};
    TCHAR     _className[256] = {};

private:
    inline static bool _isRunning = false;

public:
    std::function<void()> OnUpdate;

public:
    bool Init(HINSTANCE inst, const wchar_t* name);
    void Destroy();

public:
    HINSTANCE GetHINSTANCE() const { return _hInst; }
    HWND      GetHWND() const { return _hWnd; }
    HDC       GetHDC() const { return _hdc; }

private:
    void RegisterWindowClass();
    bool Create();
    int  Run();

    static LRESULT CALLBACK WndProc(
      HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
