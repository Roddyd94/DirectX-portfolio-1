#include "pch.h"

#include "SnowbrosUI.h"

#include "SnowbrosUIComponent.h"

bool SnowbrosUI::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Actor::Init(id, position, scale, rotation);

    _uiComponent = CreateSceneComponent<SnowbrosUIComponent>("UI");
    _uiComponent->AttachToComponent(_root);

    return true;
}

void SnowbrosUI::Destroy()
{
    Actor::Destroy();
}

void SnowbrosUI::SetScore(ScoreType::Type player, int32 score)
{
    _uiComponent->SetScore(player, score);
}
