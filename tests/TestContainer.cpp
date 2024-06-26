#include <gtest/gtest.h>

#include "Container.h"

using namespace noname;
using namespace testing;

struct TestContainer : Test
{
    Container backack{"Backpack", ContainerType::BACKPACK, 8, 100};
};

TEST_F(TestContainer, getName)
{
    EXPECT_EQ(backack.getName(), "Backpack");
}

TEST_F(TestContainer, getItemType)
{
    EXPECT_EQ(backack.getItemType(), ItemType::CONTAINER);
}

TEST_F(TestContainer, getType)
{
    EXPECT_EQ(backack.getType(), ContainerType::BACKPACK);
}

TEST_F(TestContainer, getSlots)
{
    EXPECT_EQ(backack.getSlots(), 8);
}

TEST_F(TestContainer, getMaxCapacity)
{
    EXPECT_EQ(backack.getMaxCapacity(), 100);
}