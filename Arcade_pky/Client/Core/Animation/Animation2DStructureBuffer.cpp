#include "pch.h"

#include "Animation2DStructureBuffer.h"

void Animation2DStructureBuffer::Update()
{
    if (_dataArray.empty())
        return;

    int32 count = GetElementCount();
    Resize(count);
    SetData(_dataArray.data(), count);
}

int32 Animation2DStructureBuffer::GetElementCount() const
{
    return static_cast<int32>(_dataArray.size());
}

void Animation2DStructureBuffer::Clear()
{
    _dataArray.clear();
}

void Animation2DStructureBuffer::AddData(Animation2DInstanceData&& data)
{
    _dataArray.emplace_back(std::move(data));
}
