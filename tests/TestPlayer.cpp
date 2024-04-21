#include <gtest/gtest.h>

#include "Player.h"

// System includes
#include <cmath>
#include <math.h>

using namespace noname;
using namespace testing;

struct TestPlayer: Test
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

    for (int i = 0; i <= MELEE_SKILLS + 10; ++i)
    {
        player.updateTries(SkillType::SWORD);
    }

    swordLevel = player.getSkill(SkillType::SWORD);
    EXPECT_EQ(swordLevel, 2);
}

TEST_F(TestPlayer, getMaxDamageAtLevel1)
{
    // Skill = 1, WeaponDamage = 10, AttackFactor = 1
    player.setLevel(1);
    const short damage{player.getMaxDamage(1, 10, 1)};
    EXPECT_EQ(damage, 6);
    EXPECT_EQ(player.getLevel(), 1);
    EXPECT_EQ(player.getSkill(SkillType::SWORD), 1);
}

TEST_F(TestPlayer, setSkillAndGetDamageAtLevel1)
{
    player.setLevel(1);
    short level{player.getLevel()};
    EXPECT_EQ(level, 1);

    player.setSkill(SkillType::FIST, 10);
    short damage{player.getDamage(SkillType::FIST)};
    EXPECT_EQ(damage, 13);

    player.setSkill(SkillType::SWORD, 20);
    damage = player.getDamage(SkillType::SWORD);
    EXPECT_EQ(damage, 21);

    player.setSkill(SkillType::CLUB, 30);
    damage = player.getDamage(SkillType::CLUB);
    EXPECT_EQ(damage, 29);
}

TEST_F(TestPlayer, setSkillAndGetDamageAtLevel10)
{
    player.setLevel(10);
    short level{player.getLevel()};
    EXPECT_EQ(level, 10);

    player.setSkill(SkillType::FIST, 10);
    short damage{player.getDamage(SkillType::FIST)};
    EXPECT_EQ(damage, 15);

    player.setSkill(SkillType::SWORD, 20);
    damage = player.getDamage(SkillType::SWORD);
    EXPECT_EQ(damage, 23);

    player.setSkill(SkillType::CLUB, 30);
    damage = player.getDamage(SkillType::CLUB);
    EXPECT_EQ(damage, 31);
}

TEST_F(TestPlayer, setSkillAndGetDamageAtLevel100)
{
    player.setLevel(100);
    short level{player.getLevel()};
    EXPECT_EQ(level, 100);

    player.setSkill(SkillType::FIST, 10);
    short damage{player.getDamage(SkillType::FIST)};
    EXPECT_EQ(damage, 33);

    player.setSkill(SkillType::SWORD, 20);
    damage = player.getDamage(SkillType::SWORD);
    EXPECT_EQ(damage, 41);

    player.setSkill(SkillType::CLUB, 30);
    damage = player.getDamage(SkillType::CLUB);
    EXPECT_EQ(damage, 49);
}

TEST_F(TestPlayer, takeDamage)
{
    player.setLevel(1);
    short level{player.getLevel()};
    EXPECT_EQ(level, 1);

    player.setSkill(SkillType::SWORD, 20);
    short damage{player.getDamage(SkillType::SWORD)};
    EXPECT_EQ(damage, 21);

    short initialHealth{player.getHealth()};
    player.takeDamage(damage);
    short finalHealth{player.getHealth()};
    ASSERT_EQ(finalHealth, initialHealth - damage);
}

TEST_F(TestPlayer, takeDamageAndDie)
{
    player.setLevel(2);
    short level{player.getLevel()};
    EXPECT_EQ(level, 2);

    player.setSkill(SkillType::SWORD, 20);
    short damage{player.getDamage(SkillType::SWORD)};
    EXPECT_EQ(damage, 21);

    short health{player.getHealth()};
    while(health > 0)
    {
        player.takeDamage(damage);
        health = player.getHealth();
    }
    ASSERT_TRUE(health <= 0);
    ASSERT_EQ(player.getLevel(), 1);
    unsigned long long experience = player.getExpForLevel(2) - ceil((player.getExpForLevel(2) * 25) / 100);
    ASSERT_EQ(player.getExperience(), experience);
}

TEST_F(TestPlayer, playerHealthAtLevel1)
{
    player.setLevel(1);
    short level{player.getLevel()};
    EXPECT_EQ(level, 1);

    short initialHealth{player.getHealth()};
    ASSERT_EQ(initialHealth, 5 * ((2 * level) + 21));
}

TEST_F(TestPlayer, playerHealthAtEachLevelFrom1To1000)
{
    for (int i = 1; i < 1001; i += 10)
    {
        player.setLevel(i);
        short level{player.getLevel()};
        EXPECT_EQ(level, i);

        short health{player.getHealth()};
        ASSERT_EQ(health, 5 * ((2 * i) + 21));
    }
}

TEST_F(TestPlayer, playerExperienceAtEachLevelFrom1To1000)
{
    auto getExp = [](int level) {
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