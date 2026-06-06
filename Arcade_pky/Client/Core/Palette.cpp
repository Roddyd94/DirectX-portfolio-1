#include "pch.h"

#include "Palette.h"

void Palette::Destroy() {}

Color Palette::GetColor(int32 index) const
{
    if (index < 0)
        return _colors[0];

    if (index >= _colors.size())
        _colors[_colors.size() - 1];

    return _colors[index];
}

int32 Palette::GetPaletteLength() const
{
    return _colors.size();
}

void Palette::AddColor(Color color)
{
    _colors.emplace_back(color);
}
