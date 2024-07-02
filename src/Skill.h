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
        SHIELDING,
        LAST_SKILL
    };

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
        case SkillType::SHIELDING:
            return "SHIELDING";
        default:
            return "";
        }
    };

    class Skill
    {

    private:
        short _min_value;    // Minimum value the skill can have.
        short _max_value;    // Maximum value the skill can have.
        short _tries_needed; // Number of tries needed before next level.

    public:
        Skill(const short min_value, const short max_value, const short tries) : _min_value{min_value}, _max_value{max_value}, _tries_needed{tries} {}

        short getMinValue() const { return _min_value; }
        short getMaxValue() const { return _max_value; }
        short getTriesNeeded() const { return _tries_needed; }

        bool operator==(const Skill &skill)
        {
            return _min_value == skill._min_value && _max_value == skill._max_value && _tries_needed == skill._tries_needed;
        }
    };

}
#endif // __SKILL_H__
