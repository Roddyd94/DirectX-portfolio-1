#pragma once
#include "Core/Actor.h"

class Item : public Actor
{
public:
    enum Type;

public:
    Item()           = default;
    ~Item() override = default;

protected:
    Type  _itemType   = End;
    int32 _itemNumber = -1;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;

    Type  GetItemType() const;
    int32 GetItemValue() const;
    int32 GetItemNumber() const;

    void SetItemType(Type itemType);
    void SetItemNumber(int32 itemNumber);

public:
    enum Type
    {
        Speed,
        Power,
        Range,
        Invincibility,
        Sushi,
        Envelope,
        Special,
        End
    };
};
