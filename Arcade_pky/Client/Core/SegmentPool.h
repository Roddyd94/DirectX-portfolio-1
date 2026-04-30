#pragma once

#ifdef _USE_MEMORY_POOL
struct MemorySegment
{
    byte*  data     = nullptr;
    uint32 size     = 0;
    bool   isPooled = false;
};

class SegmentPool
{
    friend class MemoryPool;

private:
    SegmentPool(uint32 buffSize, uint32 bucketSize);
    ~SegmentPool();
    SegmentPool(const SegmentPool&)            = delete;
    SegmentPool(SegmentPool&&)                 = delete;
    SegmentPool& operator=(const SegmentPool&) = delete;
    SegmentPool& operator=(SegmentPool&&)      = delete;

private:
    std::queue<MemorySegment> _buckets;
    std::vector<byte*>        _chunks;

    uint32 _buffSize   = 0;
    uint32 _bucketSize = 0;

private:
    void Create();

public:
    MemorySegment Rent();
    void          Return(MemorySegment& seg);
};
#endif // _USE_MEMORY_POOL
