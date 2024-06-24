#ifndef __CHARACTER_H__
#define __CHARACTER_H__

// System includes
#include <vector>
#include <string>

// Local includes
#include "Property.h"
#include "Skill.h"
#include "Weapon.h"
#include "WeaponsManager.h"
#include "Heritables.h"

namespace noname
{
    class Character
    {
    protected:
        Property<int> _id;
        Property<std::string> _name;
        Property<short> _level;
        Property<short> _magicLevel;
        Property<int> _currentHealth;
        Property<int> _maxHealth;
        Property<int> _currentMana;
        Property<int> _maxMana;
        Property<int> _currentCapacity;
        Property<int> _maxCapacity;
        Property<unsigned long long> _currentExperience;
        Property<unsigned long long> _nextLevelExperience;
        Property<unsigned long long> _currentManaWasted;
        Property<unsigned long long> _nextLevelManaWasted;
        std::vector<Property<short>> _skills;
        std::vector<Property<short>> _skillTries;
        Weapon _currentWeapon;
        bool _isDead;
        Heritables _heritables;

        void setLevel(short value);
        void setMagicLevel(short value);
        unsigned long long getManaForLevel(short level) const;
        void setHealth();
        void setMana();
        void setSkill(SkillType skill, short value);
        void updateTries(SkillType skill);

    public:
        Character();
        Character(const std::string &name) : Character() { _name = name; };
        bool operator==(const Character &p) const { return _id == p._id; }
        int getId() const { return _id; }
        std::string getName() const { return _name; }
        unsigned long long getExperience() { return _currentExperience; }
        static unsigned long long getExpForLevel(short level);
        void addExperience(unsigned long long exp);
        short getLevel() const { return _level; }
        short getMagicLevel() const { return _magicLevel; }
        short getSkill(SkillType skill) const { return _skills.at(static_cast<int>(skill)); }
        int getCurrentHealth() const { return _currentHealth; }
        int getMaxHealth() const { return _maxHealth; }
        short getHeritable(HeritableType value) { return _heritables.at(value); }
        bool isDead() { return _isDead; }
        void respawn();
        short getAttackDamage() const;
        void takeDamage(int damage);
        Weapon getWeapon() const { return _currentWeapon; }
        void equipWeapon(const std::string &weapon) { _currentWeapon = WM.getWeapon(weapon); }
        void writeCharacterInfo();

        // Actions
        virtual void move() {}
        virtual void attack();
        virtual void defense() {}
        virtual void eat() {}
        virtual void sleep() {}
        virtual void pick() {}
        virtual void drop() {}
        virtual void use() {}

        // Others
        void determineHeritables(const Character &father, const Character &mother);
    };
}
#endif // __CHARACTER_H__
