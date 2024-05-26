#ifndef __PLAYER_H__
#define __PLAYER_H__

// System includes

// Local includes
#include "Character.h"
#include "Skill.h"
#include "HtmlBuilder.h"

namespace noname
{
    class Player : public Character
    {
    public:
        Player();
        void getInfoForRanking(HtmlBuilder &builder, const SkillType &skill)
        {
            builder.add_child(HtmlBuilder{"tr"}.add_child("td", _id.toString()).add_child("td", _level.toString()).add_child("td", _skills.find(skill)->second.toString()));
        }
    };
}
#endif // __PLAYER_H__
