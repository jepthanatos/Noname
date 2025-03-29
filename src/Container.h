#ifndef __CONTAINER_H__
#define __CONTAINER_H__

// System includes
#include <string>
#include <vector>
#include <optional>
#include <memory>

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
        ContainerType _containerType;
        Property<short> _slotsNumber;
        Property<short> _maxCapacity;
        std::vector<std::shared_ptr<Item>> _slots;

    public:
        Container(const std::string &name, ContainerType containerType, ItemRank rank, short slots, short maxCapacity)
            : Item{name, ItemType::CONTAINER, rank}, _containerType{containerType}, _slotsNumber{slots}, _maxCapacity{maxCapacity}, _slots(static_cast<size_t>(slots)) {}

        ContainerType getContainerType() const { return _containerType; }
        short getSlotsNumber() const { return _slotsNumber; }
        short getMaxCapacity() const { return _maxCapacity; }
        short getWeight() const override;
        bool addItem(std::shared_ptr<Item> item);
        std::shared_ptr<Item> removeItem(size_t index);
    };
}
#endif // __CONTAINER_H__
