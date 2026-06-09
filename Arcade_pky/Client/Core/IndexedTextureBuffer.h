#pragma once
#include "Core/StructureBuffer.h"

class IndexedTextureBuffer : public StructureBuffer
{
public:
    IndexedTextureBuffer();
    ~IndexedTextureBuffer() override = default;

private:
    std::vector<byte> _dataArray;

public:
    void Update() override;

    int32 GetElementCount() const override;
    void  Clear() override;

    // 패딩을 추가하여 4 byte 단위로 넣을 것!
    void AddData(const char* data, size_t dataLength);

    // 패딩을 추가하여 width를 4의 배수로 맞출 것!
    void SetSize(int32 width, int32 height);
    void SetStride(int32 strideBitSize);
};
