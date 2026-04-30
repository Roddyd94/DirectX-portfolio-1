#pragma once

using byte = unsigned char;

using uint8  = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

using int8  = signed char;
using int16 = signed short;
using int32 = signed int;
using int64 = signed long long;

template <typename T>
T PI = 3.14159265358979323846264338327950288419716939937510;

template <typename T>
T TAU = 6.28318530717958647692528676655900576839433879875021;

#define DESTROY(object)                                                        \
    if (object)                                                                \
    {                                                                          \
        object->Destroy();                                                     \
        Delete(object);                                                        \
    }

#define DECLARE_NON_COPYABLE(Class)                                            \
    Class(const Class&)            = delete;                                   \
    Class& operator=(const Class&) = delete;

#define DECLARE_NON_MOVABLE(Class)                                             \
    Class(Class&&)            = delete;                                        \
    Class& operator=(Class&&) = delete;

#define DELETE_SPECIAL_FUNC(Class)                                             \
    DECLARE_NON_COPYABLE(Class)                                                \
    DECLARE_NON_MOVABLE(Class)
