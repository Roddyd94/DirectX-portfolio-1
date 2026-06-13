#include "pch.h"

#include "IndexedAnimation2DStructureBuffer.h"

void IndexedAnimation2DStructureBuffer::Update()
{
    if (_dataArray.empty())
        return;

    int32 count = GetElementCount();
    Resize(count);
    BindData(_dataArray.data(), count);
}

int32 IndexedAnimation2DStructureBuffer::GetElementCount() const
{
    return static_cast<int32>(_dataArray.size());
}

void IndexedAnimation2DStructureBuffer::Clear()
{
    _dataArray.clear();
}

void IndexedAnimation2DStructureBuffer::AddData(const IndexedAnimation2DInstanceData& data)
{
    _dataArray.push_back(data);
}

void IndexedAnimation2DStructureBuffer::AddData(IndexedAnimation2DInstanceData&& data)
{
    _dataArray.emplace_back(std::move(data));
}
