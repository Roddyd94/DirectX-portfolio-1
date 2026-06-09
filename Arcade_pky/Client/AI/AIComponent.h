#pragma once
#include "AIStateMachine.h"
#include "Core/ActorComponent.h"

class AIComponent : public ActorComponent
{
public:
    AIComponent()           = default;
    ~AIComponent() override = default;

protected:
    Weak<class AIController> _controller;
    Ptr<AIStateMachine>      _stateMachine;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    Ptr<class Pawn>         GetPawn() const;
    Ptr<class AIController> GetController() const;
    Ptr<AIStateMachine>     GetAIStateMachine();

    void SetController(Ptr<class AIController> controller);

public:
    template <typename T>
    Ptr<T> GetPawn() const
    {
        return Cast<Pawn, T>(GetPawn());
    }

    template <typename T>
    Ptr<T> GetBlackboard() const
    {
        return _stateMachine->GetBlackboard<T>();
    }

    template <typename T>
    Ptr<T> CreateAIStateMachine()
    {
        if (_stateMachine)
            return Cast<AIStateMachine, T>(_stateMachine);

        auto stateMachine = New<T>();

        Ptr<AIComponent> owner = This<AIComponent>();
        stateMachine->Init(owner);
        _stateMachine = stateMachine;

        return stateMachine;
    }
};
