#ifndef __UTILS_H__
#define __UTILS_H__

// System includes
#include <random>

namespace noname
{
    class Utils
    {
    public:
        static auto rollDie(int min, int max)
        {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
            return dist6(rng);
        };

        static int toInt(SkillType skill)
        {
            return static_cast<int>(skill);
        }
    };
}

#endif // __UTILS_H__
