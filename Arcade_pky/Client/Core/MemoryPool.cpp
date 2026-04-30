#include "pch.h"

#include "MemoryPool.h"

#ifdef _USE_MEMORY_POOL
#include <iostream>

bool MemoryPool::Init()
{
    uint32 pow = _defaultPow;
    _pools.push_back(
      new SegmentPool(static_cast<uint32>(std::pow(2, pow++)), 100)); // 16 Bytes
    _pools.push_back(
      new SegmentPool(static_cast<uint32>(std::pow(2, pow++)), 100)); // 32 Bytes
    _pools.push_back(
      new SegmentPool(static_cast<uint32>(std::pow(2, pow++)), 50)); // 64 Bytes
    _pools.push_back(
      new SegmentPool(static_cast<uint32>(std::pow(2, pow++)), 50)); // 128 Bytes
    _pools.push_back(
      new SegmentPool(static_cast<uint32>(std::pow(2, pow++)), 10)); // 256 Bytes
    _pools.push_back(
      new SegmentPool(static_cast<uint32>(std::pow(2, pow++)), 10)); // 512 Bytes

    return true;
}

void MemoryPool::Destroy()
{
    for (auto& [_, segment] : _rentals)
    {
        uint32       index = CalculateIndexFromSegmentSize(segment.size);
        SegmentPool* pool  = FindSegmentPool(index);
        pool->Return(segment);
    }

    _rentals.clear();

    for (auto pool : _pools)
        delete pool;
}

MemorySegment MemoryPool::Rent(uint32 size)
{
    uint32       index = CalculateIndexFromSegmentSize(size);
    SegmentPool* pool  = FindSegmentPool(index);

    if (!pool)
    {
        MemorySegment seg;
        seg.data     = new byte[size];
        seg.isPooled = false;
        seg.size     = size;

        return seg;
    }

    MemorySegment seg  = pool->Rent();
    _rentals[seg.data] = seg;

    return seg;
}

void MemoryPool::Return(MemorySegment& seg)
{
    if (!seg.isPooled)
    {
        delete[] seg.data;
        return;
    }

    uint32       index = CalculateIndexFromSegmentSize(seg.size);
    SegmentPool* pool  = FindSegmentPool(index);
    pool->Return(seg);
}

void MemoryPool::Return(void* memory)
{
    if (_rentals.end() == _rentals.find(memory))
    {
        delete memory;
        return;
    }

    uint32       index = CalculateIndexFromSegmentSize(_rentals[memory].size);
    SegmentPool* pool  = FindSegmentPool(index);
    pool->Return(_rentals[memory]);
    _rentals.erase(memory);
}

uint32 MemoryPool::CalculateIndexFromSegmentSize(uint32 size)
{
    uint32  log   = static_cast<int32>(std::ceil(std::log2(size)));
    uint32 index = log - _defaultPow;
    return index;
}

SegmentPool* MemoryPool::FindSegmentPool(uint32 index)
{
    if (index >= _pools.size())
        return nullptr;

    return _pools[index];
}
#endif // _USE_MEMORY_POOL
