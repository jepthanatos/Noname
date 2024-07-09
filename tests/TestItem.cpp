#include <gtest/gtest.h>

#include "Item.h"

using namespace noname;
using namespace testing;

struct TestItem : Test
{
    Item item{"Something", ItemType::ARMOR};
};

TEST_F(TestItem, getName)
{
    EXPECT_EQ(item.getName(), "Something");
}

TEST_F(TestItem, getItemType)
{
    EXPECT_EQ(item.getItemType(), ItemType::ARMOR);
}

TEST_F(TestItem, getValue)
{
    EXPECT_EQ(item.getValue(), NO_VALUE);
}

TEST_F(TestItem, getUses)
{
    EXPECT_EQ(item.getUses(), NO_VALUE);
}

TEST_F(TestItem, getWeight)
{
    EXPECT_EQ(item.getWeight(), NO_VALUE);
}

struct TestOtherItem : Test
{
    Item OtherItem{"Rune", ItemType::USABLE, ItemRank::NORMAL, 20, 10, 5};
};

TEST_F(TestOtherItem, getName)
{
    EXPECT_EQ(OtherItem.getName(), "Rune");
}

TEST_F(TestOtherItem, getItemType)
{
    EXPECT_EQ(OtherItem.getItemType(), ItemType::USABLE);
}

TEST_F(TestOtherItem, getValue)
{
    EXPECT_EQ(OtherItem.getValue(), 20);
}

TEST_F(TestOtherItem, getUses)
{
    EXPECT_EQ(OtherItem.getUses(), 10);
}

TEST_F(TestOtherItem, getWeight)
{
    EXPECT_EQ(OtherItem.getWeight(), 5);
}