#include <gtest/gtest.h>

#include "Creature.h"
#include "CreaturesManager.h"

using namespace noname;
using namespace testing;

struct TestCreature : Test
{
    void SetUp() override
    {
        LM.startUp();
        CM.startUp();
    }
    void TearDown() override
    {
        CM.shutDown();
        LM.shutDown();
    }
};

TEST_F(TestCreature, findPredatorOf)
{
    EXPECT_TRUE(CM.findPredatorOf("Rat").size() > 0);
}

TEST_F(TestCreature, findPreyOf)
{
    EXPECT_TRUE(CM.findPreyOf("Cat").size() > 0);
}