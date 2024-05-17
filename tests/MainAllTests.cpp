#include "gtest/gtest.h"

#include "TestLogManager.cpp"
#include "TestPlayer.cpp"
#include "TestRankManager.cpp"
#include "TestSkill.cpp"
#include "TestWeapon.cpp"

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
