// Local includes
#include "Character.h"
#include "Utils.h"

// System includes

namespace noname
{
    Character::Character() : _currentExp{0}, _nextLevelExp{0}, _currentManaWasted{0}, _nextLevelManaWasted{0}, _level{0}, _magicLevel{0}, _currentHealth{0}, _maxHealth{0}, _currentMana{0}, _maxMana{0}, _currentCapacity{0}, _maxCapacity{0}
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
    }

    void Character::setLevel(short value)
    {
        _level = value;
        _nextLevelExp = getExpForLevel(_level + 1);
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
        return MANA_POINTS * level;
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
        _skillTries.find(skill)->second++;
        if ((skill == SkillType::DISTANCE and _skillTries.find(skill)->second >= DISTANCE_TRIES) or (_skillTries.find(skill)->second >= MELEE_TRIES))
        {
            _skillTries.find(skill)->second = 0;
            _skills.find(skill)->second++;
            LM.writeLog(Level::Debug, "New value of " + SkillToString(skill) + " = " + std::to_string(_skills.find(skill)->second));
        }
    }

    unsigned long long Character::getExpForLevel(short level)
    {
        return ((50ULL * level * level * level) - (150ULL * level * level) + (400ULL * level)) / 3ULL;
    }

    void Character::addExperience(unsigned long long exp)
    {
        _currentExp += exp;

        while (_currentExp >= _nextLevelExp)
        {
            _level++;
            setLevel(_level);
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

    void Character::takeDamage(short damage)
    {
        _currentHealth -= damage;
        if (_currentHealth <= 0)
        {
            _currentExp -= ceil((_currentExp * 25) / 100);
            if (_currentExp < getExpForLevel(_level))
                _level--;
        }
    }

    void Character::attack()
    {
        auto damage{getAttackDamage()};
        // LM.writeLog(Level::Debug, "Character has attacked with damage equal to " + std::to_string(damage));
        if (damage > 0)
            updateTries(_currentWeapon.getType());
    }
}
