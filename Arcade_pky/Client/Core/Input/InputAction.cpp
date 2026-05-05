#include "pch.h"

#include "InputAction.h"

void InputAction::Destroy() {}

const std::string& InputAction::GetName() const
{
    return _name;
}

void InputAction::SetName(const std::string& name)
{
    _name = name;
}
