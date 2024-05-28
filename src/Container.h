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
        Property<short> _slots;
        Property<short> _maxCapacity;

    public:
        Container(const std::string &name, const ContainerType type, short slots, short maxCapacity) : _type{type}, _slots{slots}, _maxCapacity{maxCapacity}, Item(name, ItemType::CONTAINER) {}

        ContainerType getType() const { return _type; }
        short getSlots() const { return _slots; }
        short getMaxCapacity() const { return _maxCapacity; }
    };

}
#endif // __CONTAINER_H__
