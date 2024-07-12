#ifndef __INVENTORY_H__
#define __INVENTORY_H__

// System includes
#include <unordered_map>
#include <optional>
#include <vector>

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
        std::vector<std::optional<Item>> _slots;

    public:
        Inventory() : _slots(static_cast<size_t>(ItemSlotType::LAST_SLOT_TYPE), std::nullopt) {}

        std::vector<std::optional<Item>> getSlots() const { return _slots; }
        std::optional<Item> getItem(ItemSlotType slot)
        {
            auto value{std::move(_slots.at(Utils::toInt(slot)))};
            if (_slots.at(Utils::toInt(slot)).has_value())
                _slots.at(Utils::toInt(slot)) = std::nullopt;
            return value;
        }

        void useItem(ItemSlotType slot)
        {
            if (_slots.at(Utils::toInt(slot)).has_value())
                if (_slots.at(Utils::toInt(slot)).value().getUses() > 0)
                    _slots.at(Utils::toInt(slot)).value().useItem();
        }

        short getWeight()
        {
            short weight{0};
            for (auto slot : _slots)
            {
                if (slot.has_value())
                    weight += slot.value().getWeight();
            }
            return weight;
        }

        void storeItem(Item &item, ItemSlotType slot)
        {
            if (item.getItemType() == slotTypeToItemType(slot))
            {
                if (!_slots.at(Utils::toInt(slot)).has_value())
                {
                    _slots.at(Utils::toInt(slot)) = std::move(item);
                }
            }
            else
            {
                // Error handling TBD
            }
        }
    };
}
#endif // __INVENTORY_H__
