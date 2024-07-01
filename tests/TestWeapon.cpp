#include <gtest/gtest.h>

#include "Weapon.h"

using namespace noname;
using namespace testing;

struct TestWeapon : Test
{
    Weapon weapon{"Great Axe", SkillType::AXE, 20};
};

TEST_F(TestWeapon, getName)
{
    EXPECT_EQ(weapon.getName(), "Great Axe");
}

TEST_F(TestWeapon, getItemType)
{
    EXPECT_EQ(weapon.getItemType(), ItemType::WEAPON);
}

TEST_F(TestWeapon, getType)
{
    EXPECT_EQ(weapon.getType(), SkillType::AXE);
}

TEST_F(TestWeapon, getDie)
{
    EXPECT_EQ(weapon.getDie(), 20);
}