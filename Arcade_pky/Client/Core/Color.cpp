#include "pch.h"

#include "Color.h"

Color::Color(uint8 r, uint8 g, uint8 b, uint8 a) : r(r), g(g), b(b), a(a) {}

Color::Color(float r, float g, float b, float a)
    : r(static_cast<uint8>(std::clamp(r, 0.f, 1.f) * 255.f)),
      g(static_cast<uint8>(std::clamp(g, 0.f, 1.f) * 255.f)),
      b(static_cast<uint8>(std::clamp(b, 0.f, 1.f) * 255.f)),
      a(static_cast<uint8>(std::clamp(a, 0.f, 1.f) * 255.f))
{
}

Color::Color(uint32 v)
{
    r = v >> 0x18;
    g = (v >> 0x10) & 0xFF;
    b = (v >> 0x08) & 0xFF;
    a = v & 0xFF;
}

Color::Color(Vector4 v) : Color(v.r, v.g, v.b, v.a) {}

Color::operator uint32()
{
    uint32 result = r;
    result <<= 8;
    result |= g;
    result <<= 8;
    result |= b;
    result <<= 8;
    result |= a;
    return result;
}

Color::operator Vector4()
{
    return Vector4{r / 255.f, g / 255.f, b / 255.f, a / 255.f};
}

void Color::SetR(float value)
{
    r = static_cast<uint8>(std::clamp(value, 0.f, 1.f) * 255.f);
}

void Color::SetG(float value)
{
    g = static_cast<uint8>(std::clamp(value, 0.f, 1.f) * 255.f);
}

void Color::SetB(float value)
{
    b = static_cast<uint8>(std::clamp(value, 0.f, 1.f) * 255.f);
}

void Color::SetA(float value)
{
    a = static_cast<uint8>(std::clamp(value, 0.f, 1.f) * 255.f);
}
