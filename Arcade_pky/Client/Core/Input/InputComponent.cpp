#include "pch.h"

#include "InputComponent.h"

#include "InputSystem.h"

#include "InputAction.h"
#include "InputContext.h"

bool InputComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    return ActorComponent::Init(id, name, owner);
}

void InputComponent::Destroy()
{
    _binds.clear();
}

void InputComponent::Tick(float deltaTime)
{
    ActorComponent::Tick(deltaTime);

    auto it = _binds.find(_activeContext);
    if (it == _binds.end())
        return;

    for (auto& [_, bindAction] : it->second)
    {
        InputMapping* mapping = bindAction.mapping;
        if (!mapping)
            continue;

        for (int i = 0; i < ButtonEventType::End; ++i)
        {
            if (mapping->buttonState[i] && bindAction.callback[i])
                bindAction.callback[i](deltaTime);
        }
    }
}

const std::string& InputComponent::GetActiveContext() const
{
    return _activeContext;
}

void InputComponent::SetActiveContext(const std::string& name)
{
    _activeContext = name;
}

void InputComponent::AddInputContext(const std::string& name)
{
    if (_binds.find(name) != _binds.end())
        return;

    Ptr<InputContext> foundContext = InputSystem::Instance().FindOrAddInputContext(name);
    if (!foundContext)
        return;

    for (auto& [_, mapping] : foundContext->_mappings)
    {
        BindingAction bind;
        bind.mapping = &mapping;
        bind.name    = bind.mapping->action->GetName();

        _binds[name][bind.name] = bind;
    }

    if (_activeContext.empty())
        _activeContext = name;

    InputSystem::Instance().AddActiveInputContext(foundContext);
}

void InputComponent::RemoveInputContext(const std::string& name)
{
    _binds.erase(name);
    InputSystem::Instance().RemoveActiveContext(name);
}

void InputComponent::RefreshInputMapping(
  const std::string& contextName, const InputMapping& mapping)
{
    auto it = _binds.find(contextName);
    if (_binds.end() == it)
        return;

    auto& bindingActions = it->second;
    auto  foundItArr     = bindingActions.find(mapping.action->GetName());
    if (bindingActions.end() != foundItArr)
    {
        BindingAction& bindAction = foundItArr->second;
        bindAction.mapping        = const_cast<InputMapping*>(&mapping);
    }
    else
    {
        BindingAction bind;
        bind.mapping = const_cast<InputMapping*>(&mapping);
        bind.name    = bind.mapping->action->GetName();

        _binds[contextName][bind.name] = bind;
    }
}
