#ifndef __CREATURE_H__
#define __CREATURE_H__

// System includes
#include <string>

// Local includes
#include "Character.h"
#include "Skill.h"
#include "HtmlBuilder.h"
#include "Utils.h"

namespace noname
{
    enum class CreatureType
    {
        BEAST,
        LAST_CREATURE_TYPE
    };

    class Creature : public Character
    {
    private:
        CreatureType _type;

    public:
        Creature() : Character() {}
        Creature(const std::string &name, const CreatureType &type) : Character(name), _type{type} {}

        void getInfoForRanking(HtmlBuilder &builder, const SkillType &skill)
        {
            builder.add_child(HtmlBuilder{"tr"}.add_child("td", _id.toString()).add_child("td", _level.toString()).add_child("td", std::to_string(skills.at(Utils::toInt(skill)))));
        }
    };
}
#endif // __CREATURE_H__
