#ifndef __UTILS_H__
#define __UTILS_H__

// System includes
#include <random>

namespace noname
{
    class Utils
    {
    public:
        static auto rollDice(int min, int max)
        {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
            return dist6(rng);
        };
    };
}

#endif // __UTILS_H__
