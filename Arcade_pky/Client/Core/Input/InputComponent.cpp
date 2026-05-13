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
    _bindingsByContext.clear();
}

void InputComponent::Tick(float deltaTime)
{
    ActorComponent::Tick(deltaTime);

    auto it = _bindingsByContext.find(_activeContext);
    if (it == _bindingsByContext.end())
        return;

    for (auto& [_, bindAction] : it->second)
    {
        InputMapping* mapping = bindAction.mapping;
        if (!mapping)
            continue;

        for (int i = 0; i < ButtonEventType::End; ++i)
        {
            if (mapping->buttonState[i] && bindAction.callback)
                bindAction.callback(mapping->action, ButtonEventType::Type(i));
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

void InputComponent::AddInputContext(Ptr<class InputContext> context)
{
    if (nullptr == context)
        return;

    auto itBind = _bindingsByContext.find(context->GetName());
    if (itBind != _bindingsByContext.end())
    {
        auto& binds = itBind->second;

        if (binds.size() == context->_mappings.size())
            return;
    }

    for (auto& [_, mapping] : context->_mappings)
    {
        if (_bindingsByContext[context->GetName()].contains(mapping.action->GetName()))
            continue;

        BindingAction bind;
        bind.mapping = &mapping;
        bind.name    = bind.mapping->action->GetName();

        _bindingsByContext[context->GetName()][bind.name] = bind;
    }

    if (_activeContext.empty())
        _activeContext = context->GetName();

    InputSystem::Instance().AddActiveInputContext(context);
}

void InputComponent::AddInputContext(const std::string& name)
{
    if (_bindingsByContext.find(name) != _bindingsByContext.end())
        return;

    Ptr<InputContext> foundContext = InputSystem::Instance().FindOrAddInputContext(name);
    AddInputContext(foundContext);
}

void InputComponent::RemoveInputContext(const std::string& name)
{
    _bindingsByContext.erase(name);
    InputSystem::Instance().RemoveActiveContext(name);
}

void InputComponent::RefreshInputMapping(
  const std::string& contextName, const InputMapping& mapping)
{
    auto it = _bindingsByContext.find(contextName);
    if (_bindingsByContext.end() == it)
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

        _bindingsByContext[contextName][bind.name] = bind;
    }
}
