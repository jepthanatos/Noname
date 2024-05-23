#ifndef __PLAYER_H__
#define __PLAYER_H__

// System includes

// Local includes
#include "Character.h"
#include "Skill.h"

namespace noname
{
    class Player : public Character
    {
    private:
        inline std::string getIdForRanking() const
        {
            return "Player ID: " + _id.toString();
        }
        inline std::string getLevelForRanking() const
        {
            return "Level: " + _level.toString();
        }
        inline std::string getSkillForRanking(const SkillType &skill) const
        {
            return "Skill: " + _skills.find(skill)->second.toString();
        }

    public:
        Player();
        std::string getInfoForRanking(const SkillType &skill)
        {
            static const std::string separator{" | "};
            return getIdForRanking() + separator + getLevelForRanking() + separator + getSkillForRanking(skill) + "\n";
        }
    };
}
#endif // __PLAYER_H__
