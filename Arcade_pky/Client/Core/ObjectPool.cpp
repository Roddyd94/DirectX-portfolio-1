#include "pch.h"

#ifdef _USE_OBJECT_POOL
#include "Object.h"
#include "ObjectPool.h"

bool ObjectPool::Init()
{
    // 오브젝트 풀에 오브젝트를 미리 생성한다.
    // PreAlloc<T>(N);

    return true;
}

void ObjectPool::Destroy()
{
    for (auto& [_, pool] : _pools)
    {
        while (!pool.empty())
        {
            Object* obj = pool.front();
            if (nullptr != obj)
            {
                delete obj;
                obj = nullptr;
            }

            pool.pop();
        }
    }
}

void ObjectPool::Return(Object* obj)
{
    if (nullptr == obj)
        return;

    if (!obj->_isPooled)
    {
        delete obj;
        return;
    }

    obj->SetActive(false);

    _pools[std::type_index(typeid(*obj))].push(obj);
}
#endif
