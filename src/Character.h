#ifndef __CHARACTER_H__
#define __CHARACTER_H__

// System includes
#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <memory>

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
        bool _isDead{false};
        Heritables _heritables;
        Inventory _inventory;

        static int generateId() noexcept;
        void setLevel(short value) noexcept;
        void setMagicLevel(short value) noexcept;
        void setMaxHealth() noexcept;
        void setMaxMana() noexcept;
        void setMaxCapacity() noexcept;
        void updateCurrentCapacity() noexcept;
        void setSpeed() noexcept;
        void updateSpeed() noexcept;
        void setSkill(SkillType skill, short value) noexcept;
        void updateTries(SkillType skill) noexcept;

    public:
        Character();
        explicit Character(std::string_view name) : Character() { _name = std::string(name); }

        bool operator==(const Character &p) const noexcept { return _id == p._id; }

        [[nodiscard]] int getId() const noexcept { return _id; }
        [[nodiscard]] std::string getName() const noexcept { return _name; }
        [[nodiscard]] unsigned long long getExperience() const noexcept { return _currentExperience; }
        [[nodiscard]] unsigned long long getManaWasted() const noexcept { return _currentManaWasted; }
        [[nodiscard]] short getLevel() const noexcept { return _level; }
        [[nodiscard]] short getMagicLevel() const noexcept { return _magicLevel; }
        [[nodiscard]] short getSkill(SkillType skill) const noexcept { return _skills.at(static_cast<int>(skill)); }
        [[nodiscard]] int getCurrentHealth() const noexcept { return _currentHealth; }
        [[nodiscard]] int getMaxHealth() const noexcept { return _maxHealth; }
        [[nodiscard]] int getCurrentMana() const noexcept { return _currentMana; }
        [[nodiscard]] int getMaxMana() const noexcept { return _maxMana; }
        [[nodiscard]] bool isDead() const noexcept { return _isDead; }
        [[nodiscard]] short getHeritable(HeritableType value) const noexcept { return _heritables.at(value); }
        [[nodiscard]] short getAttackDamage() noexcept;
        [[nodiscard]] std::shared_ptr<Weapon> getWeapon() const noexcept { return _inventory.getWeapon(); }
        void writeCharacterInfo() const;
        [[nodiscard]] const InventorySlots &getInventorySlots() const noexcept { return _inventory.getSlots(); }

        // Others
        void gainExperience(int value) noexcept;
        void gainHealth(int value) noexcept;
        void takeDamage(int value) noexcept;
        void useMana(int value) noexcept;
        void gainMana(int value) noexcept;
        void equipWeapon(std::shared_ptr<Weapon> weapon) noexcept { pick(std::move(weapon), ItemSlotType::WEAPON); }

        // Actions
        virtual void respawn() noexcept;
        virtual void move() noexcept {}
        virtual void attack(Character &character) noexcept;
        virtual void defense(short damage) noexcept;
        virtual void pick(std::shared_ptr<Item> item, ItemSlotType slot) noexcept;
        virtual void drop(int itemID) noexcept;
        virtual void drop(ItemSlotType slot) noexcept;
        virtual void use(int itemID) noexcept {}

        // Others
        void determineHeritables(const Character &father, const Character &mother) noexcept;
    };
}

#endif // __CHARACTER_H__
