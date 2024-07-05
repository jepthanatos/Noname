#ifndef __INVENTORY_H__
#define __INVENTORY_H__

// System includes
#include <string>

// Local includes
#include "Utils.h"
#include "Item.h"
#include "Property.h"

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
        BOOTS
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

        int getWeight()
        {
            int weight{0};
            for (auto slot : _slots)
            {
                if (slot.has_value())
                    weight += slot.value().getWeight();
            }
            return weight;
        }

        void storeItem(Item &item, SlotType slot)
        {
            auto store = [&](Item &thisItem)
            {
                if (not _slots.at(Utils::toInt(slot)).has_value())
                {
                    _slots.at(Utils::toInt(slot)) = std::move(thisItem);
                }
            };

            switch (slot)
            {
            case SlotType::AMULET:
                if (item.getItemType() == ItemType::AMULET)
                {
                    store(item);
                }
                break;
            case SlotType::HELMET:
                if (item.getItemType() == ItemType::HELMET)
                {
                    store(item);
                }
                break;
            case SlotType::CONTAINER:
                if (item.getItemType() == ItemType::CONTAINER)
                {
                    store(item);
                }
                break;
            case SlotType::WEAPON:
                if (item.getItemType() == ItemType::WEAPON)
                {
                    store(item);
                }
                break;
            case SlotType::RIGHT_RING:
                if (item.getItemType() == ItemType::RING)
                {
                    store(item);
                }
                break;
            case SlotType::ARMOR:
                if (item.getItemType() == ItemType::ARMOR)
                {
                    store(item);
                }
                break;
            case SlotType::SHIELD:
                if (item.getItemType() == ItemType::SHIELD)
                {
                    store(item);
                }
                break;
            case SlotType::LEFT_RING:
                if (item.getItemType() == ItemType::RING)
                {
                    store(item);
                }
                break;
            case SlotType::LEGS_ARMOR:
                if (item.getItemType() == ItemType::LEGS_ARMOR)
                {
                    store(item);
                }
                break;
            case SlotType::BOOTS:
                if (item.getItemType() == ItemType::BOOTS)
                {
                    store(item);
                }
                break;
            default:
                // Error handling TBD
                break;
            }
        }
    };

}
#endif // __INVENTORY_H__
