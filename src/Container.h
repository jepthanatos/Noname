#ifndef __CONTAINER_H__
#define __CONTAINER_H__

// System includes
#include <string>
#include <vector>
#include <optional>

// Local includes
#include "Item.h"
#include "Property.h"
#include "ItemEnumTypes.h"

namespace noname
{
    enum class ContainerType
    {
        BACKPACK,
        LAST_CONTAINER_TYPE
    };

    class Container : public Item
    {
    private:
        ContainerType _type;
        Property<short> _slotsNumber;
        Property<short> _maxCapacity;
        std::vector<std::optional<Item>> _slots;

    public:
        Container(const std::string &name, ContainerType type, ItemRank rank, short slots, short maxCapacity)
            : Item{name, ItemType::CONTAINER, rank}, _type{type}, _slotsNumber{slots}, _maxCapacity{maxCapacity}, _slots(slots, std::nullopt) {}

        ContainerType getType() const { return _type; }
        short getSlotsNumber() const { return _slotsNumber; }
        short getMaxCapacity() const { return _maxCapacity; }

        short getWeight() const override
        {
            short weight{0};
            for (auto slot : _slots)
            {
                if (slot.has_value())
                    weight += slot->getWeight();
            }
            return weight;
        }

        bool addItem(Item &item)
        {
            for (auto &slot : _slots)
            {
                if (!slot.has_value())
                {
                    slot = std::move(item);
                    return true;
                }
            }
            return false;
        }

        std::optional<Item> removeItem(size_t index)
        {
            if (index >= _slots.size() || !_slots[index].has_value())
                return std::nullopt;

            auto item = std::move(_slots[index]);
            _slots[index] = std::nullopt;
            return item;
        }
    };
}
#endif // __CONTAINER_H__
