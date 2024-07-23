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
#include "Inventory.h"
#include "ItemEnumTypes.h"

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
        Property<int> _baseSpeed;
        Property<int> _speed;
        Property<unsigned long long> _currentExperience;
        Property<unsigned long long> _nextLevelExperience;
        Property<unsigned long long> _currentManaWasted;
        Property<unsigned long long> _nextLevelManaWasted;
        std::vector<Property<short>> _skills;
        std::vector<Property<short>> _skillTries;
        bool _isDead;
        Heritables _heritables;
        Inventory _inventory;

        static int generateId();
        void setLevel(short value);
        void setMagicLevel(short value);
        void setMaxHealth();
        void setMaxMana();
        void setMaxCapacity();
        void updateCurrentCapacity();
        void setSpeed();
        void updateSpeed();
        void setSkill(SkillType skill, short value);
        void updateTries(SkillType skill);

    public:
        Character();
        Character(const std::string &name) : Character() { _name = name; }

        bool operator==(const Character &p) const { return _id == p._id; }

        // Getters for UI or info related
        int getId() const { return _id; }
        std::string getName() const { return _name; }
        unsigned long long getExperience() const { return _currentExperience; }
        unsigned long long getManaWasted() const { return _currentManaWasted; }
        short getLevel() const { return _level; }
        short getMagicLevel() const { return _magicLevel; }
        short getSkill(SkillType skill) const { return _skills.at(static_cast<int>(skill)); }
        int getCurrentHealth() const { return _currentHealth; }
        int getMaxHealth() const { return _maxHealth; }
        int getCurrentMana() const { return _currentMana; }
        int getMaxMana() const { return _maxMana; }
        bool isDead() { return _isDead; }
        short getHeritable(HeritableType value) { return _heritables.at(value); }
        short getAttackDamage();
        std::shared_ptr<Weapon> getWeapon() { return _inventory.getWeapon(); }
        void writeCharacterInfo();
        const std::vector<std::shared_ptr<Item>> &getInventorySlots() const { return _inventory.getSlots(); }

        // Others
        void gainExperience(int value);
        void gainHealth(int value);
        void takeDamage(int value);
        void useMana(int value);
        void gainMana(int value);
        void equipWeapon(std::shared_ptr<Weapon> weapon) { pick(weapon, ItemSlotType::WEAPON); }

        // Actions
        virtual void respawn();
        virtual void move() {}
        virtual void attack(Character &character);
        virtual void defense(short damage);
        virtual void pick(std::shared_ptr<Item> item, ItemSlotType slot);
        virtual void drop(int itemID);
        virtual void drop(ItemSlotType slot);
        virtual void use(int itemID) {}

        // Others
        void determineHeritables(const Character &father, const Character &mother);
    };
}
#endif // __CHARACTER_H__
