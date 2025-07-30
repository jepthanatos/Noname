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
#include "AttackStrategy.h"
#include "EventSubject.h"
#include "Event.h"
#include "CharacterData.h"

namespace noname
{
    class Character : public EventSubject
    {
    public:
        // Constructor de copia personalizado para manejar unique_ptr
        Character(const Character& other);
        Character& operator=(const Character& other);
        Character(Character&&) = default;
        Character& operator=(Character&&) = default;
        
    protected:
        // Property solo para datos que realmente necesitan ser observables
        Property<int> _id;
        Property<std::string> _name;
        Property<short> _level;
        Property<short> _magicLevel;
        
        // Datos agrupados (sin Property overhead)
        HealthData health;
        ManaData mana;
        ExperienceData experience;
        ExperienceData magicExperience; // Para nivel mágico
        CapacityData capacity;
        SpeedData speed;
        
        // Arrays estáticos para skills (tamaño conocido)
        static constexpr size_t MAX_SKILLS = static_cast<size_t>(SkillType::LAST_SKILL);
        std::array<short, MAX_SKILLS> skills;
        std::array<short, MAX_SKILLS> skillTries;
        
        // Otros datos simples
        bool _isDead{false};
        Heritables _heritables;
        Inventory _inventory;
        
        // Strategy Pattern para combate
        std::unique_ptr<AttackStrategy> attackStrategy;

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

        // Helper methods for creating events
        Event createHealthEvent(EventType type, int oldValue, int newValue) const;
        Event createManaEvent(EventType type, int oldValue, int newValue) const;
        Event createCombatEvent(EventType type, const std::string& target, int damage = 0) const;
        Event createExperienceEvent(int expGained, int totalExp) const;
        Event createLevelEvent(EventType type, int oldLevel, int newLevel) const;
        Event createCharacterEvent(EventType type) const;

    public:
        Character();
        explicit Character(std::string_view name) : Character() { _name = std::string(name); }

        bool operator==(const Character &p) const noexcept { return _id == p._id; }

        [[nodiscard]] int getId() const noexcept { return _id; }
        [[nodiscard]] std::string getName() const noexcept { return _name; }
        [[nodiscard]] unsigned long long getExperience() const noexcept { return experience.current; }
        [[nodiscard]] unsigned long long getManaWasted() const noexcept { return magicExperience.current; }
        [[nodiscard]] short getLevel() const noexcept { return _level; }
        [[nodiscard]] short getMagicLevel() const noexcept { return _magicLevel; }
        [[nodiscard]] short getSkill(SkillType skill) const noexcept { return skills.at(static_cast<size_t>(skill)); }
        [[nodiscard]] int getCurrentHealth() const noexcept { return health.current; }
        [[nodiscard]] int getMaxHealth() const noexcept { return health.maximum; }
        [[nodiscard]] int getCurrentMana() const noexcept { return mana.current; }
        [[nodiscard]] int getMaxMana() const noexcept { return mana.maximum; }
        [[nodiscard]] bool isDead() const noexcept { return health.isDead(); }
        [[nodiscard]] short getHeritable(HeritableType value) const noexcept { return _heritables.at(value); }
        [[nodiscard]] short getAttackDamage() noexcept;
        [[nodiscard]] std::shared_ptr<Weapon> getWeapon() const noexcept { return _inventory.getWeapon(); }
        
        // Getters adicionales para acceso a las estructuras completas
        [[nodiscard]] const HealthData& getHealthData() const noexcept { return health; }
        [[nodiscard]] const ManaData& getManaData() const noexcept { return mana; }
        [[nodiscard]] const ExperienceData& getExperienceData() const noexcept { return experience; }
        [[nodiscard]] const ExperienceData& getMagicExperienceData() const noexcept { return magicExperience; }
        [[nodiscard]] const CapacityData& getCapacityData() const noexcept { return capacity; }
        [[nodiscard]] const SpeedData& getSpeedData() const noexcept { return speed; }
        
        void writeCharacterInfo() const;
        [[nodiscard]] const InventorySlots &getInventorySlots() const noexcept { return _inventory.getSlots(); }

        // Strategy Pattern para combate
        void setAttackStrategy(std::unique_ptr<AttackStrategy> strategy) { attackStrategy = std::move(strategy); }
        [[nodiscard]] AttackStrategy* getAttackStrategy() const { return attackStrategy.get(); }
        [[nodiscard]] int calculateAttackDamage() const;
        [[nodiscard]] bool performAttack(Character& target);
        [[nodiscard]] SkillType getRelevantSkillForAttack() const;

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
