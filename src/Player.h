#ifndef __PLAYER_H__
#define __PLAYER_H__

// System includes

// Local includes
#include "Character.h"
#include "Skill.h"
#include "HtmlBuilder.h"
#include "Utils.h"

namespace noname
{
    class Player : public Character
    {
    public:
        Player() : Character(){};
        Player(const std::string &name) : Character(name){};

        void getInfoForRanking(HtmlBuilder &builder, const SkillType &skill)
        {
            builder.add_child(HtmlBuilder{"tr"}.add_child("td", _id.toString()).add_child("td", _level.toString()).add_child("td", std::to_string(skills.at(Utils::toInt(skill)))));
        }
    };
}
#endif // __PLAYER_H__
