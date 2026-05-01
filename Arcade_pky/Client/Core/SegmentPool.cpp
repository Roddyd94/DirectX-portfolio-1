#include "pch.h"

#ifdef _USE_MEMORY_POOL
#include "SegmentPool.h"

SegmentPool::SegmentPool(uint32 buffSize, uint32 bucketSize)
    : _buffSize(buffSize), _bucketSize(bucketSize)
{
    Create();
}

SegmentPool::~SegmentPool()
{
    for (auto* chunk : _chunks)
        delete[] chunk;
}

void SegmentPool::Create()
{
    byte* chunk = new byte[_buffSize * _bucketSize]();
    _chunks.push_back(chunk);

    for (int32 i = 0; i < _bucketSize; ++i)
    {
        MemorySegment seg;
        seg.data     = chunk + (i * _buffSize);
        seg.size     = _buffSize;
        seg.isPooled = true;

        _buckets.push(seg);
    }
}

MemorySegment SegmentPool::Rent()
{
    if (!_buckets.empty())
    {
        MemorySegment seg = _buckets.front();
        _buckets.pop();

        return seg;
    }

    MemorySegment seg;
    seg.data     = new byte[_buffSize]();
    seg.isPooled = false;
    seg.size     = _buffSize;

    return seg;
}

void SegmentPool::Return(MemorySegment& seg)
{
    if (!seg.isPooled)
    {
        delete[] seg.data;
        return;
    }

    _buckets.push(seg);
}
#endif // _USE_MEMORY_POOL
