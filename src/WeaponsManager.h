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
        std::unordered_map<std::string, Weapon> _weapons;

    public:
        void startUp()
        {
            Manager::setType("WeaponsManager");
            LM.writeLog(Level::Debug, "WeaponsManager::startUp");
            Manager::startUp();
            initializeWeapons();
        }

        void shutDown()
        {
            _weapons.clear();
            Manager::shutDown();
            LM.writeLog(Level::Debug, "WeaponsManager::shutDown");
        }

        void initializeWeapons()
        {
            _weapons.insert({"Fists", {"Fists", SkillType::FIST, ItemRank::NORMAL, 2}});
            _weapons.insert({"Club", {"Club", SkillType::CLUB, ItemRank::NORMAL, 4}});
            _weapons.insert({"Dagger", {"Dagger", SkillType::SWORD, ItemRank::NORMAL, 4}});
            _weapons.insert({"Greatclub", {"Greatclub", SkillType::CLUB, ItemRank::NORMAL, 8}});
            _weapons.insert({"Handaxe", {"Handaxe", SkillType::AXE, ItemRank::NORMAL, 6}});
            _weapons.insert({"Javelin", {"Javelin", SkillType::DISTANCE, ItemRank::NORMAL, 6}});
            _weapons.insert({"Light hammer", {"Light hammer", SkillType::CLUB, ItemRank::NORMAL, 4}});
            _weapons.insert({"Mace", {"Mace", SkillType::CLUB, ItemRank::NORMAL, 6}});
            _weapons.insert({"Quaterstaff", {"Quaterstaff", SkillType::CLUB, ItemRank::NORMAL, 6}});
            _weapons.insert({"Sickle", {"Sickle", SkillType::SWORD, ItemRank::NORMAL, 4}});
            _weapons.insert({"Spear", {"Spear", SkillType::DISTANCE, ItemRank::NORMAL, 6}});
        }

        std::unordered_map<std::string, Weapon>
        getWeaponsList() const
        {
            return _weapons;
        }

        Weapon getWeapon(const std::string &name)
        {
            if (!isStarted())
                startUp();
            return _weapons.find(name)->second;
        }
    };
}
#endif // __WEAPONS_MANAGER_H__
