#pragma once
#include "Types.h"
#include "Core/ActorComponent.h"

struct BindingAction
{
    std::string                name;
    struct InputMapping*       mapping = nullptr;
    std::function<void(float)> callback[ButtonEventType::End];
};

class InputComponent : public ActorComponent
{
public:
    InputComponent()           = default;
    ~InputComponent() override = default;

private:
    std::unordered_map<std::string, std::unordered_map<std::string, BindingAction>> _binds;
    std::string                                                                  _activeContext;

public:
    bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    const std::string& GetActiveContext() const;

    void SetActiveContext(const std::string& name);
    void AddInputContext(const std::string& name);
    void RemoveInputContext(const std::string& name);
    void RefreshInputMapping(const std::string& contextName, const struct InputMapping& mapping);

    template <typename T>
    void BindAction(const std::string& contextName,
      const std::string&               actionName,
      ButtonEventType::Type            eventType,
      T&&                              func)
    {
        auto it = _binds.find(contextName);
        if (_binds.end() == it)
            return;

        _binds[contextName][actionName].callback[eventType] = std::forward<T>(func);
    }

    template <typename T>
    void BindAction(const std::string& contextName,
      const std::string&               actionName,
      ButtonEventType ::Type           eventType,
      T*                               obj,
      void (T::*memFunc)(float))
    {
        auto it = _binds.find(contextName);
        if (_binds.end() == it)
            return;

        _binds[contextName][actionName].callback[eventType]
          = std::bind(memFunc, obj, std::placeholders::_1);
    }
};
