#include "pch.h"

#include "PaletteStructureBuffer.h"

#include "Palette.h"

void PaletteStructureBuffer::Update()
{
    if (_dataArray.empty())
        return;

    int32 count = GetElementCount();
    Resize(count);
    BindData(_dataArray.data(), count);
}

int32 PaletteStructureBuffer::GetElementCount() const
{
    return static_cast<int32>(_dataArray.size());
}

int32 PaletteStructureBuffer::GetPaletteCount() const
{
    return _dataArray.size() / _paletteLength;
}

int32 PaletteStructureBuffer::GetPaletteLength() const
{
    return _paletteLength;
}

void PaletteStructureBuffer::Clear()
{
    _dataArray.clear();
    _paletteLength = 0;
}

void PaletteStructureBuffer::AddData(Ptr<class Palette> palette)
{
    for (int i = 0; i < _paletteLength; ++i)
        _dataArray.push_back(palette->GetColor(i));
}

void PaletteStructureBuffer::SetPaletteLength(int32 length)
{
    _paletteLength = length;
}
