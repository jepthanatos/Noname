#include <gtest/gtest.h>

// Local includes
#include "Player.h"
#include "WeaponsManager.h"
#include "LogManager.h"

// System includes
#include <cmath>
#include <math.h>

using namespace noname;
using namespace testing;

struct TestPlayer : Test
{
    Player playerOne{"Noname"};

    void SetUp() override
    {
        LM.startUp();
        WM.startUp();
        playerOne.respawn();
        playerOne.equipWeapon("Club");
    }
    void TearDown() override
    {
        LM.shutDown();
    }
};

TEST_F(TestPlayer, getName)
{
    EXPECT_EQ(playerOne.getName(), "Noname");
}

TEST_F(TestPlayer, getExperience)
{
    EXPECT_EQ(playerOne.getExperience(), 0);
}

TEST_F(TestPlayer, PlayerExperienceAtEachLevelFrom1To1000)
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

TEST_F(TestPlayer, PlayerAddExperience)
{
    playerOne.gainExperience(1000);
    EXPECT_EQ(playerOne.getExperience(), 1000);
}

TEST_F(TestPlayer, getLevel)
{
    EXPECT_EQ(playerOne.getLevel(), 1);
}

TEST_F(TestPlayer, getMagicLevel)
{
    EXPECT_EQ(playerOne.getMagicLevel(), 1);
}

TEST_F(TestPlayer, PlayerAddExperienceAndLevelUp)
{
    playerOne.gainExperience(GM.getExpForLevel(100) - playerOne.getExperience());
    EXPECT_EQ(playerOne.getLevel(), 100);
}

TEST_F(TestPlayer, getSkill)
{
    EXPECT_EQ(playerOne.getSkill(SkillType::CLUB), short(1));
}

TEST_F(TestPlayer, PlayerHealthAtEachLevelFrom1To1000)
{
    for (int i = 1; i < 1001; i++)
    {
        playerOne.gainExperience(GM.getExpForLevel(i) - GM.getExpForLevel(i - 1));
        EXPECT_EQ(playerOne.getLevel(), i);

        ASSERT_TRUE(playerOne.getMaxHealth() > playerOne.getHeritable(HeritableType::CONSTITUTION) * playerOne.getLevel());
    }
}

TEST_F(TestPlayer, getAttackDamage)
{
    EXPECT_TRUE(playerOne.getAttackDamage() >= 0);
}

TEST_F(TestPlayer, takeDamage)
{
    short damage{playerOne.getAttackDamage()};
    int initialHealth{playerOne.getCurrentHealth()};
    playerOne.takeDamage(damage);
    ASSERT_EQ(playerOne.getCurrentHealth(), initialHealth - damage);
}

TEST_F(TestPlayer, takeDamageAndDie)
{
    playerOne.gainExperience(GM.getExpForLevel(2));
    ASSERT_EQ(playerOne.getLevel(), 2);
    ASSERT_EQ(playerOne.getExperience(), GM.getExpForLevel(2));

    while (!playerOne.isDead())
    {
        playerOne.takeDamage(playerOne.getAttackDamage());
    }
    ASSERT_TRUE(playerOne.getCurrentHealth() <= 0);

    playerOne.respawn();
    ASSERT_EQ(playerOne.getLevel(), 1);
    auto experience = GM.getExpForLevel(2) - ceil((GM.getExpForLevel(2) * 25) / 100);
    ASSERT_EQ(playerOne.getExperience(), experience);
}

TEST_F(TestPlayer, PlayerSetWeaponAndGetWeapon)
{
    Weapon club{"Club", SkillType::CLUB, 4};
    EXPECT_EQ(club, playerOne.getWeapon());
}

TEST_F(TestPlayer, PlayerAttack)
{
    for (int i = 0; i < 1000; ++i)
    {
        playerOne.attack();
    }
    Property<short int> expectedSkill{1};
    ASSERT_TRUE(playerOne.getSkill(SkillType::CLUB) > expectedSkill);
}
