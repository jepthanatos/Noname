#include <gtest/gtest.h>

#include "Container.h"
#include "Weapon.h"

using namespace noname;
using namespace testing;

struct TestContainer : Test
{
    Container backpack{"Backpack", ContainerType::BACKPACK, ItemRank::NORMAL, 8, 100};
};

TEST_F(TestContainer, getName)
{
    EXPECT_EQ(backpack.getName(), "Backpack");
}

TEST_F(TestContainer, getItemType)
{
    EXPECT_EQ(backpack.getItemType(), ItemType::CONTAINER);
}

TEST_F(TestContainer, getType)
{
    EXPECT_EQ(backpack.getType(), ContainerType::BACKPACK);
}

TEST_F(TestContainer, getSlotsNumber)
{
    EXPECT_EQ(backpack.getSlotsNumber(), 8);
}

TEST_F(TestContainer, getMaxCapacity)
{
    EXPECT_EQ(backpack.getMaxCapacity(), 100);
}

TEST_F(TestContainer, AddItem)
{
    Weapon sword("Sword", SkillType::SWORD, ItemRank::NORMAL, 10, 10, 5, 3);
    EXPECT_TRUE(backpack.addItem(sword));
    EXPECT_EQ(backpack.getWeight(), 3);
}

TEST_F(TestContainer, AddItemWhenFull)
{
    for (int i = 0; i < backpack.getSlotsNumber(); ++i)
    {
        Weapon sword("Sword", SkillType::SWORD, ItemRank::NORMAL, 10, 10, 5, 3);
        EXPECT_TRUE(backpack.addItem(sword));
    }
    Weapon sword2("Sword2", SkillType::SWORD, ItemRank::NORMAL, 10, 10, 5, 3);
    EXPECT_FALSE(backpack.addItem(sword2));
    EXPECT_EQ(backpack.getWeight(), 24);
}

TEST_F(TestContainer, RemoveItem)
{
    Container backpack("Backpack", ContainerType::BACKPACK, ItemRank::NORMAL, 5, 10);
    Weapon sword("Sword", SkillType::SWORD, ItemRank::NORMAL, 10, 10, 5, 3);
    backpack.addItem(sword);
    auto removedItem = backpack.removeItem(0);
    EXPECT_TRUE(removedItem.has_value());
    EXPECT_EQ(removedItem->getName(), "Sword");
    EXPECT_EQ(backpack.getWeight(), 0);
}

TEST_F(TestContainer, RemoveItemFromEmptySlot)
{
    auto removedItem = backpack.removeItem(0);
    EXPECT_FALSE(removedItem.has_value());
}

TEST_F(TestContainer, GetWeightMultipleItems)
{
    Weapon sword("Sword", SkillType::SWORD, ItemRank::NORMAL, 10, 10, 5, 3);
    Weapon shield("Shield", SkillType::SHIELDING, ItemRank::NORMAL, 10, 10, 5, 5);
    backpack.addItem(sword);
    backpack.addItem(shield);
    EXPECT_EQ(backpack.getWeight(), 8);
}
