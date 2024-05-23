#include "gtest/gtest.h"

#include "TestLogManager.cpp"
#include "TestCharacter.cpp"
#include "TestRanking.cpp"
#include "TestSkill.cpp"
#include "TestWeapon.cpp"
#include "TestRelationship.cpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
