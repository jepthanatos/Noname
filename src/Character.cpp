// Local includes
#include "Character.h"
#include "Utils.h"
#include "HtmlBuilder.h"
#include "GameManager.h"
#include "FileManager.h"
#include "SkillsManager.h"
#include "Inventory.h"
#include "LogManager.h"
#include "MeleeAttackStrategy.h"

// System includes
#include <memory>
#include <cmath>     // std::ceil
#include <algorithm> // std::min std::max
#include <atomic>

namespace noname
{
    int Character::generateId() noexcept
    {
        static std::atomic<int> cont{0};
        return cont++;
    };

    Character::Character() : _id{generateId()},
                             _name{"Noname"},
                             _level{0},
                             _magicLevel{0},
                             health{},           // HealthData con valores por defecto
                             mana{},             // ManaData con valores por defecto
                             experience{},       // ExperienceData con valores por defecto
                             magicExperience{},  // ExperienceData para maná gastado
                             capacity{},         // CapacityData con valores por defecto
                             speed{},            // SpeedData con valores por defecto
                             skills{},           // std::array inicializado a 0
                             skillTries{},       // std::array inicializado a 0
                             _isDead{false},
                             _heritables{},
                             _inventory{},
                             attackStrategy{std::make_unique<MeleeAttackStrategy>()}
    {
        setLevel(1);
        setMagicLevel(1);
        writeCharacterInfo();
    }

    // Constructor de copia personalizado
    Character::Character(const Character& other) :
        _id{generateId()}, // Nuevo ID para la copia
        _name{other._name},
        _level{other._level},
        _magicLevel{other._magicLevel},
        health{other.health},
        mana{other.mana},
        experience{other.experience},
        magicExperience{other.magicExperience},
        capacity{other.capacity},
        speed{other.speed},
        skills{other.skills},
        skillTries{other.skillTries},
        _isDead{other._isDead},
        _heritables{other._heritables},
        _inventory{other._inventory}
    {
        // Copia profunda de la estrategia de ataque
        if (other.attackStrategy) {
            // Como no tenemos un método clone virtual, creamos una nueva estrategia por defecto
            attackStrategy = std::make_unique<MeleeAttackStrategy>();
        }
    }

    // Operador de asignación personalizado
    Character& Character::operator=(const Character& other) {
        if (this != &other) {
            // No copiamos el ID (cada character mantiene su ID único)
            _name = other._name;
            _level = other._level;
            _magicLevel = other._magicLevel;
            health = other.health;
            mana = other.mana;
            experience = other.experience;
            magicExperience = other.magicExperience;
            capacity = other.capacity;
            speed = other.speed;
            skills = other.skills;
            skillTries = other.skillTries;
            _isDead = other._isDead;
            _heritables = other._heritables;
            _inventory = other._inventory;
            
            // Copia profunda de la estrategia de ataque
            if (other.attackStrategy) {
                attackStrategy = std::make_unique<MeleeAttackStrategy>();
            } else {
                attackStrategy.reset();
            }
        }
        return *this;
    }

    void Character::setLevel(short value) noexcept
    {
        _level = value;
        experience.setNextLevelRequirement(GM.getExpForLevel(_level + 1));
        setMaxHealth();
        setMaxMana();
        setMaxCapacity();
        updateSpeed();
    }

    void Character::setMagicLevel(short value) noexcept
    {
        _magicLevel = value;
        magicExperience.setNextLevelRequirement(GM.getManaForLevel(_level + 1));
    }

    void Character::setMaxHealth() noexcept
    {
        int newMaxHealth = health.maximum + _heritables.at(HeritableType::CONSTITUTION) + Utils::rollDie(1, _level);
        health.setMaximum(newMaxHealth);
    }

    void Character::setMaxMana() noexcept
    {
        int newMaxMana = mana.maximum + _heritables.at(HeritableType::INTELLIGENCE) + Utils::rollDie(1, _level);
        mana.setMaximum(newMaxMana);
    }

    void Character::setMaxCapacity() noexcept
    {
        int newMaxCapacity = capacity.maximum + _heritables.at(HeritableType::STRENGTH) + _heritables.at(HeritableType::CONSTITUTION) + Utils::rollDie(1, _level);
        capacity.setMaximum(newMaxCapacity);
        updateCurrentCapacity();
    }

    void Character::updateCurrentCapacity() noexcept
    {
        int inventoryWeight = _inventory.getWeight();
        capacity.current = std::max(0, capacity.maximum - inventoryWeight);
    }

    void Character::setSpeed() noexcept
    {
        int newBaseSpeed = speed.base + _heritables.at(HeritableType::STRENGTH) - _heritables.at(HeritableType::CONSTITUTION) + Utils::rollDie(1, _level);
        speed.setBase(newBaseSpeed);
        updateSpeed();
    }

    void Character::updateSpeed() noexcept
    {
        int penalty = _inventory.getWeight();
        speed.updateCurrent(penalty);
    }

    void Character::setSkill(SkillType skill, short value) noexcept
    {
        skillTries.at(static_cast<size_t>(skill)) = value;
    }

    void Character::updateTries(SkillType skill) noexcept
    {
        size_t skillIndex = static_cast<size_t>(skill);
        ++skillTries.at(skillIndex);
        if (skillTries.at(skillIndex) >= SM.getSkill(skill).getTriesNeeded())
        {
            skillTries.at(skillIndex) = 0;
            ++skills.at(skillIndex);
            LM.writeLog(Level::Debug, "New value of " + SkillToString(skill) + " = " + std::to_string(skills.at(skillIndex)));
        }
    }

    // Helper methods for creating events
    Event Character::createHealthEvent(EventType type, int oldValue, int newValue) const {
        Event event(type);
        event.setData<std::string>("character_name", _name.get());
        event.setData<int>("old_value", oldValue);
        event.setData<int>("new_value", newValue);
        event.setData<int>("character_level", _level.get());
        event.setData<int>("character_id", _id.get());
        return event;
    }

    Event Character::createManaEvent(EventType type, int oldValue, int newValue) const {
        Event event(type);
        event.setData<std::string>("character_name", _name.get());
        event.setData<int>("old_value", oldValue);
        event.setData<int>("new_value", newValue);
        event.setData<int>("character_level", _level.get());
        event.setData<int>("character_id", _id.get());
        return event;
    }

    Event Character::createCombatEvent(EventType type, const std::string& target, int damage) const {
        Event event(type);
        event.setData<std::string>("attacker", _name.get());
        event.setData<std::string>("target", target);
        if (damage > 0) {
            event.setData<int>("damage", damage);
        }
        event.setData<int>("attacker_level", _level.get());
        event.setData<int>("attacker_id", _id.get());
        return event;
    }

    Event Character::createExperienceEvent(int expGained, int totalExp) const {
        Event event(EventType::EXPERIENCE_GAINED);
        event.setData<std::string>("character_name", _name.get());
        event.setData<int>("experience_gained", expGained);
        event.setData<int>("total_experience", totalExp);
        event.setData<int>("character_level", _level.get());
        event.setData<int>("character_id", _id.get());
        return event;
    }

    Event Character::createLevelEvent(EventType type, int oldLevel, int newLevel) const {
        Event event(type);
        event.setData<std::string>("character_name", _name.get());
        event.setData<int>("old_level", oldLevel);
        event.setData<int>("new_level", newLevel);
        event.setData<int>("character_id", _id.get());
        return event;
    }

    Event Character::createCharacterEvent(EventType type) const {
        Event event(type);
        event.setData<std::string>("character_name", _name.get());
        event.setData<int>("final_level", _level.get());
        event.setData<int>("character_id", _id.get());
        return event;
    }

    void Character::gainExperience(int value) noexcept
    {
        if (value <= 0)
        {
            LM.writeLog(Level::Warning, "Invalid experience value: " + std::to_string(value));
            return;
        }

        int oldExp = experience.current;
        int oldLevel = _level.get();
        
        experience.gain(value);

        // Notificar experiencia ganada
        Event expEvent = createExperienceEvent(value, experience.current);
        notifyObservers(expEvent);

        while (experience.hasLeveledUp())
        {
            setLevel(_level + 1);
            
            // Notificar subida de nivel
            Event levelEvent = createLevelEvent(EventType::LEVEL_UP, oldLevel, _level.get());
            notifyObservers(levelEvent);
            
            oldLevel = _level.get(); // Actualizar para múltiples level ups
        }
    }

    void Character::gainHealth(int value) noexcept
    {
        if (value > 0)
        {
            health.heal(value);
        }
        // Error handling TBD
    }

    short Character::getAttackDamage() noexcept
    {
        return static_cast<short>(calculateAttackDamage());
    }
    
    int Character::calculateAttackDamage() const 
    {
        if (!attackStrategy) {
            return 0;
        }
        
        return attackStrategy->calculateDamage(*this, getWeapon());
    }
    
    bool Character::performAttack(Character& target) 
    {
        if (!attackStrategy) {
            LM.writeLog(Level::Debug, "No attack strategy set for character " + std::to_string(_id));
            return false;
        }
        
        int damage = calculateAttackDamage();
        
        if (damage <= 0) {
            LM.writeLog(Level::Debug, "Character " + std::to_string(_id) + " missed the attack or had no damage");
            return false;
        }
        
        // Consumir mana si es necesario
        int manaCost = attackStrategy->getManaCost();
        if (manaCost > 0) {
            useMana(manaCost);
        }
        
        // Log del ataque
        std::string attackType = attackStrategy->isCriticalHit() ? "critical" : "normal";
        LM.writeLog(Level::Debug, "Character " + std::to_string(_id) + 
                   " performed " + attackType + " attack with damage: " + std::to_string(damage));
        
        // Emitir evento de ataque exitoso
        Event damageEvent(EventType::DAMAGE_DEALT);
        damageEvent.setData<int>("character_id", _id.get());
        damageEvent.setData<int>("damage", damage);
        damageEvent.setData<std::string>("description", attackType + " attack dealt " + std::to_string(damage) + " damage");
        notifyObservers(damageEvent);
        
        // Aplicar daño al objetivo
        target.defense(damage);
        return true;
    }
    
    SkillType Character::getRelevantSkillForAttack() const 
    {
        if (!attackStrategy) {
            return SkillType::LAST_SKILL;
        }
        
        return attackStrategy->getRelevantSkill(getWeapon());
    }

    void Character::respawn() noexcept
    {
        setLevel(_level);
        setMaxHealth();
        setMaxMana();
        health.current = health.maximum;
        mana.current = mana.maximum;
        _isDead = false;
    }

    void Character::takeDamage(int value) noexcept
    {
        if (value <= 0)
        {
            LM.writeLog(Level::Warning, "Invalid damage value: " + std::to_string(value));
            return;
        }

        int oldHealth = health.current;
        health.takeDamage(value);

        // Notificar daño recibido
        Event damageEvent(EventType::DAMAGE_TAKEN);
        damageEvent.setData<std::string>("character_name", _name.get());
        damageEvent.setData<int>("damage", value);
        damageEvent.setData<int>("remaining_health", health.current);
        damageEvent.setData<int>("character_id", _id.get());
        notifyObservers(damageEvent);

        // Notificar cambio de salud
        Event healthEvent = createHealthEvent(EventType::HEALTH_CHANGED, oldHealth, health.current);
        notifyObservers(healthEvent);

        if (health.isDead() && !_isDead)
        {
            _isDead = true;
            LM.writeLog(Level::Info, "Character " + std::to_string(_id) + " has died.");
            
            // Notificar muerte
            Event deathEvent = createCharacterEvent(EventType::CHARACTER_DIED);
            notifyObservers(deathEvent);
            
            // Penalización de experiencia por muerte
            unsigned long long expPenalty = static_cast<unsigned long long>(std::ceil((experience.current * 25) / 100.0));
            experience.current = std::max(0ULL, experience.current - expPenalty);

            // Verificar si pierde niveles
            while (experience.current < GM.getExpForLevel(_level - 1))
            {
                experience.setNextLevelRequirement(GM.getExpForLevel(_level));
                --_level;
            }
        }
    }

    void Character::useMana(int value) noexcept
    {
        if (value > 0 && mana.consume(value))
        {
            magicExperience.gain(value);
            if (magicExperience.hasLeveledUp())
            {
                ++_magicLevel;
                magicExperience.setNextLevelRequirement(GM.getManaForLevel(_magicLevel + 1));
            }
        }
        // Error handling TBD
    }

    void Character::gainMana(int value) noexcept
    {
        if (value > 0)
        {
            mana.restore(value);
        }
        // Error handling TBD
    }

    void Character::writeCharacterInfo() const
    {
        FileManager characterFile;
        characterFile.initOutputFile("character-" + _id.toString() + ".html");
        characterFile.startUp();

        auto title{"Character Info: " + _id.toString()};

        HtmlBuilder infoTable{"table"};
        infoTable.add_child("caption", title);
        infoTable.add_child(_id.toHtmlBuilder("Player ID"));
        infoTable.add_child(_name.toHtmlBuilder("Name"));
        infoTable.add_child(_level.toHtmlBuilder("Level"));
        infoTable.add_child(_magicLevel.toHtmlBuilder("Magic Level"));
        infoTable.add_child(Property<int>(health.current).toHtmlBuilder("Current Health"));
        infoTable.add_child(Property<int>(health.maximum).toHtmlBuilder("Maximum Health"));
        infoTable.add_child(Property<int>(mana.current).toHtmlBuilder("Current Mana"));
        infoTable.add_child(Property<int>(mana.maximum).toHtmlBuilder("Maximum Mana"));
        infoTable.add_child(Property<int>(capacity.current).toHtmlBuilder("Current Capacity"));
        infoTable.add_child(Property<int>(capacity.maximum).toHtmlBuilder("Maximum Capacity"));
        infoTable.add_child(Property<unsigned long long>(experience.current).toHtmlBuilder("Current Experience"));
        infoTable.add_child(Property<unsigned long long>(experience.nextLevel).toHtmlBuilder("Next Level Experience"));
        infoTable.add_child(Property<unsigned long long>(magicExperience.current).toHtmlBuilder("Current Mana Wasted"));
        infoTable.add_child(Property<unsigned long long>(magicExperience.nextLevel).toHtmlBuilder("Next Level Mana Wasted"));
        // Add Heritables here
        characterFile.write(infoTable.str());
    }

    void Character::attack(Character &character) noexcept
    {
        if (performAttack(character)) {
            // El ataque fue exitoso, actualizar skills relevantes
            auto relevantSkill = getRelevantSkillForAttack();
            if (relevantSkill != SkillType::LAST_SKILL) {
                updateTries(relevantSkill);
            }
        }
    }

    void Character::defense(short damage) noexcept
    {
        auto shield{10};
        auto defense{shield + ((skills.at(Utils::toInt(SkillType::SHIELDING)) + 10) / 40)};
        if (damage <= defense) {
            updateTries(SkillType::SHIELDING);
            // No emitir evento aquí ya que no hay daño
        }
        else
        {
            int actualDamage = damage - defense;
            LM.writeLog(Level::Debug, "Character " + std::to_string(_id) + " has been damaged with " + std::to_string(actualDamage));
            takeDamage(actualDamage);  // esto emitirá el evento DAMAGE_TAKEN
        }
    }

    void Character::pick(std::shared_ptr<Item> item, ItemSlotType slot) noexcept
    {
        if (!item)
        {
            LM.writeLog(Level::Error, "Attempted to pick a null item.");
            return;
        }

        if (capacity.getAvailable() < item->getWeight())
        {
            LM.writeLog(Level::Debug, "Character " + std::to_string(_id) + " lacks capacity for item " + item->getName());
            return;
        }

        LM.writeLog(Level::Debug, "Character " + std::to_string(_id) + " has picked item " + item->getName());
        _inventory.storeItem(std::move(item), slot);
        updateCurrentCapacity();
    }

    void Character::drop(int itemID) noexcept
    {
        // auto item{_inventory.searchItem(itemID)};
        // LM.writeLog(Level::Debug, "Character " + _id.toString() + " has droped item " + item.getName());
        // _inventory.dropItem(item);
        updateCurrentCapacity();
    }

    void Character::drop(ItemSlotType slot) noexcept
    {
        auto item{_inventory.dropItem(slot)};
        LM.writeLog(Level::Debug, "Character " + std::to_string(_id) + " has droped item " + item->getName());
        auto weapon = getWeapon();
        if (!weapon)
        {
            std::shared_ptr<Weapon> fists = WM.getWeapon("Fists");
            this->equipWeapon(fists);
        }
        updateCurrentCapacity();
    }

    void Character::determineHeritables(const Character &father, const Character &mother) noexcept
    {
        _heritables.determineHeritablesValues(father._heritables, mother._heritables);
        writeCharacterInfo();
    }
}
