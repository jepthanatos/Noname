#include <gtest/gtest.h>

#include "Character.h"
#include "WeaponsManager.h"
#include "LogManager.h"

// System includes
#include <cmath>
#include <math.h>

using namespace noname;
using namespace testing;

struct TestCharacter : Test
{
    Character character{"Noname"};

    void SetUp() override
    {
        LM.startUp();
        WM.initializeWeapons();
        character.equipWeapon("Club");
    }
    void TearDown() override
    {
        LM.shutDown();
    }
};

TEST_F(TestCharacter, getName)
{
    EXPECT_EQ(character.getName(), "Noname");
}

TEST_F(TestCharacter, getExperience)
{
    EXPECT_EQ(character.getExperience(), 0);
}

TEST_F(TestCharacter, CharacterExperienceAtEachLevelFrom1To1000)
{
    auto getExp = [](int level)
    {
        return ((50ULL * level * level * level) - (150ULL * level * level) + (400ULL * level)) / 3ULL;
    };

    for (int i = 1; i < 1001; i++)
    {
        ASSERT_EQ(character.getExpForLevel(i), getExp(i));
    }
}

TEST_F(TestCharacter, CharacterAddExperience)
{
    character.addExperience(1000);
    EXPECT_EQ(character.getExperience(), 1000);
}

TEST_F(TestCharacter, getLevel)
{
    EXPECT_EQ(character.getLevel(), 1);
}

TEST_F(TestCharacter, getMagicLevel)
{
    EXPECT_EQ(character.getMagicLevel(), 1);
}

TEST_F(TestCharacter, CharacterAddExperienceAndLevelUp)
{
    character.addExperience(character.getExpForLevel(100) - character.getExperience());
    EXPECT_EQ(character.getLevel(), 100);
}

TEST_F(TestCharacter, getSkill)
{
    EXPECT_EQ(character.getSkill(SkillType::CLUB), short(1));
}

TEST_F(TestCharacter, CharacterHealthAtEachLevelFrom1To1000)
{
    for (int i = 1; i < 1001; i++)
    {
        character.addExperience(character.getExpForLevel(i) - character.getExpForLevel(i - 1));
        EXPECT_EQ(character.getLevel(), i);

        ASSERT_EQ(character.getCurrentHealth(), 10 * character.getLevel());
    }
}

TEST_F(TestCharacter, getAttackDamage)
{
    EXPECT_TRUE(character.getAttackDamage() >= 0);
}

TEST_F(TestCharacter, takeDamage)
{
    short damage{character.getAttackDamage()};
    int initialHealth{character.getCurrentHealth()};
    character.takeDamage(damage);
    ASSERT_EQ(character.getCurrentHealth(), initialHealth - damage);
}

TEST_F(TestCharacter, takeDamageAndDie)
{
    character.addExperience(character.getExpForLevel(2));
    ASSERT_EQ(character.getLevel(), 2);
    ASSERT_EQ(character.getExperience(), character.getExpForLevel(2));

    while (!character.isDead())
    {
        character.takeDamage(character.getAttackDamage());
    }
    ASSERT_TRUE(character.getCurrentHealth() <= 0);

    character.respawn();
    ASSERT_EQ(character.getLevel(), 1);
    auto experience = character.getExpForLevel(2) - ceil((character.getExpForLevel(2) * 25) / 100);
    ASSERT_EQ(character.getExperience(), experience);
}

TEST_F(TestCharacter, CharacterGetWeapon)
{
    Weapon club{"Club", SkillType::CLUB, 4};
    EXPECT_EQ(club, character.getWeapon());
}

TEST_F(TestCharacter, CharacterAttack)
{
    for (int i = 0; i < 1000; ++i)
    {
        character.attack();
    }
    Property<short int> expectedSkill{1};
    ASSERT_TRUE(character.getSkill(SkillType::CLUB) > expectedSkill);
}
