#include <gtest/gtest.h>

#include "Skill.h"

using namespace noname;
using namespace testing;

struct TestSkill : Test
{
    Skill<SkillType::SWORD> skill{1, 100, 50};
};

TEST_F(TestSkill, getType)
{
    EXPECT_EQ(skill.getType(), SkillType::SWORD);
}

TEST_F(TestSkill, getMinimum)
{
    EXPECT_EQ(skill.getMinValue(), 1);
}

TEST_F(TestSkill, getMaximum)
{
    EXPECT_EQ(skill.getMaxValue(), 100);
}

TEST_F(TestSkill, getTries)
{
    EXPECT_EQ(skill.getTriesNeeded(), 50);
}