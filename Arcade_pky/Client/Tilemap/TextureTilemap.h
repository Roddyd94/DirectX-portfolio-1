#pragma once
#include "Tilemap.h"

class TextureTilemap : public Tilemap
{
public:
    TextureTilemap()           = default;
    ~TextureTilemap() override = default;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;

    void SetTexture(Ptr<class Texture> texture);
    void SetTexture(const std::string& name);
    void SetTexture(const std::string& name, const std::wstring& fileName);
};
