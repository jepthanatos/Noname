#ifndef __SKILLS_MANAGER_H__
#define __SKILLS_MANAGER_H__

// Local includes
#include "Singleton.h"
#include "Manager.h"
#include "LogManager.h"
#include "Skill.h"

// System includes
#include <unordered_map>
#include <string>

// Two-letter acronym for easier access to manager
#define SM noname::SkillsManager::getInstance()

namespace noname
{
    class SkillsManager : public Manager, public Singleton<SkillsManager>
    {
    private:
        std::unordered_map<SkillType, Skill> _skillsList;

    public:
        void startUp() noexcept
        {
            Manager::setType("SkillsManager");
            LM.writeLog(Level::Debug, "SkillsManager::startUp");
            Manager::startUp();
            initializeSkills();
        }

        void shutDown() noexcept
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
            if (!isStarted())
            {
                LM.writeLog(Level::Warning, "SkillsManager::getSkill - Manager not started. Starting now.");
                SM.startUp();
            }

            auto it = _skillsList.find(skill);
            if (it != _skillsList.end())
            {
                return it->second;
            }

            LM.writeLog(Level::Error, "Skill not found: " + SkillToString(skill));
            return {0, 0, 0};
        }
    };

}
#endif // __SKILLS_MANAGER_H__
