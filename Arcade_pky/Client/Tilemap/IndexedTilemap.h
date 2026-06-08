#pragma once
#include "Tilemap.h"

class IndexedTilemap : public Tilemap
{
public:
    IndexedTilemap()           = default;
    ~IndexedTilemap() override = default;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;

    void SetTexture(Ptr<class IndexedTexture> texture);
    void SetTexture(const std::string& name);

    void SetPaletteSet(int32 number);
};
