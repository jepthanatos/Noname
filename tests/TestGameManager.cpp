#include <gtest/gtest.h>

#include "GameManager.h"

using namespace noname;
using namespace testing;

TEST(TestGameManager, ExperienceAtEachLevelFrom1To1000)
{
    auto getExp = [](int level)
    {
        return ((50ULL * level * level * level) - (150ULL * level * level) + (400ULL * level)) / 3ULL;
    };

    for (int i = 1; i < 1001; i++)
    {
        ASSERT_EQ(GM.getExpForLevel(i), getExp(i));
    }
}

TEST(TestGameManager, ManaAtEachLevelFrom1To1000)
{
    auto getMana = [](int level)
    {
        return 800ULL * level * level + 800ULL * level + 0ULL;
    };

    for (int i = 1; i < 1001; i++)
    {
        ASSERT_EQ(GM.getManaForLevel(i), getMana(i));
    }
}