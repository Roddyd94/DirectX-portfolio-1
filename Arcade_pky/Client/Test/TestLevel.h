#pragma once
#include "Core/Level.h"

class TestLevel : public Level
{
public:
    bool Init(Ptr<class World> world, const std::string& path) override;
};
