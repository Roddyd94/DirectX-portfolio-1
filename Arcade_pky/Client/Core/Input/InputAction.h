#pragma once
#include "Core/Object.h"

class InputAction : public Object
{
    friend class InputSystem;

public:
    InputAction()           = default;
    ~InputAction() override = default;
    DELETE_SPECIAL_FUNC(InputAction)

private:
    std::string _name;

public:
    void Destroy() override;

    const std::string& GetName() const;
    void               SetName(const std::string& name);
};
