// Local includes
#include "Character.h"
#include "Utils.h"
#include "HtmlBuilder.h"
#include "FileManager.h"
#include "SkillsManager.h"

// System includes
#include <memory>
#include <random>

namespace noname
{
    Character::Character() : _id{0},
                             _name{"Noname"},
                             _level{0},
                             _magicLevel{0},
                             _currentHealth{0},
                             _maxHealth{0},
                             _currentMana{0},
                             _maxMana{0},
                             _currentCapacity{0},
                             _maxCapacity{0},
                             _currentExperience{0},
                             _nextLevelExperience{0},
                             _currentManaWasted{0},
                             _nextLevelManaWasted{0},
                             _strength{0},
                             _dextery{0},
                             _intelligence{0}
    {
        static int cont{0};
        _id = cont;
        ++cont;

        setLevel(1);
        setMagicLevel(1);

        for (int i = 0; i < static_cast<int>(SkillType::LAST_SKILL); ++i)
        {
            _skills.emplace(static_cast<SkillType>(i), 1);
            _skillTries.emplace(static_cast<SkillType>(i), 0);
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> attributesDist(0, 100);
        _strength = attributesDist(gen);
        _dextery = attributesDist(gen);
        _intelligence = attributesDist(gen);

        writeCharacterInfo();
    }

    Character::Character(const std::string &name) : _id{0},
                                                    _name{name},
                                                    _level{0},
                                                    _magicLevel{0},
                                                    _currentHealth{0},
                                                    _maxHealth{0},
                                                    _currentMana{0},
                                                    _maxMana{0},
                                                    _currentCapacity{0},
                                                    _maxCapacity{0},
                                                    _currentExperience{0},
                                                    _nextLevelExperience{0},
                                                    _currentManaWasted{0},
                                                    _nextLevelManaWasted{0},
                                                    _strength{0},
                                                    _dextery{0},
                                                    _intelligence{0}
    {
        static int cont{0};
        _id = cont;
        ++cont;

        setLevel(1);
        setMagicLevel(1);

        for (int i = 0; i < static_cast<int>(SkillType::LAST_SKILL); ++i)
        {
            _skills.emplace(static_cast<SkillType>(i), 1);
            _skillTries.emplace(static_cast<SkillType>(i), 0);
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> attributesDist(0, 100);
        _strength = attributesDist(gen);
        _dextery = attributesDist(gen);
        _intelligence = attributesDist(gen);

        writeCharacterInfo();
    }

    void Character::setLevel(short value)
    {
        _level = value;
        _nextLevelExperience = getExpForLevel(_level + 1);
        setHealth();
        setMana();
    }

    void Character::setMagicLevel(short value)
    {
        _magicLevel = value;
        _nextLevelManaWasted = getManaForLevel(_level + 1);
    }

    unsigned long long Character::getManaForLevel(short level) const
    {
        return 1600 * level;
    }

    void Character::setHealth()
    {
        _currentHealth = 10 * _level;
        _maxHealth = _currentHealth;
    }

    void Character::setMana()
    {
        _currentMana = 10 * _level;
        _maxMana = _currentMana;
    }

    void Character::setSkill(SkillType skill, short value)
    {
        _skillTries.find(skill)->second = value;
    }

    void Character::updateTries(SkillType skill)
    {
        ++_skillTries.find(skill)->second;
        if (_skillTries.find(skill)->second >= SM.getSkill(skill).getTriesNeeded())
        {
            _skillTries.find(skill)->second = 0;
            ++_skills.find(skill)->second;
            LM.writeLog(Level::Debug, "New value of " + SkillToString(skill) + " = " + std::to_string(_skills.find(skill)->second));
        }
    }

    unsigned long long Character::getExpForLevel(short level)
    {
        return ((50ULL * level * level * level) - (150ULL * level * level) + (400ULL * level)) / 3ULL;
    }

    void Character::addExperience(unsigned long long exp)
    {
        _currentExperience += exp;

        while (_currentExperience >= _nextLevelExperience)
        {
            setLevel(_level + 1);
        }
    }

    short Character::getAttackDamage() const
    {
        short doubleDamage{1};
        auto d20{Utils::rollDice(1, 20)};
        if (d20 > 1)
        {
            if (d20 == 20) // The character did a critical hit
            {
                doubleDamage = 2;
            }
            return (Utils::rollDice(1, _currentWeapon.getDice()) * doubleDamage) + _skills.find(_currentWeapon.getType())->second;
        }
        // The character missed the attack
        return 0;
    }

    void Character::respawn()
    {
        setLevel(_level);
        _isDead = false;
    }

    void Character::takeDamage(int damage)
    {
        _currentHealth -= damage;
        if (_currentHealth <= 0)
        {
            --_level;
            _currentExperience = _currentExperience - ceil((_currentExperience * 25) / 100);
            _isDead = true;
        }
    }

    void Character::writeCharacterInfo()
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
        infoTable.add_child(_currentHealth.toHtmlBuilder("Current Health"));
        infoTable.add_child(_maxHealth.toHtmlBuilder("Maximum Health"));
        infoTable.add_child(_currentMana.toHtmlBuilder("Current Mana"));
        infoTable.add_child(_maxMana.toHtmlBuilder("Maximum Mana"));
        infoTable.add_child(_currentCapacity.toHtmlBuilder("Current Capacity"));
        infoTable.add_child(_maxCapacity.toHtmlBuilder("Maximum Capacity"));
        infoTable.add_child(_currentExperience.toHtmlBuilder("Current Experience"));
        infoTable.add_child(_nextLevelExperience.toHtmlBuilder("Next Level Experience"));
        infoTable.add_child(_currentManaWasted.toHtmlBuilder("Current Mana Wasted"));
        infoTable.add_child(_nextLevelManaWasted.toHtmlBuilder("Next Level Mana Wasted"));
        infoTable.add_child(_strength.toHtmlBuilder("Strength"));
        infoTable.add_child(_dextery.toHtmlBuilder("Dextery"));
        infoTable.add_child(_intelligence.toHtmlBuilder("Intelligence"));
        characterFile.write(infoTable.str());
    }

    void Character::attack()
    {
        auto damage{getAttackDamage()};
        // LM.writeLog(Level::Debug, "Character has attacked with damage equal to " + std::to_string(damage));
        if (damage > 0)
            updateTries(_currentWeapon.getType());
    }

    void Character::determineAttributes(const Character &father, const Character &mother)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 2);
        std::uniform_real_distribution<> mutationChance(0.0, 1.0);
        double mutationRate = 0.1; // 10% chance of mutation

        // Function to apply complex mutation
        auto applyComplexMutation = [&](int attribute)
        {
            std::uniform_int_distribution<> mutationTypeDist(1, 3);
            int mutationType = mutationTypeDist(gen);

            LM.writeLog(Level::Debug, "Character " + std::string{_name} + " has complex mutation of type " + std::to_string(mutationType));

            switch (mutationType)
            {
            case 1:
            { // Random increase/decrease within a range
                std::uniform_int_distribution<> changeDist(-3, 3);
                attribute += changeDist(gen);
                break;
            }
            case 2:
            {                                                            // Percentage change
                std::uniform_real_distribution<> percentDist(-0.1, 0.1); // -10% to +10%
                attribute = static_cast<int>(attribute * (1.0 + percentDist(gen)));
                break;
            }
            case 3:
            { // Random set within a new range
                std::uniform_int_distribution<> newValueDist(1, 20);
                attribute = newValueDist(gen);
                break;
            }
            }

            // Ensure attribute stays within reasonable bounds
            attribute = std::max(1, attribute);
            attribute = std::min(20, attribute);
            return attribute;
        };
        // Strength
        short geneStrength = (dist(gen) == 1) ? father._strength : mother._strength;
        if (mutationChance(gen) < mutationRate)
        {
            geneStrength = applyComplexMutation(geneStrength);
        }
        _strength = geneStrength;

        // Dexterity
        short geneDexterity = (dist(gen) == 1) ? father._dextery : mother._dextery;
        if (mutationChance(gen) < mutationRate)
        {
            geneDexterity = applyComplexMutation(geneDexterity);
        }
        _dextery = geneDexterity;

        // Intelligence
        short geneIntelligence = (dist(gen) == 1) ? father._intelligence : mother._intelligence;
        if (mutationChance(gen) < mutationRate)
        {
            geneIntelligence = applyComplexMutation(geneIntelligence);
        }
        _intelligence = geneIntelligence;

        writeCharacterInfo();
    }
}
