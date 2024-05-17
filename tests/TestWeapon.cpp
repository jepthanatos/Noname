#include <gtest/gtest.h>

#include "Weapon.h"

using namespace noname;
using namespace testing;

struct TestWeapon : Test
{
    Weapon Weapon{"Great Axe", SkillType::AXE, 20};
};

TEST_F(TestWeapon, getLevel)
{
    EXPECT_EQ(Weapon.getName(), "Great Axe");
}

TEST_F(TestWeapon, getType)
{
    EXPECT_EQ(Weapon.getType(), SkillType::AXE);
}

TEST_F(TestWeapon, getDice)
{
    EXPECT_EQ(Weapon.getDice(), 20);
}