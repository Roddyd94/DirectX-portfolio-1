#include "pch.h"

#include "PaletteManager.h"

#include "Palette.h"

bool PaletteManager::Init()
{
    return true;
}

void PaletteManager::Destroy()
{
    for (auto& [_, palette] : _palettes)
        DESTROY(palette)

    _palettes.clear();
    _paletteFinder.clear();
}

Ptr<Palette> PaletteManager::FindPalette(const std::string& name)
{
    auto itFinder = _paletteFinder.find(name);
    if (_paletteFinder.end() == itFinder)
        return nullptr;

    int32 id        = itFinder->second;
    auto  itTexture = _palettes.find(id);
    if (_palettes.end() == itTexture)
        return nullptr;

    return itTexture->second;
}

Ptr<class Palette> PaletteManager::AddPalette(const std::string& name)
{
    Ptr<Palette> palette = FindPalette(name);
    if (palette)
        return palette;

    palette = New<Palette>();

    palette->SetName(name);
    palette->SetID(_idCounter);
    _palettes[_idCounter] = palette;
    _paletteFinder[name]  = _idCounter;
    _idCounter++;
    
    return palette;
}

void PaletteManager::AddColor(const std::string& name, Color color)
{
    Ptr<Palette> palette = FindPalette(name);
    if (nullptr == palette)
        return;

    palette->AddColor(color);
}
