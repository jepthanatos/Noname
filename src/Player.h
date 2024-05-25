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
            builder.add_child("tr", HtmlBuilder{"td", _id.toString()}.str() +
                                        HtmlBuilder{"td", _level.toString()}.str() +
                                        HtmlBuilder{"td", _skills.find(skill)->second.toString()}.str());
        }
    };
}
#endif // __PLAYER_H__
