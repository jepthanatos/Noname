// Local includes
#include "Character.h"
#include "Utils.h"
#include "HtmlBuilder.h"
#include "GameManager.h"
#include "FileManager.h"
#include "SkillsManager.h"

// System includes
#include <memory>
#include <cmath>     // std::ceil
#include <algorithm> // std::min std::max

namespace noname
{
    int Character::generateId()
    {
        static int cont{0};
        return cont++;
    }

    Character::Character() : _id{generateId()},
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
                             _skills(Utils::toInt(SkillType::LAST_SKILL), 1),
                             _skillTries(Utils::toInt(SkillType::LAST_SKILL), 0)
    {
        setLevel(1);
        setMagicLevel(1);
        writeCharacterInfo();
    }

    void Character::setLevel(short value)
    {
        _level = value;
        _nextLevelExperience = GM.getExpForLevel(_level + 1);
        setMaxHealth();
        setMaxMana();
    }

    void Character::setMagicLevel(short value)
    {
        _magicLevel = value;
        _nextLevelManaWasted = GM.getManaForLevel(_level + 1);
    }

    void Character::setMaxHealth()
    {
        _maxHealth = _maxHealth + _heritables.at(HeritableType::CONSTITUTION) + Utils::rollDie(1, _level);
    }

    void Character::setMaxMana()
    {
        _maxMana = _maxMana + _heritables.at(HeritableType::INTELLIGENCE) + Utils::rollDie(1, _level);
    }

    void Character::setSkill(SkillType skill, short value)
    {
        _skillTries.at(Utils::toInt(skill)) = value;
    }

    void Character::updateTries(SkillType skill)
    {
        ++_skillTries.at(Utils::toInt(skill));
        if (_skillTries.at(Utils::toInt(skill)) >= SM.getSkill(skill).getTriesNeeded())
        {
            _skillTries.at(Utils::toInt(skill)) = 0;
            ++_skills.at(Utils::toInt(skill));
            LM.writeLog(Level::Debug, "New value of " + SkillToString(skill) + " = " + std::to_string(_skills.at(Utils::toInt(skill))));
        }
    }

    void Character::gainExperience(int value)
    {
        if (value > 0)
        {
            _currentExperience += value;

            while (_currentExperience >= _nextLevelExperience)
            {
                setLevel(_level + 1);
            }
        }
        // Error handling TBD
    }

    void Character::gainHealth(int value)
    {
        if (value > 0)
        {
            _currentHealth = std::min(static_cast<short>(_currentHealth + value), static_cast<short>(_maxHealth));
        }
        // Error handling TBD
    }

    short Character::getAttackDamage() const
    {
        short doubleDamage{1};
        auto d20{Utils::rollDie(1, 20)};
        if (d20 > 1)
        {
            if (d20 == 20) // The character did a critical hit
            {
                doubleDamage = 2;
            }
            return (Utils::rollDie(1, _currentWeapon.getDie()) * doubleDamage) + _skills.at(Utils::toInt(_currentWeapon.getType()));
        }
        // The character missed the attack
        return 0;
    }

    void Character::respawn()
    {
        setLevel(_level);
        setMaxHealth();
        setMaxMana();
        _currentHealth = _maxHealth;
        _currentMana = _maxMana;
        _isDead = false;
    }

    void Character::takeDamage(int value)
    {
        if (value > 0)
        {
            _currentHealth = std::max(static_cast<int>(0), static_cast<int>(_currentHealth - value));
            if (_currentHealth == 0 && !_isDead)
            {
                _isDead = true;
                _currentExperience -= static_cast<unsigned long long>(std::ceil((_currentExperience * 25) / 100.0));
                _currentExperience = std::max(static_cast<unsigned long long>(0), static_cast<unsigned long long>(_currentExperience));
                while (_currentExperience < GM.getExpForLevel(_level - 1))
                {
                    _nextLevelExperience = GM.getExpForLevel(_level);
                    --_level;
                }
            }
        }
        // Error handling TBD
    }

    void Character::useMana(int value)
    {
        if (value > 0)
        {
            _currentMana -= value;
            _currentManaWasted += value;
            if (_currentManaWasted >= _nextLevelManaWasted)
            {
                ++_magicLevel;
                _nextLevelManaWasted = GM.getManaForLevel(_magicLevel + 1);
            }
        }
        // Error handling TBD
    }

    void Character::gainMana(int value)
    {
        if (value > 0)
        {
            _currentMana = std::min(static_cast<short>(_currentMana + value), static_cast<short>(_maxMana));
        }
        // Error handling TBD
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
        // Add Heritables here
        characterFile.write(infoTable.str());
    }

    void Character::attack()
    {
        auto damage{getAttackDamage()};
        // LM.writeLog(Level::Debug, "Character has attacked with damage equal to " + std::to_string(damage));
        if (damage > 0)
            updateTries(_currentWeapon.getType());
    }

    void Character::determineHeritables(const Character &father, const Character &mother)
    {
        _heritables.determineHeritablesValues(father._heritables, mother._heritables);
        writeCharacterInfo();
    }
}
