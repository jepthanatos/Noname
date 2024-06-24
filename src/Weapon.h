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
        Property<short> _die;

    public:
        Weapon() : _type{SkillType::FIST}, _die{2}, Item("Fists", ItemType::WEAPON) {}
        Weapon(const std::string &name, const SkillType type, const short dice) : _type{type}, _die{dice}, Item(name, ItemType::WEAPON) {}

        SkillType getType() const { return _type; }
        short getDie() const { return _die; }
    };
}
#endif // __WEAPON_H__
