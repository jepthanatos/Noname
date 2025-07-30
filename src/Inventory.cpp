#include "Inventory.h"
#include "LogManager.h"
#include "Weapon.h"

namespace noname
{
    /**
     * @brief Constructor for Inventory class. Initializes slots based on ItemSlotType.
     */
    Inventory::Inventory()
    {
        _slots.fill(nullptr); // Inicializa todos los slots con nullptr
    }

    /**
     * @brief Get all slots in the inventory.
     * @return A constant reference to the array of item slots.
     */
    const InventorySlots &Inventory::getSlots() const { return _slots; }

    /**
     * @brief Get the weapon from the inventory.
     * @return A shared pointer to the weapon, or nullptr if not present.
     */
    std::shared_ptr<Weapon> Inventory::getWeapon() const
    {
        auto item = atSlot(ItemSlotType::WEAPON);
        return std::dynamic_pointer_cast<Weapon>(item);
    }

    /**
     * @brief Get the shield from the inventory.
     * @return A shared pointer to the shield, or nullptr if not present.
     */
    std::shared_ptr<Item> Inventory::getShield() const
    {
        return atSlot(ItemSlotType::SHIELD);
    }

    /**
     * @brief Get the container from the inventory.
     * @return A shared pointer to the container, or nullptr if not present.
     */
    std::shared_ptr<Container> Inventory::getContainer() const
    {
        auto item = atSlot(ItemSlotType::CONTAINER);
        return std::dynamic_pointer_cast<Container>(item);
    }

    /**
     * @brief Drop an item from a specific slot.
     * @param slot The slot to drop the item from.
     * @return A shared pointer to the dropped item.
     */
    std::shared_ptr<Item> Inventory::dropItem(ItemSlotType slot)
    {
        auto &item = atSlot(slot);
        auto droppedItem = item;
        item = nullptr; // Elimina el item del slot
        return droppedItem;
    }

    /**
     * @brief Use an item from a specific slot.
     * @param slot The slot to use the item from.
     * @return True if the item was used successfully, false otherwise.
     */
    bool Inventory::useItem(ItemSlotType slot)
    {
        auto &item = atSlot(slot);
        if (item)
        {
            // Implementa la lógica para usar el item
            item = nullptr; // Ejemplo: elimina el item después de usarlo
            return true;
        }
        return false;
    }

    /**
     * @brief Get the total weight of items in the inventory.
     * @return The total weight of all items.
     */
    short Inventory::getWeight() const
    {
        short totalWeight = 0;
        for (const auto &item : _slots)
        {
            if (item)
            {
                totalWeight += item->getWeight(); // Asume que Item tiene un método getWeight()
            }
        }
        return totalWeight;
    }

    /**
     * @brief Store an item in a specific slot.
     * @param item The item to store.
     * @param slot The slot to store the item in.
     */
    void Inventory::storeItem(std::shared_ptr<Item> item, ItemSlotType slot)
    {
        if (!item)
        {
            LM.writeLog(Level::Warning, "Attempted to store null item");
            return;
        }

        if (!isValidItemForSlot(item, slot))
        {
            LM.writeLog(Level::Warning, "Item type " + std::to_string(static_cast<int>(item->getItemType())) + 
                       " is not valid for slot " + std::to_string(static_cast<int>(slot)));
            return;
        }

        // Handle special logic for two-handed weapons and shields
        handleTwoHandedWeaponLogic(item, slot);

        atSlot(slot) = item;
    }

    /**
     * @brief Handle the logic for two-handed weapons and shields.
     * @param item The item being equipped.
     * @param slot The slot where the item is being equipped.
     */
    void Inventory::handleTwoHandedWeaponLogic(std::shared_ptr<Item> item, ItemSlotType slot)
    {
        // If equipping a two-handed weapon, move shield to container or drop it
        if (slot == ItemSlotType::WEAPON)
        {
            auto weapon = std::dynamic_pointer_cast<Weapon>(item);
            if (weapon && weapon->isTwoHanded())
            {
                auto shield = getShield();
                if (shield)
                {
                    auto container = getContainer();
                    if (container)
                    {
                        // Try to put shield in container (simplified - real implementation would check container capacity)
                        LM.writeLog(Level::Info, "Moving shield to container due to two-handed weapon");
                        atSlot(ItemSlotType::SHIELD) = nullptr;
                        // TODO: Actually add to container inventory
                    }
                    else
                    {
                        // Drop shield to ground
                        LM.writeLog(Level::Info, "Dropping shield to ground due to two-handed weapon and no container");
                        atSlot(ItemSlotType::SHIELD) = nullptr;
                        // TODO: Add shield to world/ground
                    }
                }
            }
        }
        
        // If equipping a shield with a two-handed weapon equipped, move weapon to container or drop it
        if (slot == ItemSlotType::SHIELD)
        {
            auto weapon = getWeapon();
            if (weapon && weapon->isTwoHanded())
            {
                auto container = getContainer();
                if (container)
                {
                    // Try to put weapon in container
                    LM.writeLog(Level::Info, "Moving two-handed weapon to container due to shield");
                    atSlot(ItemSlotType::WEAPON) = nullptr;
                    // TODO: Actually add to container inventory
                }
                else
                {
                    // Drop weapon to ground
                    LM.writeLog(Level::Info, "Dropping two-handed weapon to ground due to shield and no container");
                    atSlot(ItemSlotType::WEAPON) = nullptr;
                    // TODO: Add weapon to world/ground
                }
            }
        }
    }

    /**
     * @brief Access a slot in the inventory.
     * @param slot The slot to access.
     * @return A reference to the shared pointer of the item in the slot.
     */
    std::shared_ptr<Item> &Inventory::atSlot(ItemSlotType slot)
    {
        return _slots[static_cast<size_t>(slot)];
    }

    /**
     * @brief Access a slot in the inventory (const version).
     * @param slot The slot to access.
     * @return A constant reference to the shared pointer of the item in the slot.
     */
    const std::shared_ptr<Item> &Inventory::atSlot(ItemSlotType slot) const
    {
        return _slots[static_cast<size_t>(slot)];
    }

    /**
     * @brief Check if an item is valid for a specific slot.
     * @param item The item to check.
     * @param slot The slot to check against.
     * @return True if the item can be stored in the slot, false otherwise.
     */
    bool Inventory::isValidItemForSlot(const std::shared_ptr<Item>& item, ItemSlotType slot) const
    {
        if (!item) return false;

        ItemType itemType = item->getItemType();
        
        switch (slot)
        {
            case ItemSlotType::AMULET:
                return itemType == ItemType::AMULET;
            case ItemSlotType::HELMET:
                return itemType == ItemType::HELMET;
            case ItemSlotType::CONTAINER:
                return itemType == ItemType::CONTAINER;
            case ItemSlotType::WEAPON:
                return itemType == ItemType::WEAPON;
            case ItemSlotType::RING_LEFT:
            case ItemSlotType::RING_RIGHT:
                return itemType == ItemType::RING;
            case ItemSlotType::ARMOR:
                return itemType == ItemType::ARMOR;
            case ItemSlotType::SHIELD:
                return itemType == ItemType::SHIELD;
            case ItemSlotType::LEGS_ARMOR:
                return itemType == ItemType::LEGS_ARMOR;
            case ItemSlotType::BOOTS:
                return itemType == ItemType::BOOTS;
            case ItemSlotType::AMMUNITION:
                return itemType == ItemType::AMMUNITION;
            default:
                return false;
        }
    }
}