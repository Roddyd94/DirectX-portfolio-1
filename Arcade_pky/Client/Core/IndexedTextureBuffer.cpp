#include "pch.h"

#include "IndexedTextureBuffer.h"

#include "Core/ResourceManager.h"

#include "IndexedTextureInfoConstantBuffer.h"

void IndexedTextureBuffer::Update()
{
    if (_dataArray.empty())
        return;

    int32 count = GetElementCount();
    Resize(count / 4);

    auto textureInfoBuffer
      = FIND_CONSTANT_BUFFER("IndexedTextureInfo", IndexedTextureInfoConstantBuffer);
    textureInfoBuffer->Update();

    SetData(_dataArray.data(), count / 4);
}

int32 IndexedTextureBuffer::GetElementCount() const
{
    return static_cast<int32>(_dataArray.size());
}

void IndexedTextureBuffer::Clear()
{
    _dataArray.clear();
}

void IndexedTextureBuffer::AddData(const char* data, size_t dataLength)
{
    size_t oldSize = _dataArray.size();

    _dataArray.resize(oldSize + dataLength);
    memcpy_s(&_dataArray[oldSize], dataLength, data, dataLength);
}

void IndexedTextureBuffer::SetSize(int32 width, int32 height)
{
    auto textureInfoBuffer
      = FIND_CONSTANT_BUFFER("IndexedTextureInfo", IndexedTextureInfoConstantBuffer);
    textureInfoBuffer->SetSize(width, height);
}

void IndexedTextureBuffer::SetStride(int32 strideBitSize)
{
    auto textureInfoBuffer
      = FIND_CONSTANT_BUFFER("IndexedTextureInfo", IndexedTextureInfoConstantBuffer);
    textureInfoBuffer->SetStride(strideBitSize);
}
