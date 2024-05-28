#include <gtest/gtest.h>

#include "Utils.h"

using namespace noname;
using namespace testing;

TEST(TestUtils, rollDice)
{
    auto result{Utils::rollDice(2, 4)};
    EXPECT_TRUE(result >= 2);
    EXPECT_TRUE(result <= 4);
}