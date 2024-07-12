#include <gtest/gtest.h>

#include "Inventory.h"
#include "Weapon.h"

using namespace noname;
using namespace testing;

struct TestInventory : Test
{
    Inventory inventory;
    Weapon sword{"Sword", SkillType::SWORD, ItemRank::NORMAL, 10, 10, 5, 3};
};

TEST_F(TestInventory, ConstructorTest)
{
    auto slots = inventory.getSlots();
    EXPECT_EQ(slots.size(), Utils::toInt(ItemSlotType::LAST_SLOT_TYPE));
    for (const auto &slot : slots)
    {
        EXPECT_FALSE(slot.has_value());
    }
}

TEST_F(TestInventory, StoreItemTest)
{
    inventory.storeItem(sword, ItemSlotType::WEAPON);
    auto slots = inventory.getSlots();
    EXPECT_TRUE(slots.at(Utils::toInt(ItemSlotType::WEAPON)).has_value());
    EXPECT_EQ(slots.at(Utils::toInt(ItemSlotType::WEAPON))->getName(), "Sword");
}

TEST_F(TestInventory, StoreItemWrongTypeTest)
{
    inventory.storeItem(sword, ItemSlotType::HELMET);
    auto slots = inventory.getSlots();
    EXPECT_FALSE(slots.at(Utils::toInt(ItemSlotType::HELMET)).has_value());
}

TEST_F(TestInventory, GetItemTest)
{
    inventory.storeItem(sword, ItemSlotType::WEAPON);
    auto item = inventory.getItem(ItemSlotType::WEAPON);
    EXPECT_TRUE(item.has_value());
    EXPECT_EQ(item->getName(), "Sword");
    auto slots = inventory.getSlots();
    EXPECT_FALSE(slots.at(Utils::toInt(ItemSlotType::WEAPON)).has_value());
}

TEST_F(TestInventory, UseItemTest)
{
    inventory.storeItem(sword, ItemSlotType::WEAPON);
    inventory.useItem(ItemSlotType::WEAPON);
    auto slots = inventory.getSlots();
    EXPECT_EQ(slots.at(Utils::toInt(ItemSlotType::WEAPON))->getUses(), 4);
}

TEST_F(TestInventory, GetWeightTest)
{
    inventory.storeItem(sword, ItemSlotType::WEAPON);
    EXPECT_EQ(inventory.getWeight(), 3);
    Weapon shield("Shield", SkillType::SHIELDING, ItemRank::NORMAL, 10, 10, 5, 5);
    inventory.storeItem(shield, ItemSlotType::SHIELD);
    EXPECT_EQ(inventory.getWeight(), 8);
}
