// Local includes
#include "Player.h"

// System includes
#include <cmath>
#include <math.h>
#include <random>

namespace noname
{
    Player::Player() : _weapon(SkillType::SWORD, 6)
    {
        static int cont{0};
        _id = cont;
        ++cont;
        for (int i = 0; i < static_cast<int>(SkillType::LAST_SKILL); ++i)
        {
            _skills.emplace(static_cast<SkillType>(i), 1);
            _skillTries.emplace(static_cast<SkillType>(i), 0);
        }
        setLevel(1);
    }

    short Player::getLevel() const
    {
        return _level;
    }

    void Player::setLevel(short value)
    {
        _level = value;
        setHealth();
        _currentLevelExp = getExpForLevel(_level);
        _nextLevelExp = getExpForLevel(_level + 1);
    }

    void Player::setHealth()
    {
        _health = 10 * _level;
    }

    short Player::getSkill(SkillType skill) const
    {
        return _skills.find(skill)->second;
    }

    void Player::setSkill(SkillType skill, short value)
    {
        _skills.find(skill)->second = value;
    }

    void Player::updateTries(SkillType skill)
    {
        _skillTries.find(skill)->second++;
        if ((skill == SkillType::DISTANCE and _skillTries.find(skill)->second >= DISTANCE_TRIES) or (_skillTries.find(skill)->second >= MELEE_TRIES))
        {
            _skillTries.find(skill)->second = 0;
            _skills.find(skill)->second++;
        }
    }

    short Player::getDamage(SkillType skill) const
    {
        auto rollDice = [](int min, int max)
        {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
            return dist6(rng);
        };
        short doubleDamage{1};
        auto d20{rollDice(1, 20)};
        if (d20 > 1)
        {
            if (d20 == 20)
            {
                doubleDamage = 2;
            }
            return (rollDice(1, _weapon.getDice()) * doubleDamage) + _skills.find(skill)->second;
        }
        return 0;
    }

    short Player::getHealth() const
    {
        return _health;
    }

    void Player::takeDamage(short damage)
    {
        _health -= damage;
        if (_health <= 0)
        {
            _currentLevelExp -= ceil((_currentLevelExp * 25) / 100);
            if (_currentLevelExp < getExpForLevel(_level))
                _level--;
        }
    }

    void Player::addExperience(long exp)
    {
        _currentLevelExp += exp;

        while (_currentLevelExp >= _nextLevelExp)
        {
            short newLevel = getLevel();
            ++newLevel;
            setLevel(newLevel);
        }
    }

    unsigned long long Player::getExperience() const
    {
        return _currentLevelExp;
    }

    unsigned long long Player::getExpForLevel(short level) const
    {
        return ((50ULL * level * level * level) - (150ULL * level * level) + (400ULL * level)) / 3ULL;
    }
}
