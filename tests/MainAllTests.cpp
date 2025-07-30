#include "gtest/gtest.h"

#include "TestCharacter.cpp"
#include "TestContainer.cpp"
#include "TestCreature.cpp"
#include "TestCreatureManager.cpp"
#include "TestFileManager.cpp"
#include "TestFlyweightPattern.cpp"
#include "TestGameManager.cpp"
#include "TestHeritables.cpp"
#include "TestHtmlBuilder.cpp"
#include "TestInventory.cpp"
#include "TestItem.cpp"
#include "TestItemEnumTypes.cpp"
#include "TestItemManager.cpp"
#include "TestLogManager.cpp"
#include "TestManager.cpp"
#include "TestObserverPattern.cpp"
#include "TestPlayer.cpp"
#include "TestProperty.cpp"
#include "TestRanking.cpp"
#include "TestRankingManager.cpp"
#include "TestRelationships.cpp"
#include "TestSkill.cpp"
#include "TestSkillsManager.cpp"
#include "TestUtils.cpp"
#include "TestWeapon.cpp"
#include "TestWeaponsManager.cpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
