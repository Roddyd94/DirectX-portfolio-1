#include "pch.h"

#include "SpriteComponent.h"

#include "Core/ResourceManager.h"

#include "Core/Level.h"
#include "Core/Mesh.h"
#include "Core/TransformConstantBuffer.h"

SpriteComponent::SpriteComponent()
{
    _shouldRender = true;
}

SpriteComponent::~SpriteComponent() {}

bool SpriteComponent::Init(int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    SceneComponent::Init(componentID, name, owner);

    SetShader("SpriteShader");
    _mesh = MESH_TEXTURE_RECT;

    return true;
}

void SpriteComponent::Destroy()
{
    SceneComponent::Destroy();
    DESTROY(_animation);
}

void SpriteComponent::Tick(float deltaTime)
{
    SceneComponent::Tick(deltaTime);

    if (_animation)
        _animation->Tick(deltaTime);
}

void SpriteComponent::Collision(float deltaTime)
{
    SceneComponent::Collision(deltaTime);
}

void SpriteComponent::Render(float deltaTime)
{
    SceneComponent::Render(deltaTime);

    _animation->SetShader();

    SHADER_MANAGER->SetSampler(SamplerType::Point);

    Ptr<TransformConstantBuffer> cBuffer = Lock(_transformConstantBuffer);
    cBuffer->SetWorldMatrix(_matrix.world);

    Ptr<Level> level = Lock<Level>(_level);
    if (nullptr == level)
        return;

    Matrix view = level->GetViewMatrix();
    Matrix proj = level->GetProjMatrix();

    cBuffer->SetViewMatrix(view);
    cBuffer->SetProjMatrix(proj);

    cBuffer->Update();

    _shader->SetShader();

    _mesh->Render();
}

void SpriteComponent::SetShader(const std::string& name)
{
    _shader = FIND_SHADER(name, Shader);
}

void SpriteComponent::SetPlayRate(float playRate)
{
    _animation->SetPlayRate(playRate);
}

void SpriteComponent::SetReversed(bool isReversed)
{
    _animation->SetReversed(isReversed);
}

void SpriteComponent::SetFlipX(bool flipX)
{
    _animation->SetFlipX(flipX);
}

void SpriteComponent::Play()
{
    _animation->Play();
}

void SpriteComponent::Pause()
{
    _animation->Pause();
}

void SpriteComponent::Stop()
{
    _animation->Stop();
}

void SpriteComponent::ChangeAnimation(const std::string& name, bool play)
{
    _animation->ChangeAnimationClip(name, play);
}

Ptr<class Animation2D> SpriteComponent::CreateAnimation()
{
    _animation         = New<Animation2D>();
    _animation->_owner = This<SpriteComponent>();

    if (!_animation->Init())
    {
        DESTROY(_animation);
        return nullptr;
    }

    return _animation;
}

Ptr<class Animation2D> SpriteComponent::GetAnimation()
{
    return _animation;
}
