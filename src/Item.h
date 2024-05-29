#ifndef __ITEM_H__
#define __ITEM_H__

// System includes
#include <string>
#include <optional>

// Local includes
#include "Skill.h"
#include "Property.h"

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

    inline constexpr short NO_VALUE = -1;

    class Item
    {
    protected:
        int _id;
        std::string _name;
        ItemType _type;
        std::optional<Property<short>> _value;
        std::optional<Property<short>> _uses;
        std::optional<Property<short>> _weight;

        short getValueFromOptional(const std::optional<Property<short>> &optional) const
        {
            if (not optional.has_value())
                return NO_VALUE;
            else
                return optional.value();
        };

    public:
        Item(const std::string &name, const ItemType type, const std::optional<short> value = std::nullopt, const std::optional<short> uses = std::nullopt, const std::optional<short> weight = std::nullopt) : _name{name}, _type{type}, _value{value}, _uses{uses}, _weight{weight}
        {
            static int cont{0};
            _id = cont;
            ++cont;
        }

        std::string getName() const { return _name; }
        ItemType getItemType() const { return _type; }

        short getValue() const { return getValueFromOptional(_value); }
        short getUses() const { return getValueFromOptional(_uses); }
        short getWeight() const { return getValueFromOptional(_weight); }

        bool operator==(const Item &item) const
        {
            // return item._id == _id;
            return item._name == _name;
        }
    };

}
#endif // __ITEM_H__
