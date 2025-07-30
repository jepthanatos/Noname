#ifndef __INVENTORY_H__
#define __INVENTORY_H__

// System includes
#include <unordered_map>
#include <array>
#include <memory>

// Local includes
#include "Utils.h"
#include "Item.h"
#include "ItemEnumTypes.h"
#include "Property.h"
#include "Container.h"
#include "Weapon.h"

namespace noname
{
    constexpr size_t MAX_SLOTS = 11;  // Changed from 10 to 11 to match TODOS.md specification
    using InventorySlots = std::array<std::shared_ptr<Item>, MAX_SLOTS>;

    class Inventory
    {
    private:
        InventorySlots _slots;

        inline std::shared_ptr<Item> &atSlot(ItemSlotType slot);
        inline const std::shared_ptr<Item> &atSlot(ItemSlotType slot) const;
        bool isValidItemForSlot(const std::shared_ptr<Item>& item, ItemSlotType slot) const;

    public:
        Inventory();

        [[nodiscard]] const InventorySlots &getSlots() const;
        [[nodiscard]] std::shared_ptr<Weapon> getWeapon() const;
        [[nodiscard]] std::shared_ptr<Item> getShield() const;
        [[nodiscard]] std::shared_ptr<Container> getContainer() const;
        [[nodiscard]] std::shared_ptr<Item> dropItem(ItemSlotType slot);
        bool useItem(ItemSlotType slot);
        [[nodiscard]] short getWeight() const;
        void storeItem(std::shared_ptr<Item> item, ItemSlotType slot);
        void handleTwoHandedWeaponLogic(std::shared_ptr<Item> item, ItemSlotType slot);
    };
}

#endif // __INVENTORY_H__
