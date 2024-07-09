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
        Weapon(const std::string &name, const SkillType &type, const ItemRank &rank = ItemRank::NORMAL, const short &die = 0) : _type{type}, _die{die}, Item(name, ItemType::WEAPON, rank) {}

        SkillType getSkillType() const { return _type; }
        short getDie() const { return _die; }
    };
}
#endif // __WEAPON_H__
