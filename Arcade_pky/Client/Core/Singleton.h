#pragma once

template <typename T>
class Singleton
{
protected:
    Singleton()                            = default;
    ~Singleton()                           = default;
    Singleton(const Singleton&)            = delete;
    Singleton(Singleton&&)                 = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&)      = delete;

public:
    static T& Instance()
    {
        static T Inst;
        return Inst;
    }
};

#define DECLARE_SINGLETON(Class)                                               \
    friend class Singleton<Class>;                                             \
                                                                               \
protected:                                                                     \
    Class()                        = default;                                  \
    ~Class()                       = default;                                  \
    Class(const Class&)            = delete;                                   \
    Class(Class&&)                 = delete;                                   \
    Class& operator=(const Class&) = delete;                                   \
    Class& operator=(Class&&)      = delete;
