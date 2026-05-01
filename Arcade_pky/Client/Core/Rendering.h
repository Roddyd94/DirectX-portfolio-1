#pragma once

struct TextureVertex
{
    Vector3 pos;
    Vector2 uv;

    TextureVertex(Vector3 pos, Vector2 uv) : pos(pos), uv(uv) {}

    TextureVertex(float x, float y, float z, float u, float v)
        : pos(x, y, z), uv(u, v)
    {
    }
};

struct TextureFrameData
{
    Vector2 start;
    Vector2 size;
};

struct AnimationFrameData
{
    TextureFrameData frame;
    Vector2          pivot;
    Vector2          meshSize;
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

enum class AnimationTextureType // TODO Animation 헤더로?
{
    Sprite,
    Frame,
    End
};
