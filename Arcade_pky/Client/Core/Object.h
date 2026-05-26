#pragma once
#include "Defines.h"

#ifdef _USE_MEMORY_POOL
#include "MemoryPool.h"

class Object
{
    friend class ObjectPool;

public:
    Object()          = default;
    virtual ~Object() = default;

protected:
    bool          _isActive  = false;
    bool          _isEnabled = false;
    MemorySegment _segment;

public:
    virtual void Destroy() = 0;

    bool IsActive() const { return _isActive; }
    bool IsEnable() const { return _isEnabled; }
    void SetActive(bool active) { _isActive = active; }
    void SetEnable(bool enable) { _isEnabled = enable; }

    void* operator new(size_t size)
    {
        MemorySegment seg = MemoryPool::Instance().Rent((uint32)size);
        return seg.data;
    }

    void operator delete(void* ptr) noexcept
    {
        MemoryPool::Instance().Return(ptr);
    }

    template <typename T>
    Ptr<T> This()
    {
        return Cast<Object, T>(this);
    }
};
#elif _USE_OBJECT_POOL
enum class ObjectType
{
    END
};

class Object
{
    friend class ObjectPool;

public:
    Object()          = default;
    virtual ~Object() = default;

protected:
    bool       _isActive  = false;
    bool       _isEnabled = false;
    bool       _isPooled  = false;
    ObjectType _type      = ObjectType::END;

public:
    virtual void Destroy() = 0;

    bool       IsActive() const { return _isActive; }
    bool       IsEnable() const { return _isEnabled; }
    void       SetActive(bool active) { _isActive = active; }
    void       SetEnable(bool enable) { _isEnabled = enable; }
    ObjectType GetType() const { return _type; }

    void Pool() { _isPooled = true; }

    template <typename T>
    Ptr<T> This()
    {
        return Cast<Object, T>(this);
    }
};
#else
#include <memory>
class Object : public std::enable_shared_from_this<Object>
{
public:
    Object() = default;
    virtual ~Object() = default;

protected:
    bool _isActive  = false;
    bool _isEnabled = false;

public:
    bool IsActive() const { return _isActive; }
    bool IsEnable() const { return _isEnabled; }
    void SetActive(bool active) { _isActive = active; }
    virtual void SetEnable(bool enable) { _isEnabled = enable; }

    virtual void Destroy() = 0;

    template <typename T>
    Ptr<T> This()
    {
        return Cast<Object, T>(shared_from_this());
    }
};
#endif
