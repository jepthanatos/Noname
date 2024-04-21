//==============================================================================
// Name        : Player.h
// Author      : Jorge
// Copyright   : https://creativecommons.org/licenses/by/4.0/
// Description : Manager of the logfile.
//==============================================================================

#ifndef __PLAYER_H__
#define __PLAYER_H__

// System includes.
#include <unordered_map>
#include <string>

// Engine includes.
#include "Skill.h"

//==============================================================================
/* Player */
//==============================================================================

/**
 * Player class.
 */
namespace noname
{
    class Player
    {
    private:
        int m_id;
        std::unordered_map<SkillType, short> skills;
        std::unordered_map<SkillType, short> skillTries;
        std::unordered_map<SkillType, short> weaponAttack;
        short level;
        unsigned long long experience;
        int healthMax;
        int health;
        int manaMax;
        int mana;
        int capacity;
        // Vocation vocation;

    public:
        Player();
        int getId() const { return m_id; }
        short getLevel() const;
        void setLevel(short value);
        void setHealth();
        short getSkill(SkillType skill) const;
        void setSkill(SkillType skill, short value);
        void updateTries(SkillType skill);
        short getMaxDamage(int attackSkill, int attackValue, int attackFactor) const;
        short getDamage(SkillType skill) const;
        short getHealth() const;
        void takeDamage(short damage);
        void addExperience(long exp);
        unsigned long long getExperience() const;
        unsigned long long getExpForLevel(short level) const;
        bool operator==(const Player& p) const
        {
            return m_id == p.m_id;
        }
    };
}
//------------------------------------------------------------------------------
#endif // __PLAYER_H__
