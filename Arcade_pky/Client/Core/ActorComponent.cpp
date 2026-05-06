#include "pch.h"
#include "ActorComponent.h"

bool ActorComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    return Component::Init(componentID, name, owner);
}

void ActorComponent::Destroy() {
    Component::Destroy();
}

void ActorComponent::Tick(float deltaTime) {
}
