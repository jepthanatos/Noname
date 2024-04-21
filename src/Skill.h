#ifndef __SKILL_H__
#define __SKILL_H__

// System includes.
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

    static const short MAGIC_LEVEL = 1600;
    static const short MELEE_SKILLS = 50;
    static const short DISTANCE_SKILLS = 25;
    static const short SHIELDING = 100;
    static const short FISHING = 20;

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
        // SKILL m_id;     // Unique game engine defined identifier.
        int m_min_value;    // Minimum value the skill can have.
        int m_max_value;    // Maximum value the skill can have.
        int m_tries_needed; // Number of tries needed before next level.

    public:
        Skill(int min_value, int max_value, int tries) : m_min_value{min_value}, m_max_value{max_value}, m_tries_needed{tries}
        {
        }

        virtual ~Skill(){};

        SkillType getType() { return _skill; }
    };

}
//------------------------------------------------------------------------------
#endif // __SKILL_H__
