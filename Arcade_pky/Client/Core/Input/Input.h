#pragma once
#include "Types.h"
#include "Core/Object.h"

#define DIRECTINPUT_VERSION 0x08'00

#define DIK_MOUSELBUTTON 0xFC
#define DIK_MOUSERBUTTON 0xFD
#define DIK_MOUSEMBUTTON 0xFE
#define DIK_MOUSEWHEEL   0xFE

#define KEY_PUSH 0x80

#include <dinput.h>

#pragma comment(lib, "dinput8.lib")

class Input : public Object
{
    friend class InputSystem;

public:
    Input()           = default;
    ~Input() override = default;
    DELETE_SPECIAL_FUNC(Input)

private:
    uint8 _keyState[256] = {};

    HINSTANCE _hInst = 0;
    HWND      _hWnd  = 0;

    ComPtr<IDirectInput8>       _input    = nullptr;
    ComPtr<IDirectInputDevice8> _keyboard = nullptr;
    ComPtr<IDirectInputDevice8> _mouse    = nullptr;

    InputSystemType _type = InputSystemType::DInput;

    DIMOUSESTATE _mouseState = {};

    Vector2 _mousePosition;
    Vector2 _mouseWorldPosition;
    Vector2 _mouseDelta;

    uint16 _mouseEvents  = 0x00;
    bool   _mouseCompute = true;

public:
    bool Init();
    void Destroy() override;

    void Tick(float deltaTime);

    bool GetKeyState(uint8 key);

    bool GetMouseEvent(uint16 mouseButton, uint16 buttonEvent) const
    {
        return _mouseEvents & (1 << buttonEvent) << (mouseButton << 2);
    }

    Vector2 GetMouseWorldPosition() const { return _mouseWorldPosition; }
    Vector2 GetMousePosition() const { return _mousePosition; }
    Vector2 GetMouseDelta() const { return _mouseDelta; }

private:
    void SetMouseEventOn(uint16 mouseButton, uint16 buttonEvent);
    void SetMouseEventOff(uint16 mouseButton, uint16 buttonEvent);

    bool CreateInputDevice(
      ComPtr<IDirectInputDevice8W> device, GUID deviceGuid, LPCDIDATAFORMAT df);
    bool UpdateInputDevice(
      ComPtr<IDirectInputDevice8W> device, LPVOID deviceState, DWORD sizeOfDeviceState);

    void UpdateMousePosition(float deltaTime);
    void UpdateMouseState(float deltaTime);

    uint8 ConvertKey(uint8 key);
};
