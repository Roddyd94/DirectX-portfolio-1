#pragma once
#include "StructureBuffer.h"

class PaletteStructureBuffer : public StructureBuffer
{
public:
    PaletteStructureBuffer()           = default;
    ~PaletteStructureBuffer() override = default;
    
private:
    std::vector<Vector4> _dataArray;

    int32 _paletteLength = 0;

public:
    void Update() override;

    int32 GetElementCount() const override;
    int32 GetPaletteCount() const;
    int32 GetPaletteLength() const;

    void  Clear() override;

    void AddData(Ptr<class Palette> palette);
    void SetPaletteLength(int32 length);
};
