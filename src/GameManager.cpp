// System includes
#include <iostream>
#include <filesystem>
#include <vector>

// Local includes
#include "GameManager.h"
#include "LogManager.h"
#include "WeaponsManager.h"
#include "RankingManager.h"
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
        RM.startUp();
        started = LM.isStarted() and WM.isStarted() and RM.isStarted();
    }

    void GameManager::shutDown()
    {
        LM.writeLog(Level::Debug, "Closing services...");
        WM.shutDown();
        RM.shutDown();
        LM.shutDown();
    }

    void GameManager::run()
    {
        LM.writeLog(Level::Debug, "Starting program...");
        WM.initializeWeapons();
        for (int i = 0; i < 100; ++i)
        {
            Player player{};
            player.equipWeapon("Club");
            player.addExperience(Character::getExpForLevel(Utils::rollDice(1, 100)));
            for (int j = 1; j < Utils::rollDice(1, 10000); ++j)
                player.attack();
            RM.addPlayer(std::move(player));
        }
        RM.printAllRankings();
        LM.writeLog(Level::Debug, "Ending program...");
    }

}
