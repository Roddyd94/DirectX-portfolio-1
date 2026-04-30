#pragma once
#ifdef _USE_MEMORY_POOL

template <typename T>
using Ptr = T*;

template <typename T>
using Weak = T*;

template <typename T, typename... Args>
Ptr<T> New(Args&&... args)
{
    return new T(std::forward<Args>(args)...);
}

template <typename T>
void Delete(Ptr<T>& ptr)
{
    delete ptr;
    ptr = nullptr;
}

template <typename Src, typename Dest>
Ptr<Dest> Cast(Ptr<Src> src)
{
    return dynamic_cast<Ptr<Dest>>(src);
}

template <typename T>
Ptr<T> Lock(Weak<T> ptr)
{
    return ptr;
}
#elif _USE_OBJECT_POOL
#include "ObjectPool.h"

template <typename T>
using Ptr = T*;

template <typename T>
using Weak = T*;

template <typename T, typename... Args>
Ptr<T> New(Args&&... args)
{
    return ObjectPool::Instance().Rent<T>();
}

template <typename T>
void Delete(Ptr<T>& ptr)
{
    ObjectPool::Instance().Return(ptr);
    ptr = nullptr;
}

template <typename Src, typename Dest>
Ptr<Dest> Cast(Ptr<Src> src)
{
    return dynamic_cast<Ptr<Dest>>(src);
}

template <typename T>
Ptr<T> Lock(Weak<T> ptr)
{
    return ptr;
}
#else
template <typename T>
using Ptr = std::shared_ptr<T>;

template <typename T>
using Weak = std::weak_ptr<T>;

template <typename T, typename... Args>
Ptr<T> New(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
void Delete(Ptr<T>& ptr)
{
    ptr = nullptr;
}

template <typename Src, typename Dest>
Ptr<Dest> Cast(Ptr<Src> src)
{
    return std::dynamic_pointer_cast<Dest>(src);
}

template <typename T>
Ptr<T> Lock(Weak<T> ptr)
{
    return ptr.lock();
}
#endif

template <typename T>
Weak<T> MakeWeak(Ptr<T>& ptr)
{
    return ptr;
}