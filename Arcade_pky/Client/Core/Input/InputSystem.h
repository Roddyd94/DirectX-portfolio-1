#pragma once
#include "Input.h"
#include "Types.h"
#include "Core/Singleton.h"

class InputSystem : public Singleton<InputSystem>
{
    DECLARE_SINGLETON(InputSystem)
private:
    std::unordered_map<std::string, Ptr<class InputContext>> _allContexts;
    std::unordered_map<std::string, Ptr<class InputAction>>  _allActions;
    std::unordered_map<std::string, Ptr<class InputContext>> _activeContexts;

    std::map<std::pair<int32, int32>, Weak<class InputComponent>> _inputComponents;

    Ptr<class Input> _input;

public:
    bool Init(Ptr<class Input> input);
    void Destroy();

    void Tick(float deltaTime);

    Vector2 GetMouseWorldPos() const;
    Vector2 GetMousePos() const;
    Vector2 GetMouseMove() const;

    bool GetMouseEvent(uint16 mouseButton, uint16 buttonEvent) const;

    bool IsActiveContext(const std::string& name) const;

    Ptr<class InputContext> FindOrAddInputContext(const std::string& name);
    Ptr<class InputAction>  FindOrAddInputAction(const std::string& name);

    void AddActiveInputContext(Ptr<class InputContext> context);
    void RemoveActiveContext(const std::string& name);

    void RegisterInputComponent(int32 actorID, Ptr<class InputComponent> inputComponent);
    void RemoveInputComponent(int32 actorID, int32 componentID);
    void RefreshInputComponent(const std::string& contextName, const struct InputMapping& mapping);

    uint8 ConvertKey(uint8 key);
};
