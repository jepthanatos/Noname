// Local includes
#include "Container.h"

namespace noname
{
    short Container::getWeight() const
    {
        short weight{0};
        for (const auto &slot : _slots)
        {
            if (slot)
                weight += slot->getWeight();
        }
        return weight;
    }

    bool Container::addItem(std::shared_ptr<Item> item)
    {
        for (auto &slot : _slots)
        {
            if (!slot)
            {
                slot = std::move(item);
                return true;
            }
        }
        return false;
    }

    std::shared_ptr<Item> Container::removeItem(size_t index)
    {
        if (index >= _slots.size() || !_slots[index])
            return nullptr;

        auto item = std::move(_slots[index]);
        _slots[index] = nullptr;
        return item;
    }

} // Closing brace for namespace noname
