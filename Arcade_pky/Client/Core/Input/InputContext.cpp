#include "pch.h"

#include "InputContext.h"

#include "InputSystem.h"

#include "InputAction.h"

void InputContext::Destroy()
{
    _mappings.clear();
}

const std::string& InputContext::GetName() const
{
    return _name;
}

InputMapping* InputContext::FindMapping(const std::string& name)
{
    if (auto it = _mappings.find(name); _mappings.end() != it)
        return &(it->second);

    return nullptr;
}

void InputContext::SetName(const std::string& name)
{
    _name = name;
}

void InputContext::BindInputAction(Ptr<class InputAction> action, uint8 key)
{
    auto it = _mappings.find(action->GetName());
    if (_mappings.end() != it)
        return;

    InputMapping mapping;
    mapping.action = action;
    mapping.key    = InputSystem::Instance().ConvertKey(key);

    _mappings[action->GetName()] = mapping;
    InputSystem::Instance().RefreshInputComponent(_name, _mappings[action->GetName()]);
}

void InputContext::ChangeInputActionKey(const std::string& actionName, uint8 key)
{
    auto it = _mappings.find(actionName);
    if (_mappings.end() != it)
        _mappings[actionName].key = InputSystem::Instance().ConvertKey(key);
}
