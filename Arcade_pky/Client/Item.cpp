#include "pch.h"

#include "Item.h"

#include "Core/ResourceManager.h"
#include "Core/TimeManager.h"

#include "Core/Animation/Animation2D.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Core/Palette.h"
#include "Snowbros/IndexedSpriteInstanceComponent.h"

std::vector<int> itemValues = {
  10'000, // envelope
  2'000,  // sushi 1
  1'500,  // sushi 2
  1'200,  // sushi 3
  1'000,  // sushi 4
  800,    // sushi 5
  600,    // sushi 6
  400,    // sushi 7
  300,    // sushi 8
  200,    // sushi 9
  100     // sushi 10
};

bool Item::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Actor::Init(id, position, scale, rotation);
    SetName("Item");

    auto sprite = CreateSceneComponent<SpriteInstanceComponent>("Root");
    sprite->SetRenderLayer("Item");

    SetRoot(sprite);

    auto animation = sprite->CreateAnimation();
    animation->SetAnimationSequence("item");
    animation->ChangeAnimationClip("item_none");

    auto collider = CreateSceneComponent<AABBCollisionComponent>("Collider");
    collider->AttachToComponent(_root);
    collider->SetBoxSize({0.6f, 0.6f});
    collider->SetCollisionProfile("Item");
    collider->RegisterCollisionCallBack(CollisionState::Enter,
      [this](Weak<CollisionComponent> collider)
      {
          auto otherCollider = Lock(collider);

          if (ColliderType::Player == otherCollider->GetColliderType())
              this->SetActive(false);
      });

    _itemTimer = TimeManager::Instance().SetTimer(7.f, false,
      [this]()
      {
          this->SetActive(false);
      });

    return true;
}

void Item::Destroy()
{
    Actor::Destroy();

    TimeManager::Instance().RemoveTimer(_itemTimer);
}

void Item::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Item::Collision(float deltaTime)
{
    Actor::Collision(deltaTime);
}

Item::Type Item::GetItemType() const
{
    return _itemType;
}

int32 Item::GetItemValue() const
{
    return itemValues[_itemNumber];
}

int32 Item::GetItemNumber() const
{
    return _itemNumber;
}

void Item::SetItemType(Type itemType)
{
    _itemType = itemType;

    auto sprite = FindSceneComponent<SpriteInstanceComponent>("Root");
    switch (_itemType)
    {
    case Item::Speed:
    {
        sprite->ChangeAnimation("item_speed");
        auto palette = FIND_PALETTE("item_upgrade");
        //sprite->SetPaletteNumber(palette->GetID());
    }
    break;
    case Item::Power:
    {
        sprite->ChangeAnimation("item_power");
        auto palette = FIND_PALETTE("item_upgrade");
        //sprite->SetPaletteNumber(palette->GetID());
    }
    break;
    case Item::Range:
    {
        sprite->ChangeAnimation("item_range");
        auto palette = FIND_PALETTE("item_upgrade");
        //sprite->SetPaletteNumber(palette->GetID());
    }
    break;
    case Item::Invincibility:
    {
        sprite->ChangeAnimation("item_invincibility");
        auto palette = FIND_PALETTE("item_upgrade");
        //sprite->SetPaletteNumber(palette->GetID());
    }
    break;
    case Item::Sushi:
    {
        sprite->ChangeAnimation("item_sushi");
        auto palette = FIND_PALETTE("item_sushi");
        //sprite->SetPaletteNumber(palette->GetID());
        sprite->Pause();
    }
    break;
    case Item::Envelope:
    {
        sprite->SetWorldScale(2 * Vector2::one);
        sprite->ChangeAnimation("item_envelope");
        auto palette = FIND_PALETTE("player_1");
        //sprite->SetPaletteNumber(palette->GetID());
    }
    break;
    case Item::Special:
    {
        sprite->ChangeAnimation("item_upgrade");
        auto palette = FIND_PALETTE("player_1");
        //sprite->SetPaletteNumber(palette->GetID());
    }
    break;
    }
}

void Item::SetItemNumber(int32 itemNumber)
{
    _itemNumber = std::clamp(itemNumber, 0, 10);

    auto sprite = FindSceneComponent<SpriteInstanceComponent>("Root");
    sprite->Pause();

    if (Item::Sushi == _itemType)
        sprite->SetFrameIndex(itemNumber - 1);
}
