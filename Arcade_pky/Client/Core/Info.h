#pragma once

#define OUT

struct Resolution
{
    uint32 width;
    uint32 height;
};

struct ViewVolume
{
    float width;
    float height;
};

namespace AxisType
{
    enum Type : uint8
    {
        X,
        Y,
        Z,
        End
    };
}

Resolution defaultResolution = {1'024, 896};
ViewVolume defaultViewVolume = {16.f, 14.f};