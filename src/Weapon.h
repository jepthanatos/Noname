#ifndef __WEAPON_H__
#define __WEAPON_H__

// System includes
#include <string>

// Local includes
#include "Skill.h"
#include "Item.h"
#include "ItemEnumTypes.h"
#include "Property.h"

namespace noname
{
    class Weapon : public Item
    {
    private:
        SkillType _skillType;
        Property<short> _die;

    public:
        Weapon(const std::string &name,
               SkillType skillType,
               ItemRank rank = ItemRank::NORMAL,
               short die = 0,
               std::optional<short> value = std::nullopt,
               std::optional<short> uses = std::nullopt,
               std::optional<short> weight = std::nullopt)
            : Item{name, skillTypeToItemType(skillType), rank, value, uses, weight}, _skillType{skillType}, _die{die} {}

        SkillType getSkillType() const { return _skillType; }
        short getDie() const { return _die; }
    };
}

#endif // __WEAPON_H__
