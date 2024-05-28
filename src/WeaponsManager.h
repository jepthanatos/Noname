#ifndef __WEAPONS_MANAGER_H__
#define __WEAPONS_MANAGER_H__

// Local includes
#include "Singleton.h"
#include "Manager.h"
#include "LogManager.h"
#include "Weapon.h"

// System includes
#include <vector>
#include <unordered_map>

// Two-letter acronym for easier access to manager
#define WM noname::WeaponsManager::getInstance()

namespace noname
{
    class WeaponsManager : public Manager, public Singleton<WeaponsManager>
    {

    private:
        std::unordered_map<std::string, Weapon> _weapons_list;

    public:
        void startUp()
        {
            Manager::setType("WeaponsManager");
            LM.writeLog(Level::Debug, "WeaponsManager::startUp");
            Manager::startUp();
        }

        void shutDown()
        {
            _weapons_list.clear();
            Manager::shutDown();
            LM.writeLog(Level::Debug, "WeaponsManager::shutDown");
        }

        void initializeWeapons()
        {
            _weapons_list.insert({"Fists", {"Fists", SkillType::FIST, 2}});
            _weapons_list.insert({"Club", {"Club", SkillType::CLUB, 4}});
            _weapons_list.insert({"Dagger", {"Dagger", SkillType::SWORD, 4}});
            _weapons_list.insert({"Greatclub", {"Greatclub", SkillType::CLUB, 8}});
            _weapons_list.insert({"Handaxe", {"Handaxe", SkillType::AXE, 6}});
            _weapons_list.insert({"Javelin", {"Javelin", SkillType::DISTANCE, 6}});
            _weapons_list.insert({"Light hammer", {"Light hammer", SkillType::CLUB, 4}});
            _weapons_list.insert({"Mace", {"Mace", SkillType::CLUB, 6}});
            _weapons_list.insert({"Quaterstaff", {"Quaterstaff", SkillType::CLUB, 6}});
            _weapons_list.insert({"Sickle", {"Sickle", SkillType::SWORD, 4}});
            _weapons_list.insert({"Spear", {"Spear", SkillType::DISTANCE, 6}});
        }

        std::unordered_map<std::string, Weapon> getWeaponsList() const { return _weapons_list; }

        Weapon getWeapon(const std::string &weapon) { return _weapons_list.find(weapon)->second; }
    };

}
#endif // __WEAPONS_MANAGER_H__
