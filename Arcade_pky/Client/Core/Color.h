#pragma once
struct Color
{
    uint8 r = 0;
    uint8 g = 0;
    uint8 b = 0;
    uint8 a = 0;

    Color() {}
    Color(uint8 r, uint8 g, uint8 b, uint8 a);
    Color(float r, float g, float b, float a);
    explicit Color(uint32 v);
    explicit Color(Vector4 v);

    operator uint32();
    operator Vector4();

    void SetR(float value);
    void SetG(float value);
    void SetB(float value);
    void SetA(float value);
};
