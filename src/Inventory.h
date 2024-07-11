#ifndef __INVENTORY_H__
#define __INVENTORY_H__

// System includes
#include <unordered_map>
#include <optional>
#include <vector>

// Local includes
#include "Utils.h"
#include "Item.h"
#include "Property.h"
#include "Container.h"

namespace noname
{
    enum class SlotType
    {
        AMULET,
        HELMET,
        CONTAINER,
        WEAPON,
        RIGHT_RING,
        ARMOR,
        SHIELD,
        LEFT_RING,
        LEGS_ARMOR,
        BOOTS,
        AMMUNITION,
        LAST_SLOT_TYPE
    };

    class Inventory
    {

    private:
        std::vector<std::optional<Item>> _slots;

    public:
        Inventory() : _slots{std::nullopt} {}

        std::vector<std::optional<Item>> getSlots() const { return _slots; }
        std::optional<Item> getItem(SlotType slot)
        {
            auto value{std::move(_slots.at(Utils::toInt(slot)))};
            if (_slots.at(Utils::toInt(slot)).has_value())
                _slots.at(Utils::toInt(slot)) = std::nullopt;
            return value;
        }

        void useItem(SlotType slot)
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

        void storeItem(Item &item, SlotType slot)
        {
            static const std::unordered_map<SlotType, ItemType> slotToItemType{
                {SlotType::AMULET, ItemType::AMULET},
                {SlotType::HELMET, ItemType::HELMET},
                {SlotType::CONTAINER, ItemType::CONTAINER},
                {SlotType::WEAPON, ItemType::WEAPON},
                {SlotType::RIGHT_RING, ItemType::RING},
                {SlotType::LEFT_RING, ItemType::RING},
                {SlotType::ARMOR, ItemType::ARMOR},
                {SlotType::SHIELD, ItemType::SHIELD},
                {SlotType::LEGS_ARMOR, ItemType::LEGS_ARMOR},
                {SlotType::BOOTS, ItemType::BOOTS},
                {SlotType::AMMUNITION, ItemType::AMMUNITION}};

            auto it = slotToItemType.find(slot);
            if (it != slotToItemType.end() && item.getItemType() == it->second)
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
