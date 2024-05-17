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
        short _id;
        std::string _name;
        SkillType _type;
        short _dice;

    public:
        Weapon(const std::string name, const SkillType type, const short dice) : _name{name}, _type{type}, _dice{dice}
        {
        }

        virtual ~Weapon(){};

        std::string getName() const { return _name; }

        SkillType getType() const { return _type; }

        short getDice() const { return _dice; }
    };

}
#endif // __WEAPON_H__
