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
            return "Player ID: " + std::to_string(getId());
        }
        inline std::string getLevelForRanking() const
        {
            return "Level: " + std::to_string(getLevel());
        }
        inline std::string getSkillForRanking(const SkillType &skill) const
        {
            return "Skill: " + std::to_string(getSkill(skill));
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
