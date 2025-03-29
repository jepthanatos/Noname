#ifndef __UTILS_H__
#define __UTILS_H__

#include <random>
#include <type_traits>

namespace noname
{
    class Utils
    {
    public:
        static int rollDie(int min, int max)
        {
            static thread_local std::random_device dev;
            static thread_local std::mt19937 rng(dev());
            std::uniform_int_distribution<int> dist(min, max);
            return dist(rng);
        }

        template <typename T>
        static constexpr int toInt(T value) noexcept
        {
            static_assert(std::is_enum_v<T>, "toInt requires an enum type");
            return static_cast<int>(value);
        }
    };
}

#endif // __UTILS_H__
