//==============================================================================
// Name        : Player.cpp
// Author      : Jorge
// Copyright   : https://creativecommons.org/licenses/by/4.0/
// Description : Player class
//==============================================================================

#include "Player.h"

// System includes
#include <cmath>
#include <math.h>

//==============================================================================
/* Player */
//==============================================================================

namespace noname
{
    Player::Player()
    {
        static int cont{0};
        m_id = cont;
        ++cont;
        for (int i = 0; i < static_cast<int>(SkillType::LAST_SKILL); ++i)
        {
            skills.emplace(static_cast<SkillType>(i), 1);
            skillTries.emplace(static_cast<SkillType>(i), 0);
            weaponAttack.emplace(static_cast<SkillType>(i), 10);
        }
        setLevel(1);
    }

    short Player::getLevel() const
    {
        return level;
    }

    void Player::setLevel(short value)
    {
        level = value;
        setHealth();
        experience = getExpForLevel(level);
    }

    void Player::setHealth()
    {
        // Depends on vocation
        // Paladin
        health = 5 * ((2 * level) + 21);
        // Knight
        // health = 5 * ((3 * level) + 13);
        // Sorcerer and Druid
        // health = 5 * (level + 29);
    }

    short Player::getSkill(SkillType skill) const
    {
        return skills.find(skill)->second;
    }
    
    void Player::setSkill(SkillType skill, short value)
    {
        skills.find(skill)->second = value;
    }

    void Player::updateTries(SkillType skill)
    {
        skillTries.find(skill)->second++;
        if (skillTries.find(skill)->second == MELEE_SKILLS)
        {
            skillTries.find(skill)->second = 0;
            skills.find(skill)->second++;
        }
    }

    short Player::getMaxDamage(int attackSkill, int weaponDamage, int attackFactor) const
    {
        return ceil(2 * (weaponDamage * ((attackSkill + 5.8) / 25) + (level / 10) - 0.1) / attackFactor);
    }

    short Player::getDamage(SkillType skill) const
    {
        short maxDamage{getMaxDamage(skills.find(skill)->second, weaponAttack.find(skill)->second, 1)};

        return maxDamage;
    }

    short Player::getHealth() const
    {
        return health;
    }

    void Player::takeDamage(short damage)
    {
        health -= damage;
        if (health <= 0)
        {
            experience -= ceil((experience * 25) / 100);
            if (experience < getExpForLevel(level))
                level--;
        }
    }

    void Player::addExperience(long exp)
    {
        experience += exp;
        short prevLevel = getLevel();
        short newLevel = getLevel();

        long currLevelExp = Player::getExpForLevel(newLevel);
        long nextLevelExp = Player::getExpForLevel(newLevel + 1);
        while (experience >= nextLevelExp)
        {
            ++newLevel;
            setHealth();
            //healthMax += vocation->getHPGain();
            //health += vocation->getHPGain();
            //manaMax += vocation->getManaGain();
            //mana += vocation->getManaGain();
            //capacity += vocation->getCapGain();
            nextLevelExp = Player::getExpForLevel(newLevel + 1);
        }
    }

    unsigned long long Player::getExperience() const
    {
        return experience;
    }

    unsigned long long Player::getExpForLevel(short level) const
    {
        return ((50ULL * level * level * level) - (150ULL * level * level) + (400ULL * level)) / 3ULL;
    }
}
