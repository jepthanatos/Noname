#ifndef __PLAYER_H__
#define __PLAYER_H__

// System includes
#include <unordered_map>
#include <string>

// Local includes
#include "Skill.h"
#include "Weapon.h"

namespace noname
{
    class Player
    {
    private:
        int _id;
        std::unordered_map<SkillType, short> _skills;
        std::unordered_map<SkillType, short> _skillTries;
        Weapon _weapon;
        short _level;
        int _healthMax;
        int _health;
        int _manaMax;
        int _mana;
        int _capacity;
        unsigned long long _currentLevelExp;
        unsigned long long _nextLevelExp;

    public:
        Player();
        int getId() const { return _id; }
        short getLevel() const;
        void setLevel(short value);
        void setHealth();
        short getSkill(SkillType skill) const;
        void setSkill(SkillType skill, short value);
        void updateTries(SkillType skill);
        short getDamage(SkillType skill) const;
        short getHealth() const;
        void takeDamage(short damage);
        void addExperience(long exp);
        unsigned long long getExperience() const;
        unsigned long long getExpForLevel(short level) const;
        bool operator==(const Player &p) const
        {
            return _id == p._id;
        }
    };
}
#endif // __PLAYER_H__
