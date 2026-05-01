#include "pch.h"

#include "GameEngine.h"

#include "DeviceManager.h"
#include "DirectoryManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "TimeManager.h"
#include "WindowManager.h"
#include "Common/LogManager.h"

#include "Types.h" // TODO Remove later
#include "World.h"
#include "Common/Random.h" // TODO Remove later

#ifdef _USE_MEMORY_POOL
#include "MemoryPool.h"
#elif _USE_OBJECT_POOL
#include "ObjectPool.h"
#endif

int GameEngine::Init()
{
    WindowsManager::_isRunning = InitManagers();

    if (WindowsManager::_isRunning)
    {
        _world = New<World>();
        _world->Init("Default");
    }

    return WindowsManager::Instance().Run();
}

void GameEngine::Destroy()
{
    if (_world)
        _world->Destroy();
    ResourceManager::Instance().Destroy();

#ifdef _USE_MEMORY_POOL
    MemoryPool::Instance().Destroy();
#elif _USE_OBJECT_POOL
    ObjectPool::Instance().Destroy();
#endif

    //_input->Destroy();
    // InputSystem::Instance().Destroy();

    LogManager::Instance().Destroy();
    DirectoryManager::Instance().Destroy();
}

void GameEngine::Logic()
{
    float deltaTime = TimeManager::Instance().Tick();
    Tick(deltaTime);
    Collision(deltaTime);
    Render(deltaTime);
}

Ptr<class World> GameEngine::GetWorld() const
{
    return _world;
}

void GameEngine::Tick(float deltaTime)
{
    //_input->Tick(deltaTime);
    // InputSystem::Instance().Tick(deltaTime);
    // 상태를 갱신(down? hold? up?)

    SOUND_MANAGER->Tick();

    _world->Tick(deltaTime);
    RenderManager::Instance().Tick(deltaTime);
}

void GameEngine::Collision(float deltaTime)
{
    _world->Collision(deltaTime);
}

void GameEngine::Render(float deltaTime)
{
    DeviceManager::Instance().ClearBackBuffer(_clearColor);
    DeviceManager::Instance().ClearDepthStencil(1.f, 0);
    DeviceManager::Instance().SetTarget();

    // DeviceManager::Instance().GetDefaultRenderTarget2D()->BeginDraw();
    //_world->Render(deltaTime);
    RenderManager::Instance().Render(deltaTime);
    //_world->RenderUI(deltaTime);
    // DeviceManager::Instance().GetDefaultRenderTarget2D()->EndDraw();

    DeviceManager::Instance().Render();
}

bool GameEngine::InitManagers()
{
    DirectoryManager::Instance().Init();
    LogManager::Instance().Init();

#ifdef _USE_MEMORY_POOL
    if (!MemoryPool::Instance().Init())
        return false;
#elif defined(_USE_OBJECT_POOL)
    if (!ObjectPool::Instance().Init())
        return false;
#endif

    if (!TimeManager::Instance().Init())
        return false;

    uint32 width  = defaultResolution.width;
    uint32 height = defaultResolution.height;
    auto   hWnd   = WindowsManager::Instance().GetHWND();
    if (!DeviceManager::Instance().Init(hWnd, width, height, true))
        return false;

    if (!ResourceManager::Instance().Init())
        return false;

    //_input = New<Input>();
    //_input->Init();

    // if (!InputSystem::Instance().Init(_input))
    //     return false;

    if (!RenderManager::Instance().Init())
        return false;

    return true;
}
