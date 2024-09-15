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

TEST_F(TestContainer, getContainerType)
{
    EXPECT_EQ(backpack.getContainerType(), ContainerType::BACKPACK);
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
    std::unique_ptr<Weapon> sword = std::make_unique<Weapon>("Sword", SkillType::SWORD, ItemRank::NORMAL, 10, 10, 5, 3);
    ASSERT_TRUE(backpack.addItem(std::move(sword)));
    EXPECT_EQ(backpack.getWeight(), 3);
}

TEST_F(TestContainer, AddItemWhenFull)
{
    for (int i = 0; i < backpack.getSlotsNumber(); ++i)
    {
        std::unique_ptr<Weapon> sword = std::make_unique<Weapon>("Sword", SkillType::SWORD, ItemRank::NORMAL, 10, 10, 5, 3);
        ASSERT_TRUE(backpack.addItem(std::move(sword)));
    }
    std::unique_ptr<Weapon> sword2 = std::make_unique<Weapon>("Sword2", SkillType::SWORD, ItemRank::NORMAL, 10, 10, 5, 3);
    ASSERT_FALSE(backpack.addItem(std::move(sword2)));
    EXPECT_EQ(backpack.getWeight(), 24);
}

TEST_F(TestContainer, RemoveItem)
{
    Container backpack("Backpack", ContainerType::BACKPACK, ItemRank::NORMAL, 5, 10);
    std::unique_ptr<Weapon> sword = std::make_unique<Weapon>("Sword", SkillType::SWORD, ItemRank::NORMAL, 10, 10, 5, 3);
    backpack.addItem(std::move(sword));
    auto removedItem = backpack.removeItem(0);
    ASSERT_TRUE(removedItem.get());
    EXPECT_EQ(removedItem->getName(), "Sword");
    EXPECT_EQ(backpack.getWeight(), 0);
}

TEST_F(TestContainer, RemoveItemFromEmptySlot)
{
    auto removedItem = backpack.removeItem(0);
    EXPECT_FALSE(removedItem.get());
}

TEST_F(TestContainer, GetWeightMultipleItems)
{
    std::unique_ptr<Weapon> sword = std::make_unique<Weapon>("Sword", SkillType::SWORD, ItemRank::NORMAL, 10, 10, 5, 3);
    std::unique_ptr<Weapon> shield = std::make_unique<Weapon>("Shield", SkillType::SHIELDING, ItemRank::NORMAL, 10, 10, 5, 5);
    backpack.addItem(std::move(sword));
    backpack.addItem(std::move(shield));
    EXPECT_EQ(backpack.getWeight(), 8);
}
