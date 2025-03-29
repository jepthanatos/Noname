#ifndef __INVENTORY_H__
#define __INVENTORY_H__

// System includes
#include <unordered_map>
#include <vector>
#include <memory>

// Local includes
#include "Utils.h"
#include "Item.h"
#include "ItemEnumTypes.h"
#include "Property.h"
#include "Container.h"

namespace noname
{
    class Inventory
    {
    private:
        std::vector<std::shared_ptr<Item>> _slots;

        inline std::shared_ptr<Item> &atSlot(ItemSlotType slot)
        {
            return _slots.at(Utils::toInt(slot));
        }

        inline const std::shared_ptr<Item> &atSlot(ItemSlotType slot) const
        {
            return _slots.at(Utils::toInt(slot));
        }

    public:
        Inventory() : _slots(static_cast<size_t>(ItemSlotType::LAST_SLOT_TYPE)) {}

        const std::vector<std::shared_ptr<Item>> &getSlots() const { return _slots; }

        std::shared_ptr<Weapon> getWeapon() const
        {
            auto &itemPtr = atSlot(ItemSlotType::WEAPON);
            if (itemPtr)
            {
                return std::dynamic_pointer_cast<Weapon>(itemPtr);
            }
            return nullptr;
        }

        std::shared_ptr<Item> dropItem(ItemSlotType slot)
        {
            auto value = std::move(atSlot(slot));
            atSlot(slot).reset();
            return value;
        }

        bool useItem(ItemSlotType slot)
        {
            auto &item = atSlot(slot);
            if (item && item->getUses() > 0)
            {
                item->useItem();
                return true;
            }
            return false; // No se pudo usar el ítem
        }

        short getWeight() const
        {
            short weight{0};
            for (const auto &slot : _slots)
            {
                if (slot)
                    weight += slot->getWeight();
            }
            return weight;
        }

        void storeItem(std::shared_ptr<Item> item, ItemSlotType slot)
        {
            if (!item)
            {
                LM.writeLog(Level::Error, "Attempted to store a null item.");
                return;
            }

            if (item->getItemType() != slotTypeToItemType(slot))
            {
                LM.writeLog(Level::Error, "Item type does not match slot type.");
                return;
            }

            if (atSlot(slot))
            {
                if (atSlot(slot)->getName() == "Fists")
                {
                    atSlot(slot) = std::move(item);
                }
                else
                {
                    LM.writeLog(Level::Debug, "Inventory already has an item of type " + std::to_string(static_cast<int>(slot)));
                }
            }
            else
            {
                atSlot(slot) = std::move(item);
            }
        }
    };
}

#endif // __INVENTORY_H__
