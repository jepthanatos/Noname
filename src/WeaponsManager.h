#ifndef __WEAPONS_MANAGER_H__
#define __WEAPONS_MANAGER_H__

// Local includes
#include "Singleton.h"
#include "Manager.h"
#include "LogManager.h"
#include "Weapon.h"

// System includes
#include <vector>

// Two-letter acronym for easier access to manager
#define WM noname::WeaponsManager::getInstance()

namespace noname
{
    class WeaponsManager : public Manager, public Singleton<WeaponsManager>
    {

    private:
        std::vector<Weapon> _weapons_list;

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

        void initWeapons()
        {
            _weapons_list.push_back({"Club", SkillType::CLUB, 4});
            _weapons_list.push_back({"Dagger", SkillType::SWORD, 4});
            _weapons_list.push_back({"Greatclub", SkillType::CLUB, 8});
            _weapons_list.push_back({"Handaxe", SkillType::AXE, 6});
            _weapons_list.push_back({"Javelin", SkillType::DISTANCE, 6});
            _weapons_list.push_back({"Light hammer", SkillType::CLUB, 4});
            _weapons_list.push_back({"Mace", SkillType::CLUB, 6});
            _weapons_list.push_back({"Quaterstaff", SkillType::CLUB, 6});
            _weapons_list.push_back({"Sickle", SkillType::SWORD, 4});
            _weapons_list.push_back({"Spear", SkillType::DISTANCE, 6});
        }

        std::vector<Weapon> getWeaponsList() const { return _weapons_list; }
    };

}
#endif // __WEAPONS_MANAGER_H__
