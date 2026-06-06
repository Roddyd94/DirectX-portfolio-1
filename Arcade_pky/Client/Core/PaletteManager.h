#pragma once
#include "SubManager.h"

class PaletteManager : public SubManager
{
public:
    PaletteManager()           = default;
    ~PaletteManager() override = default;
    DELETE_SPECIAL_FUNC(PaletteManager)

private:
    std::unordered_map<std::string, int32> _paletteFinder;
    std::map<int32, Ptr<class Palette>>    _palettes;

    int32 _idCounter = 0;

public:
    bool Init() override;
    void Destroy() override;

    Ptr<class Palette> FindPalette(const std::string& name);
    Ptr<class Palette> AddPalette(const std::string& name);

    void AddColor(const std::string& name, Color color);
};
