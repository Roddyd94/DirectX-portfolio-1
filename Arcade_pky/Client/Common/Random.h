#pragma once
#include <random>

namespace Utility
{
    inline int32 RandomInt(int32 numStart, int32 numEnd)
    {
        static std::mt19937                gen{std::random_device{}()};
        std::uniform_int_distribution<int> dist{numStart, numEnd};

        return dist(gen);
    }

    inline int64 RandomInt(int64 numStart, int64 numEnd)
    {
        static std::mt19937                      gen{std::random_device{}()};
        std::uniform_int_distribution<long long> dist{numStart, numEnd};

        return dist(gen);
    }
} // namespace Utility
