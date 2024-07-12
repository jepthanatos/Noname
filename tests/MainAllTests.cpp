#include "gtest/gtest.h"

#include "TestCharacter.cpp"
#include "TestContainer.cpp"
#include "TestCreatures.cpp"
#include "TestFileManager.cpp"
#include "TestGameManager.cpp"
#include "TestInventory.cpp"
#include "TestItem.cpp"
#include "TestLogManager.cpp"
#include "TestPlayer.cpp"
#include "TestProperty.cpp"
#include "TestRanking.cpp"
#include "TestRelationships.cpp"
#include "TestSkill.cpp"
#include "TestSkillsManager.cpp"
#include "TestUtils.cpp"
#include "TestWeapon.cpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
