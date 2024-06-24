#include <gtest/gtest.h>

#include "Weapon.h"

using namespace noname;
using namespace testing;

struct TestWeapon : Test
{
    Weapon Weapon{"Great Axe", SkillType::AXE, 20};
};

TEST_F(TestWeapon, getName)
{
    EXPECT_EQ(Weapon.getName(), "Great Axe");
}

TEST_F(TestWeapon, getItemType)
{
    EXPECT_EQ(Weapon.getItemType(), ItemType::WEAPON);
}

TEST_F(TestWeapon, getType)
{
    EXPECT_EQ(Weapon.getType(), SkillType::AXE);
}

TEST_F(TestWeapon, getDie)
{
    EXPECT_EQ(Weapon.getDie(), 20);
}