#include "pch.h"

#include "Input.h"

#include "Core/DeviceManager.h"
#include "Core/WindowsManager.h"

#include "Core/GameEngine.h"
#include "Core/Level.h"
#include "Core/World.h"

bool Input::Init()
{
    _hInst = WindowsManager::Instance().GetHINSTANCE();
    _hWnd  = WindowsManager::Instance().GetHWND();

    if (FAILED(DirectInput8Create(
          _hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)_input.GetAddressOf(), nullptr)))
        _type = InputSystemType::Window;
    else
        _type = InputSystemType::DInput;

    if (InputSystemType::DInput == _type)
    {
        if (!CreateInputDevice(_keyboard, GUID_SysKeyboard, &c_dfDIKeyboard))
            return false;

        if (!CreateInputDevice(_mouse, GUID_SysMouse, &c_dfDIMouse))
            return false;
    }

    return true;
}

void Input::Destroy() {}

void Input::Tick(float deltaTime)
{
    if (InputSystemType::DInput == _type)
    {
        UpdateInputDevice(_keyboard, (LPVOID)&_keyState, 256);
        UpdateInputDevice(_mouse, (LPVOID)&_mouseState, sizeof(DIMOUSESTATE));
    }

    UpdateMousePosition(deltaTime);
    UpdateMouseState(deltaTime);
}

bool Input::GetKeyState(uint8 key)
{
    return _keyState[key] & KEY_PUSH;
}

void Input::SetMouseEventOn(uint16 mouseButton, uint16 buttonEvent)
{
    _mouseEvents |= ((1 << (mouseButton << 1)) * buttonEvent);
}

void Input::SetMouseEventOff(uint16 mouseButton, uint16 buttonEvent)
{
    _mouseEvents &= ~((1 << (mouseButton << 1)) * buttonEvent);
}

bool Input::CreateInputDevice(
  ComPtr<IDirectInputDevice8W> device, GUID deviceGuid, LPCDIDATAFORMAT df)
{
    if (FAILED(_input->CreateDevice(deviceGuid, device.GetAddressOf(), nullptr)))
        return false;

    if (FAILED(device->SetDataFormat(df)))
        return false;

    if (DeviceManager::Instance().GetWindowMode())
    {
        if (FAILED(device->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
            return false;
    }
    else
    {
        if (FAILED(device->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
            return false;
    }

    if (FAILED(device->Acquire()))
        return false;

    return true;
}

bool Input::UpdateInputDevice(
  ComPtr<IDirectInputDevice8W> device, LPVOID deviceState, DWORD sizeOfDeviceState)
{
    if (!device)
        return false;

    HRESULT result = device->GetDeviceState(sizeOfDeviceState, deviceState);
    if (FAILED(result))
    {
        if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
            device->Acquire();
    }

    return true;
}

void Input::UpdateMousePosition(float deltaTime)
{
    POINT mousePoint;
    GetCursorPos(&mousePoint);
    ScreenToClient(_hWnd, &mousePoint);

    Vector2    ratio              = DeviceManager::Instance().GetResolutionRate();
    Resolution viewportResolution = DeviceManager::Instance().GetResolution();

    Vector2 newMousePos;
    newMousePos.x = mousePoint.x * ratio.x;
    newMousePos.y = mousePoint.y * ratio.y;

    newMousePos.y = viewportResolution.height - newMousePos.y;

    if (_mouseCompute)
        _mouseDelta = newMousePos - _mousePosition;
    else
        _mouseCompute = true;

    _mousePosition = newMousePos;

    Ptr<World> world = GameEngine::Instance().GetWorld();
    Ptr<Level> level = world->GetCurrentLevel();
    if (nullptr == level)
        return;

    Vector3 cameraPos = level->GetCameraWorldPosition();

    _mouseWorldPosition.x = cameraPos.x + _mousePosition.x - viewportResolution.width * 0.5f;
    _mouseWorldPosition.y = cameraPos.y + _mousePosition.y - viewportResolution.height * 0.5f;
}

void Input::UpdateMouseState(float deltaTime)
{
    if (_type == InputSystemType::DInput)
    {
        for (uint16 i = 0; i < MouseButtonType::Type::End; ++i)
        {
            if (_mouseState.rgbButtons[i] & KEY_PUSH)
            {
                if (!GetMouseEvent(i, ButtonEventType::Down)
                    && !GetMouseEvent(i, ButtonEventType::Hold))
                {
                    SetMouseEventOn(i, ButtonEventType::Down);
                    SetMouseEventOn(i, ButtonEventType::Hold);
                }
                else
                {
                    SetMouseEventOff(i, ButtonEventType::Down);
                }
            }
            else
            {
                if (GetMouseEvent(i, ButtonEventType::Hold))
                {
                    SetMouseEventOff(i, ButtonEventType::Down);
                    SetMouseEventOff(i, ButtonEventType::Hold);
                    SetMouseEventOn(i, ButtonEventType::Up);
                }
                else
                {
                    SetMouseEventOff(i, ButtonEventType::Up);
                }
            }
        }
    }
}

uint8 Input::ConvertKey(uint8 key)
{
    if (InputSystemType::DInput == _type)
    {
        switch (key)
        {
        case VK_LBUTTON:
            return DIK_MOUSELBUTTON;
        case VK_RBUTTON:
            return DIK_MOUSERBUTTON;
        case VK_MBUTTON:
            return DIK_MOUSEMBUTTON;
        case VK_BACK:
            return DIK_BACK;
        case VK_TAB:
            return DIK_TAB;
        case VK_RETURN:
            return DIK_RETURN;
        case VK_LCONTROL:
            return DIK_LCONTROL;
        case VK_RCONTROL:
            return DIK_RCONTROL;
        case VK_LMENU:
            return DIK_LALT;
        case VK_RMENU:
            return DIK_RALT;
        case VK_LSHIFT:
            return DIK_LSHIFT;
        case VK_RSHIFT:
            return DIK_RSHIFT;
        case VK_PAUSE:
            return DIK_PAUSE;
        case VK_CAPITAL:
            return DIK_CAPSLOCK;
        case VK_ESCAPE:
            return DIK_ESCAPE;
        case VK_SPACE:
            return DIK_SPACE;
        case VK_NEXT:
            return DIK_PGDN;
        case VK_PRIOR:
            return DIK_PGUP;
        case VK_END:
            return DIK_END;
        case VK_HOME:
            return DIK_HOME;
        case VK_LEFT:
            return DIK_LEFT;
        case VK_UP:
            return DIK_UP;
        case VK_RIGHT:
            return DIK_RIGHT;
        case VK_DOWN:
            return DIK_DOWN;
        case VK_PRINT:
            return DIK_SYSRQ;
        case VK_INSERT:
            return DIK_INSERT;
        case VK_DELETE:
            return DIK_DELETE;
        case VK_HELP:
            return 0;
        case '0':
            return DIK_0;
        case '1':
            return DIK_1;
        case '2':
            return DIK_2;
        case '3':
            return DIK_3;
        case '4':
            return DIK_4;
        case '5':
            return DIK_5;
        case '6':
            return DIK_6;
        case '7':
            return DIK_7;
        case '8':
            return DIK_8;
        case '9':
            return DIK_9;
        case 'A':
            return DIK_A;
        case 'B':
            return DIK_B;
        case 'C':
            return DIK_C;
        case 'D':
            return DIK_D;
        case 'E':
            return DIK_E;
        case 'F':
            return DIK_F;
        case 'G':
            return DIK_G;
        case 'H':
            return DIK_H;
        case 'I':
            return DIK_I;
        case 'J':
            return DIK_J;
        case 'K':
            return DIK_K;
        case 'L':
            return DIK_L;
        case 'M':
            return DIK_M;
        case 'N':
            return DIK_N;
        case 'O':
            return DIK_O;
        case 'P':
            return DIK_P;
        case 'Q':
            return DIK_Q;
        case 'R':
            return DIK_R;
        case 'S':
            return DIK_S;
        case 'T':
            return DIK_T;
        case 'U':
            return DIK_U;
        case 'V':
            return DIK_V;
        case 'W':
            return DIK_W;
        case 'X':
            return DIK_X;
        case 'Y':
            return DIK_Y;
        case 'Z':
            return DIK_Z;
        case VK_OEM_3:
            return DIK_GRAVE;
        case VK_OEM_MINUS:
            return DIK_MINUS;
        case VK_OEM_NEC_EQUAL:
            return DIK_EQUALS;
        case VK_OEM_4:
            return DIK_LBRACKET;
        case VK_OEM_6:
            return DIK_RBRACKET;
        case VK_OEM_5:
            return DIK_BACKSLASH;
        case VK_OEM_1:
            return DIK_SEMICOLON;
        case VK_OEM_7:
            return DIK_APOSTROPHE;
        case VK_OEM_COMMA:
            return DIK_COMMA;
        case VK_OEM_PERIOD:
            return DIK_PERIOD;
        case VK_OEM_2:
            return DIK_SLASH;
        case VK_NUMPAD0:
            return DIK_NUMPAD0;
        case VK_NUMPAD1:
            return DIK_NUMPAD1;
        case VK_NUMPAD2:
            return DIK_NUMPAD2;
        case VK_NUMPAD3:
            return DIK_NUMPAD3;
        case VK_NUMPAD4:
            return DIK_NUMPAD4;
        case VK_NUMPAD5:
            return DIK_NUMPAD5;
        case VK_NUMPAD6:
            return DIK_NUMPAD6;
        case VK_NUMPAD7:
            return DIK_NUMPAD7;
        case VK_NUMPAD8:
            return DIK_NUMPAD8;
        case VK_NUMPAD9:
            return DIK_NUMPAD9;
        case VK_MULTIPLY:
            return DIK_MULTIPLY;
        case VK_ADD:
            return DIK_ADD;
        case VK_SEPARATOR:
            return DIK_NUMPADCOMMA;
        case VK_SUBTRACT:
            return DIK_SUBTRACT;
        case VK_DECIMAL:
            return DIK_DECIMAL;
        case VK_DIVIDE:
            return DIK_DIVIDE;
            // case VK_RETURN:
            // return DIK_NUMPADENTER;
        case VK_F1:
            return DIK_F1;
        case VK_F2:
            return DIK_F2;
        case VK_F3:
            return DIK_F3;
        case VK_F4:
            return DIK_F4;
        case VK_F5:
            return DIK_F5;
        case VK_F6:
            return DIK_F6;
        case VK_F7:
            return DIK_F7;
        case VK_F8:
            return DIK_F8;
        case VK_F9:
            return DIK_F9;
        case VK_F10:
            return DIK_F10;
        case VK_F11:
            return DIK_F11;
        case VK_F12:
            return DIK_F12;
        case VK_F13:
            return DIK_F13;
        case VK_F14:
            return DIK_F14;
        case VK_F15:
            return DIK_F15;
        case VK_F16:
        case VK_F17:
        case VK_F18:
        case VK_F19:
        case VK_F20:
        case VK_F21:
        case VK_F22:
        case VK_F23:
        case VK_F24:
            return 0;
        case VK_NUMLOCK:
            return DIK_NUMLOCK;
        case VK_SCROLL:
            return DIK_SCROLL;
        case VK_LWIN:
            return DIK_LWIN;
        case VK_RWIN:
            return DIK_RWIN;
        case VK_APPS:
            return DIK_APPS;
        case VK_OEM_102:
            return DIK_OEM_102;
        }
        return 0xff;
    }
    return key;
}
