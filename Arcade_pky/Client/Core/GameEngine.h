#pragma once
#include "Singleton.h"

class GameEngine : public Singleton<GameEngine>
{
#ifdef _DEBUG
    friend class EditorEngine;
#endif // _DEBUG

    DECLARE_SINGLETON(GameEngine)
private:
    Ptr<class World> _world;
    Ptr<class Input> _input;

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
