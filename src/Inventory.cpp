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
        // Implementación para obtener un arma del inventario
        for (const auto &item : _slots)
        {
            if (item && std::dynamic_pointer_cast<Weapon>(item))
            {
                return std::dynamic_pointer_cast<Weapon>(item);
            }
        }
        return nullptr;
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
        atSlot(slot) = item;
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
}