#ifndef __WEAPON_H__
#define __WEAPON_H__

// System includes
#include <string>

// Local includes
#include "Skill.h"
#include "Item.h"
#include "Property.h"

namespace noname
{
    class Weapon : public Item
    {

    private:
        SkillType _type;
        Property<short> _dice;

    public:
        Weapon() : _type{SkillType::FIST}, _dice{2}, Item("Fists", ItemType::WEAPON) {}
        Weapon(const std::string &name, const SkillType type, const short dice) : _type{type}, _dice{dice}, Item(name, ItemType::WEAPON) {}

        SkillType getType() const { return _type; }

        short getDice() const { return _dice; }
    };
}
#endif // __WEAPON_H__
