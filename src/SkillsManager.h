#ifndef __SKILLS_MANAGER_H__
#define __SKILLS_MANAGER_H__

// Local includes
#include "Singleton.h"
#include "Manager.h"
#include "LogManager.h"
#include "Skill.h"

// System includes
#include <unordered_map>

// Two-letter acronym for easier access to manager
#define SM noname::SkillsManager::getInstance()

namespace noname
{
    class SkillsManager : public Manager, public Singleton<SkillsManager>
    {
    private:
        std::unordered_map<SkillType, Skill> _skillsList;

    public:
        void startUp()
        {
            Manager::setType("SkillsManager");
            LM.writeLog(Level::Debug, "SkillsManager::startUp");
            Manager::startUp();
            initializeSkills();
        }

        void shutDown()
        {
            _skillsList.clear();
            Manager::shutDown();
            LM.writeLog(Level::Debug, "SkillsManager::shutDown");
        }

        void initializeSkills()
        {
            _skillsList.insert({SkillType::FIST, {0, 100, 50}});
            _skillsList.insert({SkillType::SWORD, {0, 100, 50}});
            _skillsList.insert({SkillType::AXE, {0, 100, 50}});
            _skillsList.insert({SkillType::CLUB, {0, 100, 50}});
            _skillsList.insert({SkillType::DISTANCE, {0, 100, 25}});
            _skillsList.insert({SkillType::SHIELDING, {0, 100, 100}});
        }

        std::unordered_map<SkillType, Skill> getSkillsList() const { return _skillsList; }

        Skill getSkill(const SkillType &skill) const
        {
            if (_skillsList.size() > 0)
                return _skillsList.find(skill)->second;
            return {0, 0, 0};
        }
    };

}
#endif // __SKILLS_MANAGER_H__
