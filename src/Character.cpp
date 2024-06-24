// Local includes
#include "Character.h"
#include "Utils.h"
#include "HtmlBuilder.h"
#include "FileManager.h"
#include "SkillsManager.h"

// System includes
#include <memory>

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
                             _nextLevelManaWasted{0}
    {
        static int cont{0};
        _id = cont;
        ++cont;

        setLevel(1);
        setMagicLevel(1);

        for (int i = 0; i < Utils::toInt(SkillType::LAST_SKILL); ++i)
        {
            _skills.push_back(1);
            _skillTries.push_back(0);
        }

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
        _maxHealth = _maxHealth + _heritables.at(HeritableType::CONSTITUTION) + Utils::rollDie(1, _level);
    }

    void Character::setMana()
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
        setHealth();
        setMana();
        _currentHealth = _maxHealth;
        _currentMana = _maxMana;
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
        writeCharacterInfo();
    }
}
