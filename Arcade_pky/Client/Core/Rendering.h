#pragma once

struct TextureVertex
{
    Vector3 pos;
    Vector2 uv;

    TextureVertex(Vector3 pos, Vector2 uv) : pos(pos), uv(uv) {}

    TextureVertex(float x, float y, float z, float u, float v) : pos(x, y, z), uv(u, v) {}
};

struct SpriteData
{
    Vector2 start;
    Vector2 size;
    // Vector2 pivot;
};

namespace ShaderType
{
    enum Type : uint8
    {
        Vertex  = 0x01,
        Pixel   = 0x02,
        Graphic = Vertex | Pixel,
        End
    };
}

namespace SamplerType
{
    enum Type : uint8
    {
        Point,
        Linear,
        Anisotropic,
        End
    };
}
