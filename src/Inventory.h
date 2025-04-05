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
    constexpr size_t MAX_SLOTS = 10;
    using InventorySlots = std::array<std::shared_ptr<Item>, MAX_SLOTS>;

    class Inventory
    {
    private:
        InventorySlots _slots;

        inline std::shared_ptr<Item> &atSlot(ItemSlotType slot);
        inline const std::shared_ptr<Item> &atSlot(ItemSlotType slot) const;

    public:
        Inventory();

        [[nodiscard]] const InventorySlots &getSlots() const;
        [[nodiscard]] std::shared_ptr<Weapon> getWeapon() const;
        [[nodiscard]] std::shared_ptr<Item> dropItem(ItemSlotType slot);
        bool useItem(ItemSlotType slot);
        [[nodiscard]] short getWeight() const;
        void storeItem(std::shared_ptr<Item> item, ItemSlotType slot);
    };
}

#endif // __INVENTORY_H__
