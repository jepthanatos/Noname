#ifndef __INVENTORY_H__
#define __INVENTORY_H__

// System includes
#include <string>

// Local includes
#include "Item.h"
#include "Property.h"

namespace noname
{
    enum class SlotType
    {
        AMULET,
        HELMET,
        BACKPACK,
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
        std::vector<Item> _slots;

    public:
        Inventory() : _slots{} {}

        std::vector<Item> getSlots() const { return _slots; }

        int getWeight()
        {
            int weight{0};
            for (auto slot : _slots)
            {
                weight += slot.getWeight();
            }
            return weight;
        }
    };

}
#endif // __INVENTORY_H__
