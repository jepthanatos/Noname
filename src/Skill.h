#ifndef __SKILL_H__
#define __SKILL_H__

// System includes
#include <string>

namespace noname
{
    enum class SkillType
    {
        FIST,
        SWORD,
        AXE,
        CLUB,
        DISTANCE,
        MAGIC,
        LAST_SKILL
    };

    static const short MANA_POINTS = 1600;
    static const short MELEE_TRIES = 50;
    static const short DISTANCE_TRIES = 25;
    static const short SHIELDING = 100;
    // static const short FISHING = 20;

    static std::string SkillToString(SkillType s)
    {
        switch (s)
        {
        case SkillType::FIST:
            return "FIST";
        case SkillType::SWORD:
            return "SWORD";
        case SkillType::AXE:
            return "AXE";
        case SkillType::CLUB:
            return "CLUB";
        case SkillType::DISTANCE:
            return "DISTANCE";
        case SkillType::MAGIC:
            return "MAGIC";
        default:
            return "";
        }
    };

    template <SkillType _skill>
    class Skill
    {

    private:
        // short _id;
        short _min_value;    // Minimum value the skill can have.
        short _max_value;    // Maximum value the skill can have.
        short _tries_needed; // Number of tries needed before next level.

    public:
        Skill(const short min_value, const short max_value, const short tries) : _min_value{min_value}, _max_value{max_value}, _tries_needed{tries}
        {
        }

        virtual ~Skill(){};

        SkillType getType() const { return _skill; }

        short getMinValue() const { return _min_value; }

        short getMaxValue() const { return _max_value; }

        short getTriesNeeded() const { return _tries_needed; }
    };

}
#endif // __SKILL_H__
