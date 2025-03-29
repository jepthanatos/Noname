#include <gtest/gtest.h>

#include "WeaponsManager.h"

using namespace noname;
using namespace testing;

struct TestWeaponsManager : Test
{
    void SetUp() override
    {
        LM.startUp();
        LM.writeLog(Level::Info, "TestWeaponsManager::SetUp");
        WM.startUp();
    }
    void TearDown() override
    {
        WM.shutDown();
        LM.writeLog(Level::Info, "TestWeaponsManager::shutDown");
        LM.shutDown();
    }
};

TEST_F(TestWeaponsManager, getWeaponsList)
{
    auto weapons_size = WM.getWeaponsList().size();
    EXPECT_GT(weapons_size, 0);
}

TEST_F(TestWeaponsManager, getWeapon)
{
    Weapon club{"Club", SkillType::CLUB, ItemRank::NORMAL, 4};
    auto weapon = WM.getWeapon("Club").get();
    ASSERT_TRUE(club.getName() == weapon->getName());
    ASSERT_TRUE(club.getSkillType() == weapon->getSkillType());
    ASSERT_TRUE(club.getItemType() == weapon->getItemType());
    ASSERT_TRUE(club.getWeight() == weapon->getWeight());
}

TEST_F(TestWeaponsManager, getWeaponReturnsNullWeapon)
{
    EXPECT_EQ(NullWeapon::getInstance().get(), WM.getWeapon("No weapon").get());
}

TEST_F(TestWeaponsManager, getSameWeaponTwoTimes)
{
    auto weapon1 = WM.getWeapon("Club").get();
    auto weapon2 = WM.getWeapon("Club").get();
    ASSERT_TRUE(weapon1 == weapon2);
}