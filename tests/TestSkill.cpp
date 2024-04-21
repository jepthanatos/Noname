#include <gtest/gtest.h>

#include "Skill.h"

using namespace noname;

TEST(TestSkill, getType)
{
    Skill<SkillType::SWORD> s{1, 1, 1};

    ASSERT_EQ(s.getType(), SkillType::SWORD);
}