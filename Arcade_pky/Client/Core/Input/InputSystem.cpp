#include "pch.h"

#include "InputSystem.h"

#include "InputAction.h"
#include "InputComponent.h"
#include "InputContext.h"
#include "Core/Types.h"

bool InputSystem::Init(Ptr<Input> input)
{
    _input = input;

    return true;
}

void InputSystem::Destroy()
{
    _activeContexts.clear();

    for (auto& [_, context] : _allContexts)
        DESTROY(context);

    for (auto& [_, action] : _allActions)
        DESTROY(action);

    _allContexts.clear();
    _allActions.clear();
}

void InputSystem::Tick(float deltaTime)
{
    for (auto& [contextName, context] : _activeContexts)
    {
        for (auto& [mappingName, mapping] : context->_mappings)
        {
            uint8 key = mapping.key;

            if (key == DIK_MOUSELBUTTON || key == DIK_MOUSERBUTTON || key == DIK_MOUSEMBUTTON)
            {
                mapping.buttonState[ButtonEventType::Down] = _input->GetMouseEvent(
                  static_cast<uint16>(key - DIK_MOUSELBUTTON), ButtonEventType::Down);
                mapping.buttonState[ButtonEventType::Hold] = _input->GetMouseEvent(
                  static_cast<uint16>(key - DIK_MOUSELBUTTON), ButtonEventType::Hold);
                mapping.buttonState[ButtonEventType::Up] = _input->GetMouseEvent(
                  static_cast<uint16>(key - DIK_MOUSELBUTTON), ButtonEventType::Up);
            }
            else
            {
                bool keyPush = _input->GetKeyState(key);

                if (keyPush)
                {
                    if (!mapping.buttonState[ButtonEventType::Down]
                        && !mapping.buttonState[ButtonEventType::Hold])
                    {
                        mapping.buttonState[ButtonEventType::Down] = true;
                        mapping.buttonState[ButtonEventType::Hold] = true;
                    }
                    else
                    {
                        mapping.buttonState[ButtonEventType::Down] = false;
                    }
                }
                else
                {
                    if (mapping.buttonState[ButtonEventType::Hold])
                    {
                        mapping.buttonState[ButtonEventType::Down] = false;
                        mapping.buttonState[ButtonEventType::Hold] = false;
                        mapping.buttonState[ButtonEventType::Up]   = true;
                    }
                    else
                    {
                        mapping.buttonState[ButtonEventType::Up] = false;
                    }
                }
            }
        }
    }
}

Vector2 InputSystem::GetMouseWorldPos() const
{
    return _input->GetMouseWorldPosition();
}

Vector2 InputSystem::GetMousePos() const
{
    return _input->GetMousePosition();
}

Vector2 InputSystem::GetMouseMove() const
{
    return _input->GetMouseDelta();
}

bool InputSystem::GetMouseEvent(uint16 mouseButton, uint16 buttonEvent) const
{
    return _input->GetMouseEvent(mouseButton, buttonEvent);
}

bool InputSystem::IsActiveContext(const std::string& name) const
{
    return _activeContexts.contains(name);
}

Ptr<InputContext> InputSystem::FindOrAddInputContext(const std::string& name)
{
    auto it = _allContexts.find(name);
    if (_allContexts.end() != it)
        return it->second;

    Ptr<InputContext> context = New<InputContext>();
    context->_name            = name;
    _allContexts[name]        = context;

    return context;
}

Ptr<InputAction> InputSystem::FindOrAddInputAction(const std::string& name)
{
    auto it = _allActions.find(name);
    if (_allActions.end() != it)
        return it->second;

    Ptr<InputAction> context = New<InputAction>();
    context->_name           = name;
    _allActions[name]        = context;

    return context;
}

void InputSystem::AddActiveInputContext(Ptr<InputContext> context)
{
    _activeContexts[context->_name] = context;
}

void InputSystem::RemoveActiveContext(const std::string& name)
{
    _activeContexts.erase(name);
}

void InputSystem::RegisterInputComponent(int32 actorID, Ptr<InputComponent> inputComponent)
{
    ComponentIDPair key   = {actorID, inputComponent->GetComponentID()};
    _inputComponents[key] = inputComponent;
}

void InputSystem::RemoveInputComponent(int32 actorID, int32 componentID)
{
    ComponentIDPair key = {actorID, componentID};
    _inputComponents.erase(key);
}

void InputSystem::RefreshInputComponent(const std::string& contextName, const InputMapping& mapping)
{
    for (auto [_, comp] : _inputComponents)
    {
        Ptr<InputComponent> inputComponent = Lock<InputComponent>(comp);
        if (nullptr == inputComponent)
            continue;

        inputComponent->RefreshInputMapping(contextName, mapping);
    }
}

uint8 InputSystem::ConvertKey(uint8 key)
{
    return _input->ConvertKey(key);
}
