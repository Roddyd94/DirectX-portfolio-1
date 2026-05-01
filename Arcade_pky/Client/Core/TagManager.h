#pragma once
#include "SubManager.h"

class TagManager : public SubManager
{
public:
    TagManager()           = default;
    ~TagManager() override = default;
    DELETE_SPECIAL_FUNC(TagManager)

protected:
    std::unordered_map<std::string, std::set<int32>> _actorIDFinder;

public:
    bool Init() override;
    void Destroy() override;

    void GetActorIDs(const std::string& tag, OUT std::vector<int32>& outArr);

    void Add(const std::string& tag, int32 id);
    void Erase(const std::string& tag, int32 id);
    void Clear();
};
