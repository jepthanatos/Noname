#ifndef __ITEM_H__
#define __ITEM_H__

// System includes
#include <string>
#include <optional>
#include <atomic>

// Local includes
#include "ItemEnumTypes.h"
#include "Skill.h"
#include "Property.h"

namespace noname
{
    inline constexpr short NO_VALUE = -1;

    class Item
    {
    protected:
        int _id;
        std::string _name;
        ItemType _type;
        ItemRank _rank;
        std::optional<Property<short>> _value;
        std::optional<Property<short>> _uses;
        std::optional<Property<short>> _weight;

        short getValueFromOptional(const std::optional<Property<short>> &optional) const
        {
            return optional.has_value() ? static_cast<short>(optional.value()) : NO_VALUE;
        };

        auto generateId()
        {
            static std::atomic<int> cont{0};
            return cont++;
        };

    public:
        Item(const std::string &name,
             ItemType type,
             ItemRank rank = ItemRank::NORMAL,
             std::optional<short> value = std::nullopt,
             std::optional<short> uses = std::nullopt,
             std::optional<short> weight = std::nullopt)
            : _id{generateId()},
              _name{name},
              _type{type},
              _rank{rank},
              _value{value ? std::make_optional<Property<short>>(*value) : std::nullopt},
              _uses{uses ? std::make_optional<Property<short>>(*uses) : std::nullopt},
              _weight{weight ? std::make_optional<Property<short>>(*weight) : std::nullopt}
        {
        }

        // Default copy constructor and assignment operator
        Item(const Item &other) = default;
        Item &operator=(const Item &other) = default;

        // Destructor
        virtual ~Item() = default;

        // Move constructor and assignment operator
        Item(Item &&other) noexcept = default;
        Item &operator=(Item &&other) noexcept = default;

        std::string getName() const { return _name; }
        ItemType getItemType() const { return _type; }
        short getValue() const { return getValueFromOptional(_value); }
        short getUses() const { return getValueFromOptional(_uses); }
        virtual short getWeight() const { return getValueFromOptional(_weight); }

        bool operator==(const Item &item) const
        {
            return _id == item._id;
        }

        void useItem()
        {
            if (_uses && _uses.value() > short(0))
                --_uses.value();
        }
    };
}
#endif // __ITEM_H__
