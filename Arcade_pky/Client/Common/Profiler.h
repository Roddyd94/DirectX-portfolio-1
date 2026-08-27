#pragma once
template <typename T>
class Profiler
{
public:
    Profiler(std::size_t queueSize) : _containerSize(queueSize) {}

private:
    std::deque<T> _container;
    std::size_t   _containerSize = 0;
    int32         _counter       = 0;

public:
    void Reset()
    {
        _container.clear();
        _containerSize = 0;
        _counter       = 0;
    }

    void Count() { ++_counter; }

    void Add(T value)
    {
        _container.push_back(value);
        while (_container.size() > _containerSize)
        {
            if (_container.size() <= 0)
                break;

            _container.pop_front();
        }
        ++_counter;
    }

    T GetAverage() const
    {
        if (_container.size() <= 0)
            return 0;

        T result = 0;
        for (T value : _container)
            result += value;

        return result / _container.size();
    }
    T GetMax() const
    {
        T result = 0;
        for (T value : _container)
            result = std::max(result, value);

        return result;
    }
    T GetMin() const
    {
        T result = 0;
        for (T value : _container)
            result = std::min(result, value);

        return result;
    }
    int32 count() const { return _counter; }
};
