#pragma once
#include "Singleton.h"

class GameEngine : public Singleton<GameEngine>
{
    friend class EditorEngine;

    DECLARE_SINGLETON(GameEngine)
private:
    Ptr<class World> _world = nullptr;
    Ptr<class Input> _input = nullptr;

    float _clearColor[4] = {1.f, 1.f, 1.f, 1.f};

public:
    int  Init();
    void Destroy();

    void             Logic();
    Ptr<class World> GetWorld() const;

private:
    void Tick(float deltaTime);
    void Collision(float deltaTime);
    void Render(float deltaTime);

    bool InitManagers();
};
