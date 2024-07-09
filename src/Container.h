#ifndef __CONTAINER_H__
#define __CONTAINER_H__

// System includes
#include <string>

// Local includes
#include "Container.h"
#include "Item.h"
#include "Property.h"

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
        Container(const std::string &name, const ContainerType &type, const ItemRank &rank, short slots, short maxCapacity) : _type{type}, _slotsNumber{slots}, _maxCapacity{maxCapacity}, Item(name, ItemType::CONTAINER, rank) {}

        ContainerType getType() const { return _type; }
        short getSlotsNumber() const { return _slotsNumber; }
        short getMaxCapacity() const { return _maxCapacity; }
        short getWeight() const override
        {
            short weight{0};
            for (auto slot : _slots)
            {
                if (slot.has_value())
                    weight += slot.value().getWeight();
            }
            return weight;
        }
    };

}
#endif // __CONTAINER_H__
