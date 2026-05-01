#pragma once
#ifdef _USE_OBJECT_POOL
#include "Info.h"
#include "Singleton.h"

#include <map>
#include <queue>
#include <typeindex>

class ObjectPool : public Singleton<ObjectPool>
{
    DECLARE_SINGLETON(ObjectPool)

private:
    std::map<std::type_index, std::queue<class Object*>> _pools;

public:
    bool Init();
    void Destroy();
    void Return(class Object* obj);

    template <typename T>
    T* Rent()
    {
        auto pool = _pools.find(std::type_index(typeid(T)));
        if (pool == _pools.end())
            return nullptr;

        T* retVal = nullptr;
        if (pool->second.empty())
        {
            retVal = new T();

            return retVal;
        }
        else
        {
            retVal = dynamic_cast<T*>(pool->second.front());
            if (nullptr == retVal)
                return nullptr;

            pool->second.pop();
        }

        return retVal;
    }

private:
    template <typename T>
    void PreAlloc(int32 count)
    {
        for (int32 i = 0; i < count; ++i)
        {
            T* obj = new T();
            _pools[std::type_index(typeid(T))].push(obj);
        }
    }
};
#endif
