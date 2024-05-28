#include <gtest/gtest.h>

#include "Item.h"

using namespace noname;
using namespace testing;

struct TestItem : Test
{
    Item Item{"Something", ItemType::ARMOR};
};

TEST_F(TestItem, getName)
{
    EXPECT_EQ(Item.getName(), "Something");
}

TEST_F(TestItem, getItemType)
{
    EXPECT_EQ(Item.getItemType(), ItemType::ARMOR);
}