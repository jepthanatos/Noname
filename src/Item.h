#ifndef __ITEM_H__
#define __ITEM_H__

// System includes
#include <string>

// Local includes
#include "Skill.h"

namespace noname
{
    enum class ItemType
    {
        WEAPON,
        ARMOR,
        CONTAINER,
        USABLE,
        LAST_ITEM_TYPE
    };

    class Item
    {

    protected:
        int _id;
        std::string _name;
        ItemType _type;

    public:
        Item(const std::string &name, const ItemType type) : _name{name}, _type{type}
        {
            static int cont{0};
            _id = cont;
            ++cont;
        }

        std::string getName() const { return _name; }

        ItemType getItemType() const { return _type; }

        bool operator==(const Item &item) const
        {
            // return item._id == _id;
            return item._name == _name;
        }
    };

}
#endif // __ITEM_H__
