#pragma once
#include "Resource.h"

class Palette : public Resource
{
public:
    Palette()           = default;
    ~Palette() override = default;

protected:
    std::vector<Color> _colors;

public:
    void Destroy() override;

    Color GetColor(int32 index) const;
    int32 GetPaletteLength() const;

    void  AddColor(Color color);
};
