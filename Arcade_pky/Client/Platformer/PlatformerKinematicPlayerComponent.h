#pragma once
#include "PlatformerKinematicComponent.h"

class PlatformerKinematicPlayerComponent : public PlatformerKinematicComponent
{
public:
    PlatformerKinematicPlayerComponent()           = default;
    ~PlatformerKinematicPlayerComponent() override = default;

private:
    std::function<void()>          _onStateChangedTo[PlatformerKinematicState::End];
    PlatformerKinematicState::Type _state = PlatformerKinematicState::OnAir;
    
public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    void AttachTo(Ptr<class Actor> actor) override;
    
    void ChangeStateTo(PlatformerKinematicState::Type state);

public:
    template <typename T>
    void RegisterOnStateChangedCallback(
      PlatformerKinematicState::Type state, T* obj, void (T::*memFunc)())
    {
        _onStateChangedTo[state] = std::bind(memFunc, obj);
    }

    template <typename T>
    void RegisterOnStateChangedCallback(PlatformerKinematicState::Type state, T&& func)
    {
        _onStateChangedTo[state] = std::forward<T>(func);
    }
};
