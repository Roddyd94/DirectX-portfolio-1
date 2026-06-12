#include "pch.h"

#include "ScorePopup.h"

#include "Core/ResourceManager.h"
#include "Core/TimeManager.h"

#include "IndexedSpriteInstanceComponent.h"
#include "Core/Palette.h"

bool ScorePopup::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Actor::Init(id, position, scale, rotation);
    SetName("Score");
    _spriteInstance = CreateSceneComponent<IndexedSpriteInstanceComponent>("Sprite");

    _spriteInstance->SetRenderLayer("Score");
    _spriteInstance->AttachToComponent(_root);
    _spriteInstance->SetWorldScale({1.f, 1.f});

    Ptr<Animation2D> animation = _spriteInstance->CreateAnimation();
    animation->SetAnimationSequence("score");

    _timerID = TimeManager::Instance().SetTimer(2.f, false,
      [this]()
      {
          SetActive(false);
      });

    return true;
}

void ScorePopup::Destroy()
{
    Actor::Destroy();
    TimeManager::Instance().RemoveTimer(_timerID);
}

void ScorePopup::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);

    Vector3 position = GetWorldPosition();

    position.y += _velocity * deltaTime;
    _velocity *= 1.f - 2.f * deltaTime;

    SetWorldPosition(position);
}

void ScorePopup::SetInfo(ScorePopup::Type scoreType, int32 index)
{
    int32 paletteNumber = FIND_PALETTE("item_sushi")->GetID();

    switch (scoreType)
    {
    case ScorePopup::General:
        _spriteInstance->ChangeAnimation("score_enemy");
        break;
    case ScorePopup::Sushi:
        _spriteInstance->ChangeAnimation("score_item");
        break;
    case ScorePopup::Envelope:
    {
        _spriteInstance->ChangeAnimation("player_stand");
        paletteNumber = FIND_PALETTE("player_2")->GetID();
        _spriteInstance->SetWorldScale({2.f, 2.f});
    }
    break;
    }

    _spriteInstance->Pause();
    _spriteInstance->SetPaletteNumber(paletteNumber);
    _spriteInstance->SetFrameIndex(index);
}
