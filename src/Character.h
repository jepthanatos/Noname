#ifndef __CHARACTER_H__
#define __CHARACTER_H__

// System includes
#include <unordered_map>
#include <string>

// Local includes
#include "Skill.h"
#include "Weapon.h"
#include "WeaponsManager.h"

namespace noname
{
    class Character
    {
    private:
        int _id;
        std::string _name;
        unsigned long long _currentExp;
        unsigned long long _nextLevelExp;
        unsigned long long _currentManaWasted;
        unsigned long long _nextLevelManaWasted;
        short _level;
        short _magicLevel;
        int _currentHealth;
        int _maxHealth;
        int _currentMana;
        int _maxMana;
        std::unordered_map<SkillType, short> _skills;
        std::unordered_map<SkillType, short> _skillTries;
        int _currentCapacity;
        int _maxCapacity;
        Weapon _currentWeapon;

        void setLevel(short value);
        void setMagicLevel(short value);
        unsigned long long getManaForLevel(short level) const;
        void setHealth();
        void setMana();
        void setSkill(SkillType skill, short value);
        void updateTries(SkillType skill);

    public:
        Character();
        Character(const std::string &name) : _name{name} { Character(); }
        bool operator==(const Character &p) const { return _id == p._id; }
        int getId() const { return _id; }
        std::string getName() const { return _name; }
        unsigned long long getExperience() const { return _currentExp; }
        static unsigned long long getExpForLevel(short level);
        void addExperience(unsigned long long exp);
        short getLevel() const { return _level; }
        short getSkill(SkillType skill) const { return _skills.find(skill)->second; }
        short getCurrentHealth() const { return _currentHealth; }
        short getAttackDamage() const;
        void takeDamage(short damage);
        Weapon getWeapon() const { return _currentWeapon; }
        void equipWeapon(const std::string &weapon) { _currentWeapon = WM.getWeapon(weapon); }

        // Actions
        virtual void move() {}
        virtual void attack();
        virtual void defense() {}
        virtual void eat() {}
        virtual void sleep() {}
        virtual void pick() {}
        virtual void drop() {}
        virtual void use() {}
    };
}
#endif // __CHARACTER_H__
