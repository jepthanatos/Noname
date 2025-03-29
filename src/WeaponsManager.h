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
#include <memory>

// Two-letter acronym for easier access to manager
#define WM noname::WeaponsManager::getInstance()

namespace noname
{
    class WeaponsManager : public Manager, public Singleton<WeaponsManager>
    {

    private:
        std::unordered_map<std::string, std::shared_ptr<Weapon>> _defaultWeapons;
        std::unordered_map<std::string, std::shared_ptr<Weapon>> _gameWeapons;

    public:
        void startUp() noexcept override
        {
            try
            {
                Manager::setType("WeaponsManager");
                LM.writeLog(Level::Debug, "WeaponsManager::startUp");
                Manager::startUp();
                initializeWeapons();
            }
            catch (const std::exception &e)
            {
                LM.writeLog(Level::Error, "Failed to start WeaponsManager: " + std::string(e.what()));
            }
        }

        void shutDown() noexcept override
        {
            _defaultWeapons.clear();
            Manager::shutDown();
            LM.writeLog(Level::Debug, "WeaponsManager::shutDown");
        }

        void initializeWeapons()
        {
            if (!_defaultWeapons.empty())
            {
                LM.writeLog(Level::Warning, "WeaponsManager::initializeWeapons - Weapons already initialized");
                return;
            }

            try
            {
                _defaultWeapons.insert({"Fists", std::make_shared<Weapon>("Fists", SkillType::FIST, ItemRank::NORMAL, 2)});
                _defaultWeapons.insert({"Club", std::make_shared<Weapon>("Club", SkillType::CLUB, ItemRank::NORMAL, 4)});
                _defaultWeapons.insert({"Dagger", std::make_shared<Weapon>("Dagger", SkillType::SWORD, ItemRank::NORMAL, 4)});
                _defaultWeapons.insert({"Greatclub", std::make_shared<Weapon>("Greatclub", SkillType::CLUB, ItemRank::NORMAL, 8)});
                _defaultWeapons.insert({"Handaxe", std::make_shared<Weapon>("Handaxe", SkillType::AXE, ItemRank::NORMAL, 6)});
                _defaultWeapons.insert({"Javelin", std::make_shared<Weapon>("Javelin", SkillType::DISTANCE, ItemRank::NORMAL, 6)});
                _defaultWeapons.insert({"Light hammer", std::make_shared<Weapon>("Light hammer", SkillType::CLUB, ItemRank::NORMAL, 4)});
                _defaultWeapons.insert({"Mace", std::make_shared<Weapon>("Mace", SkillType::CLUB, ItemRank::NORMAL, 6)});
                _defaultWeapons.insert({"Quaterstaff", std::make_shared<Weapon>("Quaterstaff", SkillType::CLUB, ItemRank::NORMAL, 6)});
                _defaultWeapons.insert({"Sickle", std::make_shared<Weapon>("Sickle", SkillType::SWORD, ItemRank::NORMAL, 4)});
                _defaultWeapons.insert({"Spear", std::make_shared<Weapon>("Spear", SkillType::DISTANCE, ItemRank::NORMAL, 6)});
            }
            catch (const std::exception &e)
            {
                LM.writeLog(Level::Error, "Failed to initialize weapons: " + std::string(e.what()));
            }
        }

        std::unordered_map<std::string, std::shared_ptr<Weapon>> getWeaponsList() const
        {
            if (!isStarted())
            {
                LM.writeLog(Level::Error, "WeaponsManager::getWeaponsList - Weapons Manager has not been started");
                return {};
            }
            return _defaultWeapons;
        }

        std::shared_ptr<Weapon> getWeapon(std::string_view name)
        {
            if (!isStarted())
            {
                LM.writeLog(Level::Error, "WeaponsManager::getWeapon - Weapons Manager has not been started");
                return NullWeapon::getInstance();
            }
            auto it = _defaultWeapons.find(std::string{name});
            if (it != _defaultWeapons.end())
                return it->second;
            LM.writeLog(Level::Warning, "Weapon not found: " + std::string{name});
            return NullWeapon::getInstance();
        }
    };
}
#endif // __WEAPONS_MANAGER_H__
