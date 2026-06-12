#pragma once
#include "Types.h"
#include "Core/ActorComponent.h"
#include "Core/Input/InputContext.h"
#include "Core/Input/InputSystem.h"

struct BindingAction
{
    std::string          name;
    struct InputMapping* mapping = nullptr;

    std::function<void(Ptr<class InputAction>, ButtonEventType::Type)> callback;
};

class InputComponent : public ActorComponent
{
public:
    InputComponent()           = default;
    ~InputComponent() override = default;

private:
    std::unordered_map<std::string, std::unordered_map<std::string, BindingAction>>
                _bindingsByContext;
    std::string _activeContext;

public:
    bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    const std::string& GetActiveContext() const;

    void SetActiveContext(const std::string& name);
    void AddInputContext(Ptr<class InputContext> context);
    void AddInputContext(const std::string& name);
    void RemoveInputContext(const std::string& name);
    void RefreshInputMapping(const std::string& contextName, const struct InputMapping& mapping);
    void ChangeInputActionKey(
      const std::string& contextName, const std::string& actionName, uint8 key);

    template <typename T>
    void BindAction(
      const std::string& contextName, const std::string& actionName, uint8 key, T&& func)
    {
        Ptr<InputContext> context = InputSystem::Instance().FindOrAddInputContext(contextName);
        Ptr<InputAction>  action  = InputSystem::Instance().FindOrAddInputAction(actionName);

        context->BindInputAction(action, key);

        AddInputContext(context);

        auto it = _bindingsByContext.find(contextName);
        if (_bindingsByContext.end() == it)
            return;

        _bindingsByContext[contextName][actionName].callback = std::forward<T>(func);
    }

    template <typename T>
    void BindAction(const std::string& contextName,
      const std::string&               actionName,
      uint8                            key,
      T*                               obj,
      void (T::*memFunc)(Ptr<class InputAction>, ButtonEventType::Type))
    {
        Ptr<InputContext> context = InputSystem::Instance().FindOrAddInputContext(contextName);
        Ptr<InputAction>  action  = InputSystem::Instance().FindOrAddInputAction(actionName);

        context->BindInputAction(action, key);

        AddInputContext(context);

        auto it = _bindingsByContext.find(contextName);
        if (_bindingsByContext.end() == it)
            return;

        _bindingsByContext[contextName][actionName].callback
          = std::bind(memFunc, obj, std::placeholders::_1, std::placeholders::_2);
    }

    template <typename T>
    void BindAction(const std::string& contextName, const std::string& actionName, T&& func)
    {
        auto itContext = _bindingsByContext.find(contextName);
        if (_bindingsByContext.end() == itContext)
            return;

        auto& context  = itContext->second;
        auto  itAction = context.find(actionName);
        if (context.end() == itAction)
            return;

        itAction->second.callback = std::forward<T>(func);
    }

    template <typename T>
    void BindAction(const std::string& contextName,
      const std::string&               actionName,
      T*                               obj,
      void (T::*memFunc)(Ptr<class InputAction>, ButtonEventType::Type))
    {
        auto itContext = _bindingsByContext.find(contextName);
        if (_bindingsByContext.end() == itContext)
            return;

        auto& context  = itContext->second;
        auto  itAction = context.find(actionName);
        if (context.end() == itAction)
            return;

        itAction->second.callback
          = std::bind(memFunc, obj, std::placeholders::_1, std::placeholders::_2);
    }
};
