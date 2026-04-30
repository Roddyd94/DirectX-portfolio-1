#pragma once

#ifdef _USE_MEMORY_POOL
#include "SegmentPool.h"
#include "Singleton.h"

#include <unordered_map>
#include <vector>

class MemoryPool : public Singleton<MemoryPool>
{
    DECLARE_SINGLETON(MemoryPool)

private:
    inline static const uint32 _defaultPow = 4;

private:
    std::unordered_map<void*, MemorySegment> _rentals;
    std::vector<class SegmentPool*>          _pools;

public:
    bool Init();
    void Destroy();

    MemorySegment Rent(uint32 size);
    void          Return(MemorySegment& seg);
    void          Return(void* memory);

private:
    uint32       CalculateIndexFromSegmentSize(uint32 size);
    SegmentPool* FindSegmentPool(uint32 index);
};
#endif // _USE_MEMORY_POOL
