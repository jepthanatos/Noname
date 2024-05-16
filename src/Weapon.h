#ifndef __WEAPON_H__
#define __WEAPON_H__

// System includes
#include <string>

// Local includes
#include "Skill.h"

namespace noname
{
    class Weapon
    {

    private:
        short _id; // Unique game engine defined identifier
        SkillType _type;
        short _dice;

    public:
        Weapon(SkillType type, short dice) : _type{type}, _dice{dice}
        {
        }

        virtual ~Weapon(){};

        SkillType getType() const { return _type; }

        short getDice() const { return _dice; }
    };

}
#endif // __WEAPON_H__
