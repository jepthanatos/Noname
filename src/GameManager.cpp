// System includes
#include <iostream>
#include <filesystem>
#include <vector>

// Local includes
#include "GameManager.h"
#include "LogManager.h"
#include "WeaponsManager.h"
#include "RankingManager.h"
#include "CreaturesManager.h"
#include "SkillsManager.h"
#include "Utils.h"

namespace noname
{
    int GameManager::initialization(int argc, char *argv[])
    {
        // Define the usage string printed to console.
        std::string usage{"Usage is: Noname_run"};

        // if something is not right then
        // return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    void GameManager::startUp()
    {
        Manager::setType("GameManager");
        LM.startUp();
        LM.writeLog(Level::Debug, "Starting services...");
        WM.startUp();
        CM.startUp();
        RM.startUp();
        SM.startUp();
        started = LM.isStarted() and CM.isStarted() and WM.isStarted() and RM.isStarted() and SM.isStarted();
    }

    void GameManager::shutDown()
    {
        LM.writeLog(Level::Debug, "Closing services...");
        WM.shutDown();
        CM.shutDown();
        RM.shutDown();
        SM.shutDown();
        LM.shutDown();
    }

    void GameManager::run()
    {
        LM.writeLog(Level::Debug, "Starting program...");
        for (int i = 0; i < 100; ++i)
        {
            Player player{};
            player.equipWeapon("Club");
            player.gainExperience(getExpForLevel(Utils::rollDie(1, 100)));
            for (int j = 1; j < Utils::rollDie(1, 10000); ++j)
                player.attack();
            RM.addPlayer(std::move(player));
            player.writeCharacterInfo();
        }
        RM.printAllRankings();
        LM.writeLog(Level::Debug, "Ending program...");
    }

    unsigned long long GameManager::getExpForLevel(short level) const
    {
        return ((50ULL * level * level * level) - (150ULL * level * level) + (400ULL * level)) / 3ULL;
    }

    unsigned long long GameManager::getManaForLevel(short level) const
    {
        return 1600 * level;
    }
}
