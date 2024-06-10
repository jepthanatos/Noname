#include <gtest/gtest.h>

#include "SkillsManager.h"
#include "LogManager.h"
#include "Skill.h"

using namespace noname;
using namespace testing;

struct TestSkillsManager : Test
{
    Skill s1{0, 100, 50};
    Skill s2{10, 70, 40};
    void SetUp() override
    {
        LM.startUp();
        SM.startUp();
    }
    void TearDown() override
    {
        SM.shutDown();
        LM.shutDown();
    }
};

TEST_F(TestSkillsManager, getSkill)
{
    EXPECT_TRUE(SM.getSkill(SkillType::FIST) == s1);
}
