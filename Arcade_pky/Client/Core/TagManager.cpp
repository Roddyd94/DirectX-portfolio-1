#include "pch.h"

#include "TagManager.h"

bool TagManager::Init()
{
    Clear();
    return true;
}

void TagManager::Destroy()
{
    Clear();
}

void TagManager::GetActorIDs(
  const std::string& tag, OUT std::vector<int32>& outArr)
{
    auto it = _actorIDFinder.find(tag);
    if (it == _actorIDFinder.end())
        return;

    auto& actorIDs = it->second;
    if (actorIDs.empty())
        return;

    outArr.clear();
    for (auto& actorID : actorIDs)
        outArr.push_back(actorID);
}

void TagManager::Add(const std::string& tag, int32 id)
{
    _actorIDFinder[tag].insert(id);
}

void TagManager::Erase(const std::string& tag, int32 id)
{
    auto it = _actorIDFinder.find(tag);
    if (_actorIDFinder.end() == it)
        return;

    auto& actorIDs = it->second;
    actorIDs.erase(id);
}

void TagManager::Clear()
{
    if (_actorIDFinder.empty())
        return;

    for (auto& [_, actorIDs] : _actorIDFinder)
    {
        if (actorIDs.empty())
            continue;

        actorIDs.clear();
    }

    _actorIDFinder.clear();
}
