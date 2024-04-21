#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <unordered_map>
#include <string>
#include "Skill.h"

namespace df
{
    class Player
    {
    public:
        int m_id;
        std::string m_name;
        std::unordered_map<SkillType, int> m_skills;

        Player(std::string name, SkillType skill, int value) : m_name{name}
        {
            static int cont{0};
            m_id = cont;
            ++cont;
            m_skills.emplace(skill, value);
        }

        std::string getName() const { return m_name; }
        int getSkill(SkillType s) const { return m_skills.find(s)->second; }

        bool operator==(const Player& p) const
        {
            return m_id == p.m_id;
        }
    };
} // end of namespace df
#endif // __PLAYER_H__