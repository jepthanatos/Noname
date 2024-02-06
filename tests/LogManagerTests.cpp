#include <gtest/gtest.h>

#include "../src/LogManager.h"

using namespace noname;
using namespace testing;

TEST(TestLogManager, getLevel)
{
    auto level{LogManager::getInstance().getLevel()};
    EXPECT_EQ(level, Level::Debug);
}

TEST(TestLogManager, setLevel)
{
    LogManager::getInstance().setLevel(Level::Error);
    auto level{LogManager::getInstance().getLevel()};
    EXPECT_EQ(level, Level::Error);
}