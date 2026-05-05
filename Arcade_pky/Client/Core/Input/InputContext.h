#pragma once
#include "Types.h"
#include "Core/Object.h"

struct InputMapping
{
    Ptr<class InputAction> action;
    bool                   buttonState[ButtonEventType::End] = {};
    uint8                  key                               = 0;
};

class InputContext : public Object
{
    friend class InputSystem;
    friend class InputComponent;

public:
    InputContext()           = default;
    ~InputContext() override = default;
    DELETE_SPECIAL_FUNC(InputContext)

private:
    std::string                                   _name;
    std::unordered_map<std::string, InputMapping> _mappings;

public:
    void Destroy() override;

    const std::string& GetName() const;
    InputMapping*      FindMapping(const std::string& name);

    void SetName(const std::string& name);
    void BindInputAction(Ptr<class InputAction> action, uint8 key);
    void ChangeInputActionKey(const std::string& actionName, uint8 key);
};
