#include <gtest/gtest.h>

#include "Container.h"

using namespace noname;
using namespace testing;

struct TestContainer : Test
{
    Container Container{"Backpack", ContainerType::BACKPACK, 8, 100};
};

TEST_F(TestContainer, getName)
{
    EXPECT_EQ(Container.getName(), "Backpack");
}

TEST_F(TestContainer, getItemType)
{
    EXPECT_EQ(Container.getItemType(), ItemType::CONTAINER);
}

TEST_F(TestContainer, getType)
{
    EXPECT_EQ(Container.getType(), ContainerType::BACKPACK);
}

TEST_F(TestContainer, getSlots)
{
    EXPECT_EQ(Container.getSlots(), 8);
}

TEST_F(TestContainer, getMaxCapacity)
{
    EXPECT_EQ(Container.getMaxCapacity(), 100);
}