#include <gtest/gtest.h>

#include "Player.h"

// System includes
#include <cmath>
#include <math.h>

using namespace noname;
using namespace testing;

struct TestPlayer : Test
{
    Player player{};
};

TEST_F(TestPlayer, getLevel)
{
    short level{player.getLevel()};
    EXPECT_EQ(level, 1);
}

TEST_F(TestPlayer, setLevel)
{
    player.setLevel(756);
    short level{player.getLevel()};
    EXPECT_EQ(level, 756);
}

TEST_F(TestPlayer, updateTriesTest)
{
    short swordLevel{player.getSkill(SkillType::SWORD)};
    EXPECT_EQ(swordLevel, 1);

    for (int i = 0; i <= MELEE_TRIES + 10; ++i)
    {
        player.updateTries(SkillType::SWORD);
    }

    swordLevel = player.getSkill(SkillType::SWORD);
    EXPECT_EQ(swordLevel, 2);
}

TEST_F(TestPlayer, takeDamage)
{
    player.setSkill(SkillType::SWORD, 20);
    short damage{player.getDamage(SkillType::SWORD)};

    short initialHealth{player.getHealth()};
    player.takeDamage(damage);
    short finalHealth{player.getHealth()};
    ASSERT_EQ(finalHealth, initialHealth - damage);
}

TEST_F(TestPlayer, takeDamageAndDie)
{
    player.setLevel(2);
    short level{player.getLevel()};

    player.setSkill(SkillType::SWORD, 20);
    short damage{player.getDamage(SkillType::SWORD)};

    short health{player.getHealth()};
    while (health > 0)
    {
        player.takeDamage(damage);
        health = player.getHealth();
    }
    ASSERT_TRUE(health <= 0);
    ASSERT_EQ(player.getLevel(), 1);
    unsigned long long experience = player.getExpForLevel(2) - ceil((player.getExpForLevel(2) * 25) / 100);
    ASSERT_EQ(player.getExperience(), experience);
}

TEST_F(TestPlayer, playerHealthAtEachLevelFrom1To1000)
{
    for (int i = 1; i < 1001; i += 10)
    {
        player.setLevel(i);
        short level{player.getLevel()};
        EXPECT_EQ(level, i);

        short health{player.getHealth()};
        ASSERT_EQ(health, 10 * level);
    }
}

TEST_F(TestPlayer, playerExperienceAtEachLevelFrom1To1000)
{
    auto getExp = [](int level)
    {
        return ((50ULL * level * level * level) - (150ULL * level * level) + (400ULL * level)) / 3ULL;
    };

    for (int i = 1; i < 1001; i += 10)
    {
        player.setLevel(i);
        short level{player.getLevel()};
        EXPECT_EQ(level, i);

        ASSERT_EQ(player.getExpForLevel(i), getExp(i));
    }
}