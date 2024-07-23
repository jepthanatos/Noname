#include <gtest/gtest.h>

#include "Inventory.h"
#include "Weapon.h"

using namespace noname;
using namespace testing;

struct TestInventory : Test
{
    Inventory inventory;
    std::unique_ptr<Weapon> sword = std::make_unique<Weapon>("Sword", SkillType::SWORD, ItemRank::NORMAL, 10, 10, 5, 3);
};

TEST_F(TestInventory, Constructor)
{
    auto slots = inventory.getSlots();
    EXPECT_EQ(slots.size(), Utils::toInt(ItemSlotType::LAST_SLOT_TYPE));
    for (const auto &slot : slots)
    {
        EXPECT_FALSE(slot.get());
    }
}

TEST_F(TestInventory, StoreItem)
{
    inventory.storeItem(std::move(sword), ItemSlotType::WEAPON);
    auto slots = inventory.getSlots();
    EXPECT_TRUE(slots.at(Utils::toInt(ItemSlotType::WEAPON)).get());
    EXPECT_EQ(slots.at(Utils::toInt(ItemSlotType::WEAPON))->getName(), "Sword");
}

TEST_F(TestInventory, StoreItemWrongType)
{
    inventory.storeItem(std::move(sword), ItemSlotType::HELMET);
    auto slots = inventory.getSlots();
    EXPECT_FALSE(slots.at(Utils::toInt(ItemSlotType::HELMET)).get());
}

TEST_F(TestInventory, DropItem)
{
    inventory.storeItem(std::move(sword), ItemSlotType::WEAPON);
    auto item = inventory.dropItem(ItemSlotType::WEAPON);
    EXPECT_TRUE(item.get());
    EXPECT_EQ(item->getName(), "Sword");
    auto slots = inventory.getSlots();
    EXPECT_FALSE(slots.at(Utils::toInt(ItemSlotType::WEAPON)).get());
}

TEST_F(TestInventory, UseItem)
{
    inventory.storeItem(std::move(sword), ItemSlotType::WEAPON);
    inventory.useItem(ItemSlotType::WEAPON);
    auto slots = inventory.getSlots();
    EXPECT_EQ(slots.at(Utils::toInt(ItemSlotType::WEAPON))->getUses(), 4);
}

TEST_F(TestInventory, GetWeight)
{
    inventory.storeItem(std::move(sword), ItemSlotType::WEAPON);
    EXPECT_EQ(inventory.getWeight(), 3);
    std::unique_ptr<Weapon> shield = std::make_unique<Weapon>("Shield", SkillType::SHIELDING, ItemRank::NORMAL, 10, 10, 5, 5);
    inventory.storeItem(std::move(shield), ItemSlotType::SHIELD);
    EXPECT_EQ(inventory.getWeight(), 8);
}
